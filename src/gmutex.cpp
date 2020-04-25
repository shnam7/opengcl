#include "gthread.h"

//--------------------------------------------------------------------
//	class GMutex - wrapper for gthread mutex
//--------------------------------------------------------------------
gcl_api gcl::gmutex::gmutex(int kind)
{
	gthread_mutexattr_t attr;
	gthread_mutexattr_init( &attr );
	gthread_mutexattr_settype( &attr, kind );
	gthread_mutex_init( &m_mtx, &attr );
	gthread_mutexattr_destroy( &attr );
}
