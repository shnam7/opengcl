/*
 *	* gthread.c
 *
 *	OpenGCL Module : gthread - POSIX pthread compatables and C++ wrappers
 *	                 Linux/POSIX port implementation
 *
 *	Written by Soo-Hyuk Nam.
 *		2004. 1. 15. Thu.
 *
 *	History:
 *		2004/01/15: Originally written.
 *
 *	Notes:
 *		- All the functions returns 0 on success, nonzero code on error,
 *		  unless specified otherwise.
 */

#define _GNU_SOURCE			/* enables whole pthread features */

#include "gthread.h"
#include <sys/time.h>


GCLAPI int gthread_create(gthread_t *th, gthread_attr_t *attr,
			gthread_func_t func, void *data)
{
	return pthread_create(th, attr, func, data);
}

GCLAPI void gthread_exit(void *retval)
{
	pthread_exit(retval);
}

GCLAPI int gthread_join(gthread_t th, void **return_val)
{
	return pthread_join(th, return_val);
}

GCLAPI int gthread_cancel(gthread_t th)
{
	return pthread_cancel(th);
}

GCLAPI void gthread_testcancel()
{
	return pthread_testcancel();
}

GCLAPI void gthread_detach(gthread_t th)
{
	pthread_detach(th);
}


/* thread private storage */
GCLAPI int gthread_key_create(gthread_key_t *key,
							  void (*destr_func)(void *) )
{
	return pthread_key_create(key, destr_func);
}

GCLAPI int gthread_key_delete(gthread_key_t key)
{
	return pthread_key_delete(key);
}

GCLAPI int gthread_setspecific(gthread_key_t key, void *ptr)
{
	return pthread_setspecific(key, ptr);
}

GCLAPI void *gthread_getspecific(gthread_key_t key)
{
	return pthread_getspecific(key);
}

GCLAPI gthread_t gthread_self()
{
	return pthread_self();
}


/* gthread attributes */
GCLAPI int gthread_attr_init(gthread_attr_t *attr)
{
	return pthread_attr_init(attr);
}

GCLAPI int gthread_attr_destroy(gthread_attr_t *attr)
{
	return pthread_attr_destroy(attr);
}

GCLAPI int gthread_attr_setdetachstate(gthread_attr_t *attr,
									   int _detachstate)
{
	return pthread_attr_setdetachstate(attr, _detachstate);
}

GCLAPI int gthread_attr_getdetachstate(gthread_attr_t *attr,
									   int *_detachstate)
{
	return pthread_attr_getdetachstate(attr, _detachstate);
}

GCLAPI int gthread_attr_setstacksize(gthread_attr_t *attr,
									 size_t stacksize)
{
	return pthread_attr_setstacksize(attr, stacksize);
}

GCLAPI int gthread_attr_getstacksize(gthread_attr_t *attr,
									 size_t *stacksize)
{
	return pthread_attr_getstacksize(attr, stacksize);
}

#ifdef __USE_GNU
/* GNU specific - not POSIX standard
 * It returns always zero.
 */
GCLAPI int gthread_yield()
{
	return pthread_yield();
}
#endif


/* mutex */
GCLAPI int gthread_mutex_init(gthread_mutex_t *mtx,
							  const gthread_mutexattr_t *attr)
{
	return pthread_mutex_init(mtx, attr);
}

GCLAPI int gthread_mutex_destroy(gthread_mutex_t *mtx)
{
	return pthread_mutex_destroy(mtx);
}

GCLAPI int gthread_mutex_trylock(gthread_mutex_t *mtx)
{
	return pthread_mutex_trylock(mtx);
}

GCLAPI int gthread_mutex_lock(gthread_mutex_t *mtx)
{
	return pthread_mutex_lock(mtx);
}

GCLAPI int gthread_mutex_unlock(gthread_mutex_t *mtx)
{
	return pthread_mutex_unlock(mtx);
}

GCLAPI int gthread_mutexattr_init(gthread_mutexattr_t *attr)
{
	return pthread_mutexattr_init(attr);
}

GCLAPI int gthread_mutexattr_destroy(gthread_mutexattr_t *attr)
{
	return pthread_mutexattr_destroy(attr);
}

GCLAPI int gthread_mutexattr_settype(gthread_mutexattr_t *attr,
											int kind)
{
	return pthread_mutexattr_settype(attr, kind);
}

GCLAPI int gthread_mutexattr_gettype(gthread_mutexattr_t *attr,
											int *kind)
{
	return pthread_mutexattr_gettype(attr, kind);
}


/* pthread once */


/* cond */

/* attr is not supported yat. It always should be zero.
 * Although pthread_cond_init supports attr,
 * gthread interface doesn't. Using it may cause
 * portability problem.
 */
GCLAPI int gthread_cond_init(gthread_cond_t *cond,
							 gthread_condattr_t *attr)
{
	return pthread_cond_init(cond, attr);
}

GCLAPI int gthread_cond_destroy(gthread_cond_t *cond)
{
	return pthread_cond_destroy(cond);
}

GCLAPI int gthread_cond_signal(gthread_cond_t *cond)
{
	return pthread_cond_signal(cond);
}

GCLAPI int gthread_cond_broadcast(gthread_cond_t *cond)
{
	return pthread_cond_broadcast(cond);
}

GCLAPI int gthread_cond_wait(gthread_cond_t *cond,
							 gthread_mutex_t *mtx)
{
	return pthread_cond_wait(cond, mtx);
}

/*
 * gthread used do not use 'struct timespec' for compatability reason.
 */
GCLAPI int gthread_cond_timedwait(gthread_cond_t *cond,
							 gthread_mutex_t *mtx,
							 unsigned long timeout_msec)
{
	struct timespec tm;
	struct timeval	tv;

	gettimeofday( &tv, 0 );
	tm.tv_sec = tv.tv_sec + timeout_msec / 1000;
	tm.tv_nsec = tv.tv_usec*1000 + (timeout_msec%1000) * 1000000;
	if ( tm.tv_nsec > 1000000000 )
	{
		++tm.tv_sec;
		tm.tv_nsec -= 1000000000;
	}

	return pthread_cond_timedwait(cond, mtx, &tm);
}

/* rwlock */
// #ifdef __USE_UNIX98
GCLAPI int gthread_rwlock_init(gthread_rwlock_t *rwlock,
							   gthread_rwlockattr_t *attr)
{
	return pthread_rwlock_init(rwlock, attr);
}

GCLAPI int gthread_rwlock_destroy(gthread_rwlock_t *rwlock)
{
	return pthread_rwlock_destroy(rwlock);
}

GCLAPI int gthread_rwlock_rdlock(gthread_rwlock_t *rwlock)
{
	return pthread_rwlock_rdlock(rwlock);
}

GCLAPI int gthread_rwlock_tryrdlock(gthread_rwlock_t *rwlock)
{
	return pthread_rwlock_tryrdlock(rwlock);
}

GCLAPI int gthread_rwlock_wrlock(gthread_rwlock_t *rwlock)
{
	return pthread_rwlock_wrlock(rwlock);
}

GCLAPI int gthread_rwlock_trywrlock(gthread_rwlock_t *rwlock)
{
	return pthread_rwlock_trywrlock(rwlock);
}

GCLAPI int gthread_rwlock_unlock(gthread_rwlock_t *rwlock)
{
	return pthread_rwlock_unlock(rwlock);
}
// #endif

/* spin */

/*	barrier
	Software objects used to ensure that all threads in a
	group are ready before any of them proceed.

	--> not supported yat.
*/

/* cancel state */

/* atfork */

/* non-POSIX extensions */

GCLAPI uintptr_t gthread_get_oshandle(gthread_t th)
{
	return (uintptr_t)th;
}
