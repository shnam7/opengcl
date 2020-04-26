#include "gthread.h"
#include "gsem.h"

#if defined(_WIN32)
#include <windows.h>
#else
#define ERROR_INVALID_HANDLE    EINVAL
#include <errno.h>
#endif

//--------------------------------------------------------------------
//	class thread
//--------------------------------------------------------------------

struct __start_param {
	gcl::gthread		*pThread;
	gcl::_runnable		*runnable;
	gthread_func_t	    pFunc;
	void			    *data;
	gsem_t			    *sem;			/* parent --> thread */
	gsem_t			    *sem_th;		/* thread --> parent */
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

inline int gthread_setobject(void *obj)
{
	int r = _check_init();
	if ( r != 0 ) return r;
	return gthread_setspecific( _gkey_object, obj );
}

inline void *gthread_getobject()
{
	return gthread_getspecific( _gkey_object );
}


gcl::gthread::gthread() : m_t(GTHREAD_NULL) { _check_init(); }

gcl::gthread::~gthread() { stop(); }

gcl_api int gcl::gthread::start(runnable *runnable)
{
	__start_param pa = { this, runnable, 0, 0, 0, 0 };
	return __start(&pa);
}

gcl_api int gcl::gthread::start(gthread_func_t func, void *data)
{
	struct __start_param pa = { this, 0, func, data, 0, 0 };
	return __start(&pa) != 0;
}

gcl_api int gcl::gthread::join(void **thread_return) const
{
    if (m_t == GTHREAD_NULL || gthread_self() == m_t) return ERROR_INVALID_HANDLE;
    return gthread_join(m_t, thread_return);
}

gcl_api int gcl::gthread::stop()
{
	if ( m_t == GTHREAD_NULL ) return ERROR_INVALID_HANDLE;
	// if ( gthread_self() == m_t ) gthread_exit(0);
	int r = gthread_cancel(m_t);
	if ( r != 0 ) return r;
	return join(0);
}

gcl_api int gcl::gthread::__start(void *pa)
{
    if (m_t != GTHREAD_NULL || gthread_self() == m_t) return ERROR_INVALID_HANDLE;

    int r = _check_init();
    if (r != 0) return r;

    /* create temporary semaphore */
    gsem_t sem, sem_th;
    r = gsem_init(&sem, 0, 0);
    if (r != 0) return r;
    r = gsem_init(&sem_th, 0, 0);
    if (r != 0) {
        gsem_destroy(&sem);
        _check_cleanup();
        return r;
	}

	((__start_param *)pa)->sem = &sem;
	((__start_param *)pa)->sem_th = &sem_th;
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

void *gcl::gthread::_thread_wrapper(void *data)
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
	if ( pa.runnable )
		retval = pa.runnable->run();
	else if ( pa.pFunc )
		retval = (*pa.pFunc)(pa.data);
	else
		retval = 0;
	gthread_cleanup_pop( 1 );
	_check_cleanup();
	gthread_exit( retval );
	return 0;
}

void gcl::gthread::_thread_cleanup(void *data)
{
	gcl::gthread *pT = (gcl::gthread *)data;
	pT->m_t = GTHREAD_NULL;
}

gcl::gthread *gcl::gthread::getCurrent()
{
	return (gcl::gthread *)gthread_getobject();
}
