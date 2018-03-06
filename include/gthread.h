/*
***************************************************************************
* This File is a part of OpenGCL.
* Copyright (c) 2004 Soo-Hyuk Nam (shnam7@gmail.com)
* 
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU Library General Public License
* as published by the Free Software Foundation: either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Library General Public License for more details.
*
* Should this software be used in another application, an acknowledgement
* that OpenGCL code is used would be appreciated, but it is not mandatory.
*
***************************************************************************
*/

/*
 *	* gthread.h
 *
 *	OpenGCL Module : gthread - POSIX pthread compatables and C++ wrappers
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
 *		- The macro variable, _GNU_SOURCE should be defined to enable
 *		  all the features of gthread.
 *		  Refer to /usr/include/features.h.
 */

#ifndef __GTHREAD_H
#define __GTHREAD_H

#if defined( _WIN32 )	/* win32 port */

/*--------------------------------------------------------------------
 * WIN32 port
 *--------------------------------------------------------------------*/

#if defined(_DLL)	/* win32 DLL linkages */
#ifdef GCL_EXPORTS
#define GCLAPI		__declspec(dllexport)
#else
#define GCLAPI		__declspec(dllimport)
#endif
#else
#define GCLAPI
#endif

#else	/* unix port */

#if 0 /*defined(__GNUC__) && !defined(_GNU_SOURCE) */
#define _GNU_SOURCE			/* enables whole pthread features */
#endif

#include <pthread.h>

#define GCLAPI

#endif


#ifdef __cplusplus
extern "C" {
#endif


#if defined( _WIN32 )
/*--------------------------------------------------------------------
 * non-WIN32 port
 *--------------------------------------------------------------------*/

#define GTHREAD_CANCELED			((void *) -1)
#define GTHREAD_INVALID_KEY			((gthread_key_t)TLS_OUT_OF_INDEXES)
#define GTHREAD_MUTEX_FAST			1
#define GTHREAD_MUTEX_RECURSIVE		2
#define GTHREAD_CREATE_JOINABLE		0
#define GTHREAD_CREATE_DETACHED		1

typedef	unsigned long				gthread_t;

typedef struct {
	int			__detachstate;
	size_t		__stacksize;
} gthread_attr_t;

typedef	unsigned long				gthread_key_t;

typedef struct {
	int				__mutexkind;
	void			*h_mutex;
} gthread_mutex_t;

typedef struct {
	int		__mutexkind;
} gthread_mutexattr_t;

typedef struct {
	void			*h_mutex;	/* internal lock */
	void			*h_event;	/* wakeup signal */
	unsigned int	nr_sleepers;
		/* number of thread waiting for cv */

	unsigned int	nr_wakers;
		/* number of thread signaled to be woken up */
} gthread_cond_t;

typedef struct {
	int		dummy;
} gthread_condattr_t;

typedef struct {
	void			*h_wrlock;
	void			*h_rdlock;
	long			readers;
} gthread_rwlock_t;

typedef struct {
	int				dummy;
} gthread_rwlockattr_t;

/* cleanup buffers */
struct _gthread_cleanup_buffer {
	void	(*__routine)(void *);
	void	*__arg;
	int		__canceltype;
	struct _gthread_cleanup_buffer	*__prev;
};

#else	/* unix port */

/*--------------------------------------------------------------------
 * unix port
 *--------------------------------------------------------------------*/

#define GTHREAD_CANCELED			PTHREAD_CANCELED
#define GTHREAD_INVALID_KEY			((gthread_key_t)(-1))
#define GTHREAD_MUTEX_FAST			PTHREAD_MUTEX_FAST_NP
#define GTHREAD_MUTEX_RECURSIVE		PTHREAD_MUTEX_RECURSIVE_NP
#define GTHREAD_CREATE_JOINABLE		PTHREAD_CREATE_JOINABLE
#define GTHREAD_CREATE_DETACHED		PTHREAD_CREATE_DETACHED

typedef pthread_t				gthread_t;
typedef pthread_attr_t			gthread_attr_t;
typedef pthread_key_t			gthread_key_t;
typedef pthread_mutex_t			gthread_mutex_t;
typedef pthread_mutexattr_t		gthread_mutexattr_t;

typedef pthread_cond_t			gthread_cond_t;
typedef pthread_condattr_t		gthread_condattr_t;
#ifdef __USE_UNIX98
typedef pthread_rwlock_t		gthread_rwlock_t;
typedef pthread_rwlockattr_t	gthread_rwlockattr_t;
#endif

#endif	/* unix port */


/*--------------------------------------------------------------------
 * architecture independents
 *--------------------------------------------------------------------*/
#define	GTHREAD_NULL				((gthread_t)0)

typedef void *(*gthread_func_t)(void *data);


GCLAPI int gthread_create(gthread_t *th, gthread_attr_t *attr,
			gthread_func_t func, void *data);

GCLAPI void gthread_exit(void *retval);

GCLAPI int gthread_join(gthread_t gt, void **return_val);
	
GCLAPI int gthread_cancel(gthread_t th);

GCLAPI void gthread_testcancel();

GCLAPI void gthread_detach(gthread_t th);

/* thread private storage */
GCLAPI int gthread_key_create(gthread_key_t *key,
							  void (*destr_func)(void *) );

GCLAPI int gthread_key_delete(gthread_key_t key);

GCLAPI int gthread_setspecific(gthread_key_t key, void *ptr);

GCLAPI void *gthread_getspecific(gthread_key_t key);

GCLAPI gthread_t gthread_self();

/* gthread attributes */
GCLAPI int gthread_attr_init(gthread_attr_t *attr);

GCLAPI int gthread_attr_destroy(gthread_attr_t *attr);

GCLAPI int gthread_attr_setdetachstate(gthread_attr_t *attr,
									   int _detachstate);

GCLAPI int gthread_attr_getdetachstate(gthread_attr_t *attr,
									   int *_detachstate);

GCLAPI int gthread_attr_setstacksize(gthread_attr_t *attr,
									 size_t stacksize);

GCLAPI int gthread_attr_getstacksize(gthread_attr_t *attr,
									 size_t *stacksize);

/* gthread cleanup */
#if defined( _WIN32 )

#define gthread_cleanup_push(routine,arg) \
	{ struct _gthread_cleanup_buffer __buffer_; \
		_gthread_cleanup_push(&__buffer_, (routine), (arg));

GCLAPI void _gthread_cleanup_push(struct _gthread_cleanup_buffer *buf,
						   void (*routine)(void *), void *arg);

#define gthread_cleanup_pop(execute) \
    _gthread_cleanup_pop(&__buffer_, (execute)); }

GCLAPI void _gthread_cleanup_pop(struct _gthread_cleanup_buffer *buf, int execute);

#else

#define gthread_cleanup_push(routine,arg) pthread_cleanup_push(routine, arg)
#define gthread_cleanup_pop(execute) pthread_cleanup_pop(execute)

#endif


/* gthread_yield: GNU extension - not POSIX standard.
 *
 * Win32 implementation uses Sleep() and this may take
 * fairly long time (about 10-20 msec).
 * The return value is always zero.
 *
 * GNU implementation uses sched_yield().
 * Refer to man relevant pages for details.
 */
#if defined( _WIN32 ) || defined( __USE_GNU )
GCLAPI int gthread_yield();
#endif


/* mutex */
GCLAPI int gthread_mutex_init(gthread_mutex_t *mtx,
							  const gthread_mutexattr_t *attr);

GCLAPI int gthread_mutex_destroy(gthread_mutex_t *mtx);

GCLAPI int gthread_mutex_trylock(gthread_mutex_t *mtx);

GCLAPI int gthread_mutex_lock(gthread_mutex_t *mtx);

GCLAPI int gthread_mutex_unlock(gthread_mutex_t *mtx);

GCLAPI int gthread_mutexattr_init(gthread_mutexattr_t *attr);

GCLAPI int gthread_mutexattr_destroy(gthread_mutexattr_t *attr);

GCLAPI int gthread_mutexattr_settype(gthread_mutexattr_t *attr, int kind);

GCLAPI int gthread_mutexattr_gettype(gthread_mutexattr_t *attr, int *kind);

/* sem */

/* pthread once */

/* cond */
	/* attr is not supported yat. It always should be zero. */
GCLAPI int gthread_cond_init(gthread_cond_t *cond,
							 gthread_condattr_t *attr);

GCLAPI int gthread_cond_destroy(gthread_cond_t *cond);

GCLAPI int gthread_cond_signal(gthread_cond_t *cond);

GCLAPI int gthread_cond_broadcast(gthread_cond_t *cond);

GCLAPI int gthread_cond_wait(gthread_cond_t *cond,
							 gthread_mutex_t *mtx);

GCLAPI int gthread_cond_timedwait(gthread_cond_t *cond,
							 gthread_mutex_t *mtx,
							 unsigned long timeout_msec);

/* rwlock */
#if defined(_WIN32) || defined(__USE_UNIX98)
GCLAPI int gthread_rwlock_init(gthread_rwlock_t *rwlock,
							   gthread_rwlockattr_t *attr);

GCLAPI int gthread_rwlock_destroy(gthread_rwlock_t *rwlock);

GCLAPI int gthread_rwlock_rdlock(gthread_rwlock_t *rwlock);

GCLAPI int gthread_rwlock_tryrdlock(gthread_rwlock_t *rwlock);

GCLAPI int gthread_rwlock_wrlock(gthread_rwlock_t *rwlock);

GCLAPI int gthread_rwlock_trywrlock(gthread_rwlock_t *rwlock);

GCLAPI int gthread_rwlock_unlock(gthread_rwlock_t *rwlock);
#endif


/* spin */

/*	barrier
	Software objects used to ensure that all threads in a
	group are ready before any of them proceed.

	--> not supported
*/

/* cancel state */

/* atfork */

/* non-POSIX extensions */
/*
GCLAPI int gthread_setobject(void *obj);

GCLAPI void *gthread_getobject();
*/

unsigned long gthread_get_oshandle(gthread_t th);


#ifdef __cplusplus
};

//--------------------------------------------------------------------
//	class GRunnable
//--------------------------------------------------------------------
class GCLAPI GRunnable {
	friend class GThread;

protected:
	GRunnable() {}
	virtual ~GRunnable() {}
	virtual void *run() { return (void *)0; }

	// NOTE: these functions applies for gthread_self()
	//   and should not be called by other thread.
	void testCancel() { gthread_testcancel(); }
};


//--------------------------------------------------------------------
//	class GThread
//--------------------------------------------------------------------
class GCLAPI GThread : public GRunnable {
protected:
	gthread_t		m_t;

	// Not-allowed operations
	GThread(gthread_t th);
	GThread(GThread& th);
	GThread& operator=(GThread& th);
	GThread& operator=(gthread_t th);

public:
	GThread();
	~GThread();

	//--- manipulators
	int start() { return start(this); }
	int start(GRunnable *pRunnable);
	int start(gthread_func_t func, void *data=0);

	int join(void **thread_return=0) const;
	int stop();
	void cancel() { gthread_cancel(m_t); }

	//--- accessors
	bool isRunning() const { return m_t != GTHREAD_NULL; }
	gthread_t getThreadID() const { return m_t; }

	// Returns OS dependent handle.
	// On win32 platforms, it returns win32 handle to thread.
	unsigned long getOSHandle() const { return gthread_get_oshandle(m_t); }

protected:
	int __start(struct __start_param *pa);
	static void *_thread_wrapper(void *);
	static void _thread_cleanup(void *);

public:
	// static GThread *getCurrent() { return (GThread *)gthread_getobject(); }
	static GThread *getCurrent();
};



//--------------------------------------------------------------------
//	class GMutex - wrapper for gthread_mutex_t
//--------------------------------------------------------------------
class GCLAPI GMutex {
protected:
	gthread_mutex_t		m_mtx;

public:
	GMutex() { gthread_mutex_init( &m_mtx, 0 ); }
	GMutex(int kind);
	~GMutex() { gthread_mutex_destroy( &m_mtx ); }

	bool lock() { return gthread_mutex_lock( &m_mtx ) == 0; }
	bool unlock() { return gthread_mutex_unlock( &m_mtx ) == 0; }
	bool trylock() { return gthread_mutex_trylock( &m_mtx ) == 0; }
	gthread_mutex_t *getMutex() { return &m_mtx; }
};


//--------------------------------------------------------------------
//	class GCondition - wrapper for pthread_cond_t
//--------------------------------------------------------------------
class GCondition {
protected:
	gthread_cond_t		m_cond;

public:
	GCondition() { gthread_cond_init(&m_cond, 0); }

	~GCondition() { gthread_cond_destroy(&m_cond); }

	bool signal() { return gthread_cond_signal(&m_cond)==0; }

	bool broadcast() { return gthread_cond_broadcast(&m_cond)==0; }

	bool wait(gthread_mutex_t *mtx)
		{ return gthread_cond_wait(&m_cond, mtx)==0; }

	bool wait(GMutex& mtx)
		{ return gthread_cond_wait(&m_cond, mtx.getMutex())==0; }

	bool timedwait(gthread_mutex_t *mtx, unsigned long timeout_msec)
		{ return gthread_cond_timedwait(&m_cond, mtx, timeout_msec)==0; }

	bool timedwait(GMutex& mtx, unsigned long timeout_msec)
		{ return timedwait(mtx.getMutex(), timeout_msec); }

	gthread_cond_t *getCond() { return &m_cond; }
};


//--------------------------------------------------------------------
//	class GRWLock - wrapper for gthread_rwlock_t
//--------------------------------------------------------------------
#if defined(_WIN32) || defined(__USE_UNIX98)
class GRWLock {
protected:
	gthread_rwlock_t		m_rwlock;

public:
	GRWLock() { gthread_rwlock_init(&m_rwlock, 0); }

	~GRWLock() { gthread_rwlock_destroy(&m_rwlock); }

	int rdlock() { return gthread_rwlock_rdlock(&m_rwlock); }
	
	int tryrdlock() { return gthread_rwlock_tryrdlock(&m_rwlock); }

	int wrlock() { return gthread_rwlock_wrlock(&m_rwlock); }

	int trywrlock() { return gthread_rwlock_trywrlock(&m_rwlock); }

	int unlock() { return gthread_rwlock_unlock(&m_rwlock); }
};
#endif

#endif	/* __cplusplus */

#endif
