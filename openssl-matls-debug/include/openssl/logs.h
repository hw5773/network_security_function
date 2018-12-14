/** 
 * @file logs.h
 * @author Hyunwoo Lee
 * @date 21 Feb 2018
 * @brief This file is to define log messages
 */

#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include <assert.h>
#include <errno.h>

#ifdef DEBUG
int log_idx;
unsigned char ipb[4];
#define MA_LOG(msg) \
  fprintf(stderr, "[matls] %s: %s\n", __func__, msg)
#define MA_LOG1d(msg, arg1) \
  fprintf(stderr, "[matls] %s: %s: %d\n", __func__, msg, arg1);
#define MA_LOG1x(msg, arg1) \
  fprintf(stderr, "[matls] %s: %s: %x\n", __func__, msg, arg1);
#define MA_LOG1p(msg, arg1) \
  fprintf(stderr, "[matls] %s: %s: %p\n", __func__, msg, arg1);
#define MA_LOG1s(msg, arg1) \
  fprintf(stderr, "[matls] %s: %s: %s\n", __func__, msg, arg1);
#define MA_LOG1lu(msg, arg1) \
  fprintf(stderr, "[matls] %s: %s: %lu\n", __func__, msg, arg1);
#define MA_LOG1ld(msg, arg1) \
  fprintf(stderr, "[matls] %s: %s: %ld\n", __func__, msg, arg1);
#define MA_LOG1u(msg, arg1) \
  fprintf(stderr, "[matls] %s: %s: %u\n", __func__, msg, arg1);
#define MA_LOGip(msg, ip) \
  ipb[0] = ip & 0xFF; \
  ipb[1] = (ip >> 8) & 0xFF; \
  ipb[2] = (ip >> 16) & 0xFF; \
  ipb[3] = (ip >> 24) & 0xFF; \
  fprintf(stderr, "[matls] %s: %s: %d.%d.%d.%d\n", __func__, msg, ipb[0], ipb[1], ipb[2], ipb[3]);
#define MA_LOG2s(msg, arg1, arg2) \
  fprintf(stderr, "[matls] %s: %s (%d bytes) ", __func__, msg, arg2); \
  for (log_idx=0; log_idx<arg2; log_idx++) \
  { \
    if (log_idx % 10 == 0) \
      fprintf(stderr, "\n"); \
    fprintf(stderr, "%02X ", arg1[log_idx]); \
  } \
  fprintf(stderr, "\n");
#define MA_LOGmac(msg, mac) \
  fprintf(stderr, "[matls] %s: %s: %x %x %x %x %x %x\n", __func__, msg, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
#else
#define MA_LOG(msg)
#define MA_LOG1d(msg, arg1)
#define MA_LOG1x(msg, arg1)
#define MA_LOG1p(msg, arg1)
#define MA_LOG1s(msg, arg1)
#define MA_LOG1lu(msg, arg1)
#define MA_LOG1ld(msg, arg1)
#define MA_LOG1u(msg, arg1)
#define MA_LOGip(msg, ip)
#define MA_LOG2s(msg, arg1, arg2)
#define MA_LOGmac(msg, mac)
#endif /* DEBUG */

#ifdef DEBUG
#define PRINTK(msg, arg1, arg2) \
  fprintf(stderr, "[matls] %s: %s (%d bytes) \n", __func__, msg, arg2); \
  for (idx=0; idx<arg2; idx++) \
  { \
    if (idx % 10 == 0) \
      fprintf(stderr, "\n"); \
    fprintf(stderr, "%02X ", arg1[idx]); \
  } \
  fprintf(stderr, "\n");
#else
#define PRINTK(msg, arg1, arg2) 
#endif /* DEBUG */

unsigned long get_current_microseconds();

#ifdef TIME_LOG
unsigned long mstart, mend;
#define MSTART(msg, side) \
	mstart = get_current_microseconds(); \
	printf("[TT] %s:%s:%d: %s) %s start\n", __FILE__, __func__, __LINE__, side, msg);
#define MEND(msg, side) \
	mend = get_current_microseconds(); \
	printf("[TT] %s:%s:%d: %s) %s end: %lu us\n", __FILE__, __func__, __LINE__, side, msg, mend - mstart);
#define MEASURE(msg, side) \
	printf("[TT] %s:%s:%d: %s) %s: %lu\n", __FILE__, __func__, __LINE__, side, msg, get_current_microseconds());
#else
#define MSTART(msg, side)
#define MEND(msg, side)
#define MEASURE(msg, side) 
#endif /* TIME_LOG */

#ifdef LOGGER
typedef struct log_record
{
  char *name;
  unsigned long time;
} log_t;

#define NUM_OF_LOGS 150

#define SERVER_ACCEPT_START 0
#define SERVER_CLIENT_HELLO_START 1
#define SERVER_CLIENT_HELLO_END 2
#define SERVER_SERVER_HELLO_START 3
#define SERVER_SERVER_HELLO_END 4
#define SERVER_SERVER_CERTIFICATE_START 5
#define SERVER_SERVER_CERTIFICATE_END 6
#define SERVER_SERVER_KEY_EXCHANGE_START 7
#define SERVER_SERVER_KEY_EXCHANGE_END 8
#define SERVER_SERVER_HELLO_DONE_START 9
#define SERVER_SERVER_HELLO_DONE_END 10
#define SERVER_CLIENT_KEY_EXCHANGE_START 11
#define SERVER_CLIENT_KEY_EXCHANGE_END 12
#define SERVER_CLIENT_CCS_START 13
#define SERVER_CLIENT_CCS_END 14
#define SERVER_CLIENT_FINISHED_START 15
#define SERVER_CLIENT_FINISHED_END 16
#define SERVER_CLIENT_EXTENDED_FINISHED_START 17
#define SERVER_CLIENT_EXTENDED_FINISHED_END 18
#define SERVER_SERVER_CCS_START 19
#define SERVER_SERVER_CCS_END 20
#define SERVER_SERVER_FINISHED_START 21
#define SERVER_SERVER_FINISHED_END 22
#define SERVER_SERVER_EXTENDED_FINISHED_START 23
#define SERVER_SERVER_EXTENDED_FINISHED_END 24
#define SERVER_ACCEPT_END 25
#define SERVER_SERVER_MODIFICATION_GENERATE_START 26
#define SERVER_SERVER_MODIFICATION_GENERATE_END 27
#define SERVER_SERVER_MODIFICATION_VERIFY_START 28
#define SERVER_SERVER_MODIFICATION_VERIFY_END 29

#define CLIENT_CONNECT_START 30
#define CLIENT_CLIENT_HELLO_START 31
#define CLIENT_CLIENT_HELLO_END 32
#define CLIENT_SERVER_HELLO_START 33
#define CLIENT_SERVER_HELLO_END 34
#define CLIENT_SERVER_CERTIFICATE_START 35
#define CLIENT_SERVER_CERTIFICATE_END 36
#define CLIENT_SERVER_KEY_EXCHANGE_START 37
#define CLIENT_SERVER_KEY_EXCHANGE_END 38
#define CLIENT_SERVER_HELLO_DONE_START 39
#define CLIENT_SERVER_HELLO_DONE_END 40
#define CLIENT_CLIENT_KEY_EXCHANGE_START 41
#define CLIENT_CLIENT_KEY_EXCHANGE_END 42
#define CLIENT_CLIENT_CCS_START 43
#define CLIENT_CLIENT_CCS_END 44
#define CLIENT_CLIENT_FINISHED_START 45
#define CLIENT_CLIENT_FINISHED_END 46
#define CLIENT_CLIENT_EXTENDED_FINISHED_START 47
#define CLIENT_CLIENT_EXTENDED_FINISHED_END 48
#define CLIENT_SERVER_CCS_START 49
#define CLIENT_SERVER_CCS_END 50
#define CLIENT_SERVER_FINISHED_START 51
#define CLIENT_SERVER_FINISHED_END 52
#define CLIENT_SERVER_EXTENDED_FINISHED_START 53
#define CLIENT_SERVER_EXTENDED_FINISHED_END 54
#define CLIENT_CONNECT_END 55
#define CLIENT_SERVER_MODIFICATION_GENERATE_START 56
#define CLIENT_SERVER_MODIFICATION_GENERATE_END 57
#define CLIENT_SERVER_MODIFICATION_VERIFY_START 58
#define CLIENT_SERVER_MODIFICATION_VERIFY_END 59


#define CLIENT_CLIENT_HELLO_1S 60
#define CLIENT_CLIENT_HELLO_1E 61
#define CLIENT_CLIENT_HELLO_2S 62
#define CLIENT_CLIENT_HELLO_2E 63
#define CLIENT_CLIENT_HELLO_3S 64
#define CLIENT_CLIENT_HELLO_3E 65
#define CLIENT_CLIENT_HELLO_4S 66
#define CLIENT_CLIENT_HELLO_4E 67
#define CLIENT_CLIENT_HELLO_5S 68
#define CLIENT_CLIENT_HELLO_5E 69
#define CLIENT_CLIENT_HELLO_6S 70
#define CLIENT_CLIENT_HELLO_6E 71
#define CLIENT_CLIENT_HELLO_7S 72
#define CLIENT_CLIENT_HELLO_7E 73

#define SERVER_CLIENT_HELLO_1S 80
#define SERVER_CLIENT_HELLO_1E 81
#define SERVER_CLIENT_HELLO_2S 82
#define SERVER_CLIENT_HELLO_2E 83
#define SERVER_CLIENT_HELLO_3S 84
#define SERVER_CLIENT_HELLO_3E 85
#define SERVER_CLIENT_HELLO_4S 86
#define SERVER_CLIENT_HELLO_4E 87
#define SERVER_CLIENT_HELLO_5S 88
#define SERVER_CLIENT_HELLO_5E 89
#define SERVER_CLIENT_HELLO_6S 90
#define SERVER_CLIENT_HELLO_6E 91
#define SERVER_CLIENT_HELLO_7S 92
#define SERVER_CLIENT_HELLO_7E 93

#define CLIENT_SERVER_HELLO_1S 100
#define CLIENT_SERVER_HELLO_1E 101
#define CLIENT_SERVER_HELLO_2S 102
#define CLIENT_SERVER_HELLO_2E 103
#define CLIENT_SERVER_HELLO_3S 104
#define CLIENT_SERVER_HELLO_3E 105
#define CLIENT_SERVER_HELLO_4S 106
#define CLIENT_SERVER_HELLO_4E 107
#define CLIENT_SERVER_HELLO_5S 108
#define CLIENT_SERVER_HELLO_5E 109
#define CLIENT_SERVER_HELLO_6S 110
#define CLIENT_SERVER_HELLO_6E 111
#define CLIENT_SERVER_HELLO_7S 112
#define CLIENT_SERVER_HELLO_7E 113

#define SERVER_SERVER_HELLO_1S 120
#define SERVER_SERVER_HELLO_1E 121
#define SERVER_SERVER_HELLO_2S 122
#define SERVER_SERVER_HELLO_2E 123
#define SERVER_SERVER_HELLO_3S 124
#define SERVER_SERVER_HELLO_3E 125
#define SERVER_SERVER_HELLO_4S 126
#define SERVER_SERVER_HELLO_4E 127
#define SERVER_SERVER_HELLO_5S 128
#define SERVER_SERVER_HELLO_5E 129
#define SERVER_SERVER_HELLO_6S 130
#define SERVER_SERVER_HELLO_6E 131
#define SERVER_SERVER_HELLO_7S 132
#define SERVER_SERVER_HELLO_7E 133

#define SERVER_HANDSHAKE_START 140
#define SERVER_HANDSHAKE_END 141
#define SERVER_SERVE_HTML_START 142
#define SERVER_SERVE_HTML_END 143

#define CLIENT_GENERATE_ACCOUNTABILITY_KEYS_START 146
#define CLIENT_GENERATE_ACCOUNTABILITY_KEYS_END 147
#define SERVER_GENERATE_ACCOUNTABILITY_KEYS_START 148
#define SERVER_GENERATE_ACCOUNTABILITY_KEYS_END 149

/*
#define SERVER_PARSE_CLIENT_TLSEXT_START 74
#define SERVER_PARSE_CLIENT_TLSEXT_END 75

#define CLIENT_PARSE_SERVER_HELLO_1S 80
#define CLIENT_PARSE_SERVER_HELLO_1E 81
#define CLIENT_PARSE_SERVER_HELLO_2S 82
#define CLIENT_PARSE_SERVER_HELLO_2E 83
#define CLIENT_PARSE_SERVER_HELLO_3S 84
#define CLIENT_PARSE_SERVER_HELLO_3E 85
#define CLIENT_PARSE_SERVER_HELLO_4S 86
#define CLIENT_PARSE_SERVER_HELLO_4E 87
#define CLIENT_PARSE_SERVER_HELLO_5S 88
#define CLIENT_PARSE_SERVER_HELLO_5E 89
#define CLIENT_PARSE_SERVER_HELLO_6S 90
#define CLIENT_PARSE_SERVER_HELLO_6E 91
#define CLIENT_PARSE_SERVER_HELLO_7S 92
#define CLIENT_PARSE_SERVER_HELLO_7E 93
*/

#define CLIENT_HANDSHAKE_START 0
#define CLIENT_HANDSHAKE_END 1
#define CLIENT_CERT_VALIDATION_START 2
#define CLIENT_CERT_VALIDATION_END 3
#define CLIENT_FETCH_HTML_START 4
#define CLIENT_FETCH_HTML_END 5
#define CLIENT_EXTENDED_FINISHED_START 6
#define CLIENT_EXTENDED_FINISHED_END 7
#define CLIENT_TCP_CONNECT_START 8
#define CLIENT_TCP_CONNECT_END 9
#define CLIENT_MODIFICATION_RECORD_START 10
#define CLIENT_MODIFICATION_RECORD_END 11

int lidx;
FILE *log_file;
#define INITIALIZE_LOG(arr) \
  if (arr) { \
    for (lidx=0; lidx<NUM_OF_LOGS; lidx++) \
      arr[lidx].time = 0; \
  }

#define RECORD_LOG(arr, n) \
  if (arr) { \
    arr[n].name = #n; \
    arr[n].time = get_current_microseconds(); \
  } 

#define PRINT_LOG(arr) \
  if (arr) { \
    for ((lidx)=0; (lidx) < (NUM_OF_LOGS); (lidx)++) \
      printf("%s: %lu\n", arr[lidx].name, arr[lidx].time); \
  }

#define INTERVAL(arr, a, b) \
  if (arr) { \
    printf("Time from %s to %s: %lu us\n", arr[a].name, arr[b].name, arr[b].time - arr[a].time); \
  }

#define FINALIZE(arr, fname) \
  if (arr) { \
    log_file = fopen(fname, "a"); \
    for (lidx = 0; lidx < NUM_OF_LOGS; lidx++) \
    { \
      if (arr[lidx].time > 0) \
        fprintf(log_file, "%lu, %d, %s\n", arr[lidx].time, lidx, arr[lidx].name); \
    } \
    fclose(log_file); \
  }
  
extern log_t time_log[NUM_OF_LOGS];
#else
#define INITIALIZE_LOG(arr) 
#define RECORD_LOG(arr, n)
#define PRINT_LOG(arr)
#define INTERVAL(arr, a, b)
#define FINALIZE(arr, fname)
#endif /* TIME_LOG */

#endif /* __MB_LOG__ */