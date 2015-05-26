#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <pthread.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG

#define STR_ARRAY_LEN(str)	(sizeof(str)/sizeof(str[0]))

#ifdef DEBUG
#define trace(fmt...) \
do{\
	char __buf1[64], __buf2[1024];\
    snprintf(__buf1, sizeof(__buf1), "[%s:%d-%s] ", __FILE__, __LINE__, __FUNCTION__);\
    snprintf(__buf2, sizeof(__buf2), fmt);\
    printf("%s%s", __buf1, __buf2);\
} while(0)
#else
#define trace(fmt...) do {} while(0)
#endif

typedef struct {
	pthread_mutex_t lock;
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

#endif

