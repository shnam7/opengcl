/*
 *  @packaage OpenGCL
 *
 *  @module cond - POSIX pthread_cond implementation for windows
 *
 *	Written by Soo-Hyuk Nam.
 *		2004. 1. 15. Thu.
 *
 *	History:
 *		2004/01/15: Originally written.
 */

#pragma once
#include "gcldef.h"
#include "gthread_mutex.h"

#if defined (_WIN32)
typedef struct {
	void            *h_mutex;	    // internal lock
	void            *h_event;	    // wakeup signal
	unsigned int    nr_sleepers;    // number of thread waiting for cv
	unsigned int	nr_wakers;      // number of thread signaled to be woken up
} gthread_cond_t;
typedef struct { int dummy; } gthread_condattr_t;

// cond: attr is not supported yat. It always should be zero.
gcl_api int gthread_cond_init(gthread_cond_t *cond, gthread_condattr_t *attr);
gcl_api int gthread_cond_destroy(gthread_cond_t *cond);
gcl_api int gthread_cond_signal(gthread_cond_t *cond);
gcl_api int gthread_cond_broadcast(gthread_cond_t *cond);
gcl_api int gthread_cond_wait(gthread_cond_t *cond, gthread_mutex_t *mtx);
gcl_api int gthread_cond_timedwait(gthread_cond_t *cond, gthread_mutex_t *mtx, unsigned long timeout_msec);

// #endif


#else   // unix port: use pthread
typedef pthread_cond_t			gthread_cond_t;
typedef pthread_condattr_t		gthread_condattr_t;

/* attr is not supported yat. It always should be zero.
 * Although pthread_cond_init supports attr,
 * gthread interface doesn't. Using it may cause
 * portability problem.
 */
inline int gthread_cond_init(gthread_cond_t *cond, gthread_condattr_t *attr) { return pthread_cond_init(cond, attr); }

inline int gthread_cond_destroy(gthread_cond_t *cond) { return pthread_cond_destroy(cond); }

inline int gthread_cond_signal(gthread_cond_t *cond) { return pthread_cond_signal(cond); }

inline int gthread_cond_broadcast(gthread_cond_t *cond) { return pthread_cond_broadcast(cond); }

inline int gthread_cond_wait(gthread_cond_t *cond, gthread_mutex_t *mtx) { return pthread_cond_wait(cond, mtx); }

/* gthread used do not use 'struct timespec' for compatability reason. */
gcl_api int gthread_cond_timedwait(gthread_cond_t *cond, gthread_mutex_t *mtx, unsigned long timeout_msec);

#endif
