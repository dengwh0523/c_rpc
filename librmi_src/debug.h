#ifndef __DEBUG_H__
#define __DEBUG_H__

#ifndef _WIN32
#include <sys/time.h>
#else
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "thread.h"


#ifdef __cplusplus
extern "C" {
#endif

#define DEBUG

#define STR_ARRAY_LEN(str)	(sizeof(str)/sizeof(str[0]))
#define OFFSET(Type, member) (int)(&( ((Type*)0)->member) )
#define SIZE(Type, member, num) (sizeof(((Type*)0)->member)/num)

#ifdef _WIN32
#define  snprintf  _snprintf
#endif

#ifdef DEBUG
#define trace(fmt, ...) \
do{\
	char __buf1[64], __buf2[1024];\
    snprintf(__buf1, sizeof(__buf1), "[%s:%d-%s] ", __FILE__, __LINE__, __FUNCTION__);\
    snprintf(__buf2, sizeof(__buf2), fmt);\
    printf("%s%s", __buf1, __buf2);\
} while(0)
#else
#define trace(fmt, ...) do {} while(0)
#endif

#if defined(__WIN32__) || defined(_WIN32)
// For Windoze, we need to implement our own gettimeofday()
extern int gettimeofday(struct timeval*, int*);
#endif

typedef struct {
	lock * lock;
	int thread_cnt;
} THREAD_TEST_S;

typedef struct {
	struct timeval otv;
	struct timeval ntv;
	unsigned int used_time;
} TIME_USED_S;

void	init_thread_cnt(THREAD_TEST_S * pstThreadTest);
void	add_thread_cnt(THREAD_TEST_S * pstThreadTest);
void	del_thread_cnt(THREAD_TEST_S * pstThreadTest);
int		get_thread_cnt(THREAD_TEST_S * pstThreadTest);

int start_time(TIME_USED_S * pstTimeUsed);
int end_time(TIME_USED_S * pstTimeUsed);
unsigned int get_used_time(TIME_USED_S * pstTimeUsed);

unsigned int get_tick_time(void);	// unit: ms

#ifdef __cplusplus
}
#endif

#endif

