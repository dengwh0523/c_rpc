#include <stdio.h>
#include <string.h>
#include <time.h>

#include "debug.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__WIN32__) || defined(_WIN32)
// For Windoze, we need to implement our own gettimeofday()

// used to make sure that static variables in gettimeofday() aren't initialized simultaneously by multiple threads
static LONG initializeLock_gettimeofday = 0;  

#if !defined(_WIN32_WCE)
#include <sys/timeb.h>
#endif

int 
gettimeofday(struct timeval* tp, int* tz) {
  static LARGE_INTEGER tickFrequency, epochOffset;

  static int isInitialized = 0;

  LARGE_INTEGER tickNow;

#if !defined(_WIN32_WCE)
  QueryPerformanceCounter(&tickNow);
#else
  tickNow.QuadPart = GetTickCount();
#endif
 
  if (!isInitialized) {
    if(1 == InterlockedIncrement(&initializeLock_gettimeofday)) {
#if !defined(_WIN32_WCE)
      // For our first call, use "ftime()", so that we get a time with a proper epoch.
      // For subsequent calls, use "QueryPerformanceCount()", because it's more fine-grain.
      struct timeb tb;
      ftime(&tb);
      tp->tv_sec = (long)tb.time;
      tp->tv_usec = 1000*tb.millitm;

      // Also get our counter frequency:
      QueryPerformanceFrequency(&tickFrequency);
#else
      /* FILETIME of Jan 1 1970 00:00:00. */
      const LONGLONG epoch = 116444736000000000LL;
      FILETIME fileTime;
      LARGE_INTEGER time;
      GetSystemTimeAsFileTime(&fileTime);

      time.HighPart = fileTime.dwHighDateTime;
      time.LowPart = fileTime.dwLowDateTime;

      // convert to from 100ns time to unix timestamp in seconds, 1000*1000*10
      tp->tv_sec = (long)((time.QuadPart - epoch) / 10000000L);

      /*
        GetSystemTimeAsFileTime has just a seconds resolution,
        thats why wince-version of gettimeofday is not 100% accurate, usec accuracy would be calculated like this:
        // convert 100 nanoseconds to usec
        tp->tv_usec= (long)((time.QuadPart - epoch)%10000000L) / 10L;
      */
      tp->tv_usec = 0;

      // resolution of GetTickCounter() is always milliseconds
      tickFrequency.QuadPart = 1000;
#endif     
      // compute an offset to add to subsequent counter times, so we get a proper epoch:
      epochOffset.QuadPart
          = tp->tv_sec * tickFrequency.QuadPart + (tp->tv_usec * tickFrequency.QuadPart) / 1000000L - tickNow.QuadPart;
      
      // next caller can use ticks for time calculation
      isInitialized = 1; 
      return 0;
    } else {
        InterlockedDecrement(&initializeLock_gettimeofday);
        // wait until first caller has initialized static values
        while(!isInitialized){
          Sleep(1);
        }
    }
  }

  // adjust our tick count so that we get a proper epoch:
  tickNow.QuadPart += epochOffset.QuadPart;

  tp->tv_sec =  (long)(tickNow.QuadPart / tickFrequency.QuadPart);
  tp->tv_usec = (long)(((tickNow.QuadPart % tickFrequency.QuadPart) * 1000000L) / tickFrequency.QuadPart);

  return 0;
}
#endif

void init_thread_cnt(THREAD_TEST_S * pstThreadTest) {
	memset(pstThreadTest, 0, sizeof(THREAD_TEST_S));
	pstThreadTest->lock = lock_create();
}

void add_thread_cnt(THREAD_TEST_S * pstThreadTest) {
	if (NULL == pstThreadTest) {
		printf("para error\n");
		return;
	}
	lock_lock(pstThreadTest->lock);
	pstThreadTest->thread_cnt++;
	lock_unlock(pstThreadTest->lock);
}

void del_thread_cnt(THREAD_TEST_S * pstThreadTest) {
	if (NULL == pstThreadTest) {
		printf("para error\n");
		return;
	}
	lock_lock(pstThreadTest->lock);
	if (pstThreadTest->thread_cnt > 0) {
		pstThreadTest->thread_cnt--;
	} else {
		printf("calc thread_cnt error\n");
	}
	lock_unlock(pstThreadTest->lock);
}

int get_thread_cnt(THREAD_TEST_S * pstThreadTest) {
	int cnt;
	if (NULL == pstThreadTest) {
		printf("para error\n");
		return -1;
	}
	lock_lock(pstThreadTest->lock);
	cnt = pstThreadTest->thread_cnt;
	lock_unlock(pstThreadTest->lock);

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


#ifndef _WIN32
unsigned int get_tick_time() {
	struct timespec tp;
	unsigned int cur_time;
	clock_gettime(CLOCK_MONOTONIC, &tp);
	cur_time = tp.tv_sec*1000 + tp.tv_nsec/1000000;

	return cur_time;
}
#else
unsigned int get_tick_time() {
	return GetTickCount();
}
#endif

int str_to_int(char * str) {
	return strtol(str, NULL, 0);
}

char * for_each(char * buf, int len, int step, void * id, for_each_cb cb) {
	int i;
	for (i = 0; i < len; i++) {
		if (0 == cb(buf, id)) {
			return buf;
		}
		buf += step;
	}

	return NULL;
}

char * fast_for_each(char * buf, int len, int step, void * id, for_each_cb cb) {
	int i;
	int ret = 0;
	int left = 0;
	int right = len-1;
	int next;
	char * tmp_buf;

	// cmp min
	tmp_buf = buf;
	ret = cb(tmp_buf, id);
	if (0 == ret) {
		return tmp_buf;
	}
	if (ret < 0) {
		return NULL;
	}

	// cmp max
	tmp_buf = buf+(len-1)*step;
	ret = cb(tmp_buf, id);
	if (0 == ret) {
		return tmp_buf;
	}
	if (ret > 0) {
		return NULL;
	}

	//else	
	for (i = 1; i < len-1; i++) {
		next = MIDDLE(left, right);
		if (next == left) {
			next = right;
		}
		tmp_buf = buf + next*step;
		ret = cb(tmp_buf, id);
		if (0 == ret) {
			return tmp_buf;
		} else if (ret < 0) {
			right = next;			
		} else {
			left = next;
		}
		if (right-left == 1) {
			break;
		}
	}

	return NULL;
}

#ifdef __cplusplus
}
#endif

