#ifndef __MSSL_API_H__
#define __MSSL_API_H__

#include <stdint.h>
#include <netinet/in.h>
#include <sys/uio.h>

#ifndef UNUSED
#define UNUSED(x) (void) x
#endif

#ifndef INPORT_ANY
#define INPORT_ANY (uint16_t) 0
#endif

struct mssl_context
{
  int cpu;
};

typedef struct mssl_context *mctx_t;
typedef void (*mssl_sighandler_t)(int);

struct mssl_conf
{
#define APP_NAME_LEN 40
#define MOS_APP 20

  int num_cores;
  int max_concurrency;
  uint64_t cpu_mask;

  int max_num_buffers;
  int clnt_rcvbuf_size;
  int clnt_sndbuf_size;
  int serv_rcvbuf_size;
  int serv_sndbuf_size;

  int tcp_timewait;
  int tcp_timeout;

#define MOS_APP_ARGC 20
  uint64_t app_cpu_mask[MOS_APP];
  char *app_argv[MOS_APP][MOS_APP_ARGC];
  int app_argc[MOS_APP];
  int num_app;
};

struct app_context
{
  mctx_t mctx;
  int socket_id;
  struct conn_filter *cf;
  int ep_id;
};

int mssl_init();
int mssl_getconf(struct mssl_conf *conf);
int mssl_setconf(const struct mssl_conf *conf);
int mssl_core_affinitize(int cpu);
mctx_t mssl_create_context(int cpu);
int mssl_destroy_context(mctx_t mctx);

int get_num_cpus(void);
#endif /* __MSSL_API_H__ */
