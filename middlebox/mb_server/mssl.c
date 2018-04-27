#include "mssl.h"

void handle_error(const char *file, int lineno, const char *msg) {
  fprintf(stderr, "** %s:%i %s\n", file, lineno, msg);
  ERR_print_errors_fp(stderr);
  exit(1);
}

#define int_error(msg) handle_error(__FILE__, __LINE__, msg)

void die(const char *msg) {
  perror(msg);
  exit(1);
}

void print_unencrypted_data(char *buf, size_t len) {
  printf("%.*s", (int)len, buf);
}

void ssl_client_init(struct ssl_client *p)
{
  memset(p, 0, sizeof(struct ssl_client));

  p->rbio = BIO_new(BIO_s_mem());
  p->wbio = BIO_new(BIO_s_mem());

  p->ssl = SSL_new(ctx);

  SSL_set_accept_state(p->ssl); /* sets ssl to work in server mode. */
  SSL_set_bio(p->ssl, p->rbio, p->wbio);

  p->io_on_read = print_unencrypted_data;
}

void ssl_client_cleanup(struct ssl_client *p)
{
  SSL_free(p->ssl);   /* free the SSL object and its BIO's */
  free(p->write_buf);
  free(p->encrypt_buf);
}

int ssl_client_want_write(struct ssl_client *cp) {
  return (cp->write_len>0);
}

static enum sslstatus get_sslstatus(SSL* ssl, int n)
{
  switch (SSL_get_error(ssl, n))
  {
    case SSL_ERROR_NONE:
      return SSLSTATUS_OK;
    case SSL_ERROR_WANT_WRITE:
    case SSL_ERROR_WANT_READ:
      return SSLSTATUS_WANT_IO;
    case SSL_ERROR_ZERO_RETURN:
    case SSL_ERROR_SYSCALL:
    default:
      return SSLSTATUS_FAIL;
  }
}

void send_unencrypted_bytes(const char *buf, size_t len)
{
  client.encrypt_buf = (char*)realloc(client.encrypt_buf, client.encrypt_len + len);
  memcpy(client.encrypt_buf+client.encrypt_len, buf, len);
  client.encrypt_len += len;
}

void queue_encrypted_bytes(const char *buf, size_t len)
{
  client.write_buf = (char*)realloc(client.write_buf, client.write_len + len);
  memcpy(client.write_buf+client.write_len, buf, len);
  client.write_len += len;
}

int on_read_cb(char* src, size_t len)
{
  char buf[DEFAULT_BUF_SIZE];
  enum sslstatus status;
  int n;
  printf("len: %lu\n", len);

  while (len > 0) {
    n = BIO_write(client.rbio, src, len);
    printf("after BIO_write: %d\n", n);

    if (n<=0)
      return -1; /* if BIO write fails, assume unrecoverable */

    src += n;
    len -= n;

    if (!SSL_is_init_finished(client.ssl)) {
      n = SSL_accept(client.ssl);
      printf("after accept: %d\n", n);
      status = get_sslstatus(client.ssl, n);

      /* Did SSL request to write bytes? */
      if (status == SSLSTATUS_WANT_IO)
        do {
          n = BIO_read(client.wbio, buf, sizeof(buf));
          printf("after BIO_read: %d\n", n);
          if (n > 0)
            queue_encrypted_bytes(buf, n);
          else if (!BIO_should_retry(client.wbio))
            return -1;
        } while (n>0);

      if (status == SSLSTATUS_FAIL)
        return -1;

      printf("SSL_is_init_finished accept: %d\n", SSL_is_init_finished(client.ssl));
      if (!SSL_is_init_finished(client.ssl))
        return 0;
    }

    /* The encrypted data is now in the input bio so now we can perform actual
     * read of unencrypted data. */

    do {
      n = SSL_read(client.ssl, buf, sizeof(buf));
      printf("SSL_read bytes: %d\n", n);
      if (n > 0)
        client.io_on_read(buf, (size_t)n);
    } while (n > 0);

    status = get_sslstatus(client.ssl, n);

    if (status == SSLSTATUS_WANT_IO)
      do {
        n = BIO_read(client.wbio, buf, sizeof(buf));
        if (n > 0)
          queue_encrypted_bytes(buf, n);
        else if (!BIO_should_retry(client.wbio))
          return -1;
      } while (n>0);

    if (status == SSLSTATUS_FAIL)
      return -1;
  }
  return 0;
}
int do_encrypt()
{
  char buf[DEFAULT_BUF_SIZE];
  enum sslstatus status;

  if (!SSL_is_init_finished(client.ssl))
    return 0;

  while (client.encrypt_len>0) {
    int n = SSL_write(client.ssl, client.encrypt_buf, client.encrypt_len);
    status = get_sslstatus(client.ssl, n);

    if (n>0) {
      /* consume the waiting bytes that have been used by SSL */
      if ((size_t)n<client.encrypt_len)
        memmove(client.encrypt_buf, client.encrypt_buf+n, client.encrypt_len-n);
      client.encrypt_len -= n;
      client.encrypt_buf = (char*)realloc(client.encrypt_buf, client.encrypt_len);

      /* take the output of the SSL object and queue it for socket write */
      do {
        n = BIO_read(client.wbio, buf, sizeof(buf));
        if (n > 0)
          queue_encrypted_bytes(buf, n);
        else if (!BIO_should_retry(client.wbio))
          return -1;
      } while (n>0);
    }

    if (status == SSLSTATUS_FAIL)
      return -1;

    if (n==0)
      break;
  }
  return 0;
}

int do_sock_read()
{
  char buf[DEFAULT_BUF_SIZE];
  ssize_t n = read(client.fd, buf, sizeof(buf));
  if (n>0)
    return on_read_cb(buf, (size_t)n);
  else
    return -1;
}

/* Write encrypted bytes to the socket. */
int do_sock_write()
{
  ssize_t n = write(client.fd, client.write_buf, client.write_len);
  printf("after write: %lu\n", n);
  if (n>0) {
    if ((size_t)n<client.write_len)
      memmove(client.write_buf, client.write_buf+n, client.write_len-n);
    client.write_len -= n;
    client.write_buf = (char*)realloc(client.write_buf, client.write_len);
    return 0;
  }
  else
    return -1;
}

void msg_callback(int write, int version, int content_type, const void *buf, size_t len, SSL *ssl, void *arg)
{
  int i;
  unsigned char *p;
  p = (unsigned char *)buf;

  printf("write operation? %d\n", write);
  printf("version? 0x%x\n", version);
  printf("content type? ");

  switch(content_type)
  {
    case 20:
      printf("change cipher spec\n");
      break;
    case 21:
      printf("alert\n");
      break;
    case 22:
      printf("handshake\n");
      break;
    case 23:
      printf("application data\n");
      break;
    default:
      printf("invalid\n");
  }

  for (i=0; i<len; i++)
  {
    printf("%02X ", p[i]);
    if (i % 8 == 7)
      printf("\n");
  }
  printf("\n");
}

void ssl_init(char *cert, char *priv) {
  printf("initialising SSL\n");

  /* SSL library initialisation */
  SSL_library_init();
  OpenSSL_add_all_algorithms();
  SSL_load_error_strings();
  ERR_load_BIO_strings();
  ERR_load_crypto_strings();

  /* create the SSL server context */
  ctx = SSL_CTX_new(SSLv23_server_method());
  if (!ctx)
    die("SSL_CTX_new()");

  /* Load certificate and private key files, and check consistency  */
  int err;
  err = SSL_CTX_use_certificate_file(ctx, cert,  SSL_FILETYPE_PEM);
  if (err != 1)
    int_error("SSL_CTX_use_certificate_file failed");
  else
    printf("certificate file loaded ok\n");

  /* Indicate the key file to be used */
  err = SSL_CTX_use_PrivateKey_file(ctx, priv, SSL_FILETYPE_PEM);
  if (err != 1)
    int_error("SSL_CTX_use_PrivateKey_file failed");
  else
    printf("private-key file loaded ok\n");

  /* Make sure the key and certificate file match. */
  if (SSL_CTX_check_private_key(ctx) != 1)
    int_error("SSL_CTX_check_private_key failed");
  else
    printf("private key verified ok\n");

  /* Recommended to avoid SSLv2 & SSLv3 */
  SSL_CTX_set_options(ctx, SSL_OP_ALL|SSL_OP_NO_SSLv2|SSL_OP_NO_SSLv3);
  SSL_CTX_set_msg_callback(ctx, msg_callback);
}

