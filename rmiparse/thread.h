#ifndef THREAD_H
#define THREAD_H

/*#include <stdint.h>*/

#ifdef _WIN32
#define msleep Sleep
#else
#define msleep(ms) usleep(ms*1000)
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct lock_ lock;
typedef struct thread_pool_ thread_pool;

extern lock *lock_create(void);
extern void lock_lock(lock *l);
extern void lock_unlock(lock *l);
extern void lock_destroy(lock *l);

extern int atomic_inc(int *v);			// return pre-value
extern int atomic_dec(int *v);			// return post-value

extern int atomic_dec_and_zero(int *v, int *v2);

/*extern int64_t atomic_add64(int64_t *v, int n);		// return pre-value*/
/*extern uint64_t atomic_addu64(uint64_t *v, int n);	// return pre-value*/

// Run a supplied function as a one-off detached thread. The
// thread will be destroyed after this single use.

extern int thread_run(void * (*f)(void*), void *data);

// Run a supplied function from a pool of threads. The thread
// will be returned to the pool for fast re-use when needed.

extern thread_pool *tpool_create(int threads);
extern int tpool_start(thread_pool *tp, void * (*f)(void*), void *data);
extern void tpool_destroy(thread_pool *tp);

#ifdef __cplusplus
}
#endif

#endif
