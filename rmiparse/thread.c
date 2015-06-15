#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#ifdef _WIN32
#include <windows.h>
#include <process.h>
#else
#include <unistd.h>
#include <pthread.h>
#endif

#include "thread.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_THREADS 64

struct _thread
{
	void *id;
	void * (*f)(void*);
	void *data;
	int cnt;
	volatile int running, busy;

#ifndef _WIN32
	pthread_cond_t cond;
	pthread_mutex_t mutex;
#endif
};

typedef struct _thread *thread;

struct thread_pool_
{
	thread threads[MAX_THREADS];
	int cnt, last;
};

struct lock_
{
#ifdef _WIN32
	CRITICAL_SECTION mutex;
#else
	pthread_mutex_t mutex;
#endif
};

lock *lock_create()
{
	lock *l = (lock*)calloc(1, sizeof(struct lock_));

#ifdef _WIN32
	InitializeCriticalSection(&l->mutex);
#else
	pthread_mutex_init(&l->mutex, NULL);
#endif

	return l;
}

void lock_lock(lock *l)
{
	if (!l)
		return;

#ifdef _WIN32
	EnterCriticalSection(&l->mutex);
#else
	pthread_mutex_lock(&l->mutex);
#endif
}

void lock_unlock(lock *l)
{
	if (!l)
		return;

#ifdef _WIN32
	LeaveCriticalSection(&l->mutex);
#else
	pthread_mutex_unlock(&l->mutex);
#endif
}

void lock_destroy(lock *l)
{
	if (!l)
		return;

#ifndef _WIN32
	pthread_mutex_destroy(&l->mutex);
#endif
}

// Crude atomicity, uses global lock.

static lock *g_lock = NULL;

/*uint64_t atomic_addu64(uint64_t *v, int n)*/
/*{*/
/*	uint64_t tmp;*/
/*	if (!g_lock)*/
/*		g_lock = lock_create();*/
/**/
/*	lock_lock(g_lock);*/
/*	tmp = *v;*/
/*	*v += n;*/
/*	lock_unlock(g_lock);*/
/*	return tmp;*/
/*}*/

/*int64_t atomic_add64(int64_t *v, int n)*/
/*{	*/
/*	int64_t tmp;*/
/*	if (!g_lock)*/
/*		g_lock = lock_create();*/
/**/
/*	lock_lock(g_lock);*/
/*	tmp = *v;*/
/*	*v += n;*/
/*	lock_unlock(g_lock);*/
/*	return tmp;*/
/*}*/

int atomic_inc(int *v)
{
	int tmp;
	if (!g_lock)
		g_lock = lock_create();

	lock_lock(g_lock);
	tmp = (*v)++;
	lock_unlock(g_lock);
	return tmp;
}

int atomic_dec(int *v)
{
	int tmp;
	if (!g_lock)
		g_lock = lock_create();

	lock_lock(g_lock);
	tmp = --(*v);
	lock_unlock(g_lock);
	return tmp;
}

int atomic_dec_and_zero(int *v, int *v2)
{
	int tmp;
	if (!g_lock)
		g_lock = lock_create();

	lock_lock(g_lock);
	tmp = --(*v);
	if (tmp == 0) *v2 = 0;
	lock_unlock(g_lock);
	return tmp;
}

static void thread_pause(thread t)
{
#ifdef _WIN32
	t->busy = 0;
	SuspendThread((HANDLE)t->id);
	t->busy = 1;
#else
	pthread_mutex_lock(&t->mutex);
	t->busy = 0;
	pthread_cond_wait(&t->cond, &t->mutex);
	t->busy = 1;
	pthread_mutex_unlock(&t->mutex);
#endif

	if (t->running)
		t->f((void*)t->data);

	t->f = NULL;
	t->data = NULL;
	t->busy = 0;
}

static int thread_resume(thread t, void * (*f)(void*), void *data)
{
#ifdef _WIN32
	ResumeThread((HANDLE)t->id);
#else
	pthread_mutex_lock(&t->mutex);
	pthread_cond_signal(&t->cond);
	pthread_mutex_unlock(&t->mutex);
#endif
	return 1;
}

static void *start_routine(void *data)
{
	thread t = (thread)data;

	while (t->running)
		thread_pause(t);

	return 0;
}

static void *thread_start(thread t)
{
	t->running = 1;
	t->busy = 1;

#ifdef _WIN32
	{
		typedef unsigned (_stdcall *start_routine_t)(void*);
		SECURITY_ATTRIBUTES sa = {0};
		sa.nLength = sizeof (sa);
		sa.lpSecurityDescriptor = 0;
		sa.bInheritHandle = 0;
		t->id = (void*)_beginthreadex(&sa, 0, (start_routine_t)start_routine, (LPVOID)t, 0, NULL);
		return (void*)t->id;
	}
#else
	{
		typedef void * (*start_routine_t)(void*);
		pthread_attr_t sa;
		pthread_attr_init(&sa);
		pthread_attr_setdetachstate(&sa, PTHREAD_CREATE_DETACHED);
		pthread_t handle;
		int status = pthread_create(&handle, &sa, (start_routine_t)start_routine, t);
		t->id = (void*)handle;
		return (void*)(size_t)!status;
	}
#endif
}

static thread thread_create()
{
	thread t = (thread)calloc(1, sizeof(struct _thread));
	if (!t) return NULL;

#ifndef _WIN32
	pthread_cond_init(&t->cond, NULL);
	pthread_mutex_init(&t->mutex, NULL);
#endif
	return t;
}

static void thread_destroy(thread t)
{
#ifndef _WIN32
	pthread_cond_destroy(&t->cond);
	pthread_mutex_destroy(&t->mutex);
#endif
	free(t);
}

int thread_run(void * (*f)(void*), void *data)
{
#ifdef _WIN32
	typedef unsigned(_stdcall *start_routine_t)(void*);
	SECURITY_ATTRIBUTES sa = {0};
	int id;
	sa.nLength = sizeof (sa);
	sa.lpSecurityDescriptor = 0;
	sa.bInheritHandle = 0;
	id = _beginthreadex(&sa, 0, (start_routine_t)f, (LPVOID)data, 0, NULL);
	return id != 0;
#else
	typedef void *(*start_routine_t)(void*);
	pthread_attr_t sa;
	pthread_attr_init(&sa);
	pthread_attr_setdetachstate(&sa, PTHREAD_CREATE_DETACHED);
	pthread_t handle;
	int status = pthread_create(&handle, &sa, (start_routine_t)f, data);
	return !status;
#endif
}

thread_pool *tpool_create(int threads)
{
	thread_pool *tp = (thread_pool*)calloc(1, sizeof(struct thread_pool_));
	int i;

	if (threads > MAX_THREADS)
		threads = MAX_THREADS;

	while (threads-- > 0)
	{
		thread t = thread_create();
		t->cnt = tp->cnt;
		if (!t) break;
		tp->threads[tp->cnt++] = t;
		thread_start(t);
	}

	// wait thread to start
	for (i = 0; i < tp->cnt;) {
		if (tp->threads[i]->busy) {
			msleep(1);
			continue;
		}
		++i;
	}

	return tp;
}

int tpool_start(thread_pool *tp, void * (*f)(void*), void *data)
{
	int i;

	if (!tp || !f)
		return 0;


	for (i = 0; i < tp->cnt; i++)
	{
		thread t = tp->threads[tp->last++%tp->cnt];

		if (t->busy)
			continue;

		t->f = f;
		t->data = data;
		thread_resume(t, f, data);
		return 1;
	}

	f(data);
	return 1;
}

void tpool_destroy(thread_pool *tp)
{
	int i;

	if (!tp)
		return;

	for (i = 0; i < tp->cnt; i++)
	{
		thread t = tp->threads[i];
		int cnt = 0;
		t->running = 0;

		while (t->busy && (cnt++ < (1000*5))) {
			msleep(1);
		}

		thread_resume(t, NULL, NULL);
		msleep(1);
		thread_destroy(t);
	}

	free(tp);
}

#ifdef __cplusplus
}
#endif