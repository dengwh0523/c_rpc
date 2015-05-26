#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include "debug.h"

void init_thread_cnt(THREAD_TEST_S * pstThreadTest) {
	memset(pstThreadTest, 0, sizeof(THREAD_TEST_S));
	pthread_mutex_init(&pstThreadTest->lock, NULL);
}

void add_thread_cnt(THREAD_TEST_S * pstThreadTest) {
	if (NULL == pstThreadTest) {
		printf("para error\n");
		return;
	}
	pthread_mutex_lock(&pstThreadTest->lock);
	pstThreadTest->thread_cnt++;
	pthread_mutex_unlock(&pstThreadTest->lock);
}

void del_thread_cnt(THREAD_TEST_S * pstThreadTest) {
	if (NULL == pstThreadTest) {
		printf("para error\n");
		return;
	}
	pthread_mutex_lock(&pstThreadTest->lock);
	if (pstThreadTest->thread_cnt > 0) {
		pstThreadTest->thread_cnt--;
	} else {
		printf("calc thread_cnt error\n");
	}
	pthread_mutex_unlock(&pstThreadTest->lock);
}

int get_thread_cnt(THREAD_TEST_S * pstThreadTest) {
	int cnt;
	if (NULL == pstThreadTest) {
		printf("para error\n");
		return;
	}
	pthread_mutex_lock(&pstThreadTest->lock);
	cnt = pstThreadTest->thread_cnt;
	pthread_mutex_unlock(&pstThreadTest->lock);

	return cnt;
}

int start_time(TIME_USED_S * pstTimeUsed) {
	gettimeofday(&pstTimeUsed->otv, NULL);

	return 0;
}

int end_time(TIME_USED_S * pstTimeUsed) {
	gettimeofday(&pstTimeUsed->ntv, NULL);
	pstTimeUsed->used_time = (pstTimeUsed->ntv.tv_sec - pstTimeUsed->otv.tv_sec)*1000000 + (pstTimeUsed->ntv.tv_usec - pstTimeUsed->otv.tv_usec);

	return 0;
}

unsigned int get_used_time(TIME_USED_S * pstTimeUsed) {
	(void)end_time(pstTimeUsed);
	return pstTimeUsed->used_time;
}


unsigned int get_tick_time() {
	struct timespec tp;
	unsigned int cur_time;
	clock_gettime(CLOCK_MONOTONIC, &tp);
	cur_time = tp.tv_sec*1000 + tp.tv_nsec/1000000;

	return cur_time;
}

