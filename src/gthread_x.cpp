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
 *	* gthread_x.cpp
 *
 *	OpenGCL Module : gthread - WIN32 C++ wrapper for POSIX pthread
 *
 *	Written by Soo-Hyuk Nam.
 *		2004. 1. 15. Thu.
 *
 *	History:
 *		2004/01/15: Originally written.
 */

#include "gthread.h"
#include "gsem.h"

#if defined( _WIN32 )
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <errno.h>
#endif

#if !defined( _WIN32 )
#define ERROR_INVALID_HANDLE	EINVAL
#endif

struct __start_param {
	GThread			*pThread;
	GRunnable		*pRunnable;	
	gthread_func_t	pFunc;
	void			*data;
	gsem_t			*sem;			/* parent --> thread */
	gsem_t			*sem_th;		/* thread --> parent */
};

static gthread_key_t	_gkey_object  = GTHREAD_INVALID_KEY;
static unsigned long	_gthread_object_count = 0;

static int _check_init()
{
	if ( _gthread_object_count != 0 ) return 0;
	int r = gthread_key_create( &_gkey_object, 0 );
	if ( r != 0 ) return r;
	++_gthread_object_count;
	return 0;
}

static int _check_cleanup()
{
	if ( _gthread_object_count == 0 ) return 0;
	int r = gthread_key_delete(_gkey_object);
	if ( r != 0 ) return r;
	_gkey_object = GTHREAD_INVALID_KEY;
	--_gthread_object_count;
	return 0;
}

static inline int gthread_setobject(void *obj)
{
	int r = _check_init();
	if ( r != 0 ) return r;
	return gthread_setspecific( _gkey_object, obj );
}

static inline void *gthread_getobject()
{
	return gthread_getspecific( _gkey_object );
}



//------------------------------------------------
//	class GThread Unix impl.
//------------------------------------------------
GThread::GThread()
	: m_t(GTHREAD_NULL)
{
	_check_init();
}

GThread::~GThread()
{
	stop();
}

int GThread::start(GRunnable *pRunnable)
{
	__start_param pa = { this, pRunnable, 0, 0, 0 };
	return __start( &pa );
}

int GThread::start(gthread_func_t func, void *data)
{
	struct __start_param pa = { this, 0, func, data, 0, 0 };
	return __start(&pa) != 0;
}

int GThread::join(void **thread_return) const
{
	if (m_t==GTHREAD_NULL || gthread_self()==m_t )
		return ERROR_INVALID_HANDLE;
	return gthread_join( m_t, thread_return );
}

int GThread::stop()
{
	if ( m_t == GTHREAD_NULL ) return ERROR_INVALID_HANDLE;
	if ( gthread_self() == m_t ) gthread_exit(0);
	int r = gthread_cancel(m_t);
	if ( r != 0 ) return r;
	return join(0);
}

int GThread::__start(struct __start_param *pa)
{
	if ( m_t != GTHREAD_NULL || gthread_self()==m_t )
		return ERROR_INVALID_HANDLE;

	int r = _check_init();
	if ( r != 0 ) return r;

	/* create temporary semaphore */
	gsem_t sem, sem_th;
	r = gsem_init(&sem, 0, 0);
	if ( r != 0 ) return r;
	r = gsem_init(&sem_th, 0, 0);
	if ( r != 0 )
	{
		gsem_destroy( &sem );
		_check_cleanup();
		return r;
	}

	pa->sem = &sem;
	pa->sem_th = &sem_th;
	gthread_t tid;
	r = gthread_create( &tid, 0, _thread_wrapper, pa );
	if ( r != 0 )
	{
		gsem_destroy( &sem );
		gsem_destroy( &sem_th );
		_check_cleanup();
		return r;
	}

	//--- wait until the thread notifies that it is ready.
	gsem_wait( &sem_th );

	m_t = tid;		// this assures isRunning() returns true
					// only after the thread actually has started.

	// notify the new thread that tid is set.
	gsem_post( &sem );

	// wait until the new thread ack to destroy semaphores
	// and parameter block
	gsem_wait( &sem_th);

	gsem_destroy( &sem );
	gsem_destroy( &sem_th );

	return 0;
}

void *GThread::_thread_wrapper(void *data)
{
	__start_param pa = *(__start_param *)data;
	gthread_setobject( pa.pThread );

	/* notify that the new thread is ready */
	gsem_post( pa.sem_th );

	/* wait until tid is set */
	gsem_wait( pa.sem );

	/* notify to destroy semaphores and parameter block */
	gsem_post( pa.sem_th );

	/* start running */
	void *retval;
	gthread_cleanup_push( _thread_cleanup, pa.pThread );
	if ( pa.pRunnable )
		retval = pa.pRunnable->run();
	else if ( pa.pFunc )
		retval = (*pa.pFunc)(pa.data);
	else
		retval = 0;
	gthread_cleanup_pop( 1 );
	_check_cleanup();
	gthread_exit( retval );
	return 0;
}

void GThread::_thread_cleanup(void *data)
{
	GThread *pT = (GThread *)data;
	pT->m_t = GTHREAD_NULL;
}

GThread *GThread::getCurrent()
{
	return (GThread *)gthread_getobject();
}


//--------------------------------------------------------------------
//	class GMutex - wrapper for gthread mutex
//--------------------------------------------------------------------
GMutex::GMutex(int kind)
{
	gthread_mutexattr_t attr;
	gthread_mutexattr_init( &attr );
	gthread_mutexattr_settype( &attr, kind );
	gthread_mutex_init( &m_mtx, &attr );
	gthread_mutexattr_destroy( &attr );
}
