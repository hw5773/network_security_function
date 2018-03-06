/**
 * @file file.h
 * @author Hyunwoo Lee
 * @date 21 Feb 2018
 * @brief This file is to define the attributes and functions for the record
 * chain
 */

#ifndef __RECORD_H__
#define __RECORD_H__

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/queue.h>

#include "prf.h"

/**
 * @brief Data structure for global MAC
 */
typedef struct entry
{
  unsigned char *writer;                    /**< The writer who modifies the message */
  unsigned char *prior_msg_hash;            /**< The prior message which is modified */
  unsigned char *modification_hash;         /**< The hash value which shows the modification */
  TAILQ_ENTRY(entry) entries;               /**< The pointer to the next entry */
} MR_ENTRY;

/**
 * @brief Data structure for the modification record
 */
typedef struct modification_record
{
  unsigned char *source_mac;              /**< Endpoint MAC */
  TAILQ_HEAD(,entry) global_macs_head;      /**< Modification Record, a series of global MACs */
} MOD_RECORD;

int init_record(MOD_RECORD **mr, int len);  /**< Initialize the modification record */
int free_record(MOD_RECORD *mr);            /**< Destruct the modification record */

int init_entry(MR_ENTRY **entry, int len);
int free_entry(MR_ENTRY *entry);

int add_source_mac(SECURITY_PARAMS *sp, MOD_RECORD *mr, unsigned char *msg, int mlen, unsigned char *key, int klen);
int add_global_mac(SECURITY_PARAMS *sp, MOD_RECORD *mr, unsigned char *id, int idlen, unsigned char *key, int klen, unsigned char *prev, int plen, unsigned char *next, int nlen);

int verify_record();
int print_record(MOD_RECORD *mr);

#endif /* __RECORD_H__ */
