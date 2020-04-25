/*
 *  @packaage OpenGCL
 *
 *  @module gsem - POSIX semaphore for inter-process locking
 *  *
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

#include "gsem.h"

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define GSEM_VALUE_MAX		((int)((~0u) >> 1))

static int _gsem_wait(gsem_t *sem, unsigned long timeout)
{
	DWORD r = WaitForSingleObject(sem->h_sem, timeout);
	if ( r == WAIT_TIMEOUT ) return ERROR_TIMEOUT;
	if ( r == WAIT_FAILED ) return GetLastError();
	InterlockedDecrement( &sem->value );
	if ( sem->value>0 && !SetEvent(sem->h_sem) )
		return GetLastError();
	return 0;
}

gcl_api int gsem_init(gsem_t *sem, int pshared, unsigned int val)
{
	if ( val > GSEM_VALUE_MAX ) val = GSEM_VALUE_MAX;
	sem->value = val;
	sem->h_sem = CreateEvent( 0, 0, (val>0), 0 );
	if ( !sem->h_sem ) GetLastError();
	return 0;
}

gcl_api int gsem_destroy(gsem_t *sem)
{
	if ( !CloseHandle(sem->h_sem) ) return GetLastError();
	return 0;
}

gcl_api int gsem_wait(gsem_t *sem)
{
	return _gsem_wait(sem, INFINITE);
}

gcl_api int gsem_trywait(gsem_t *sem)
{
	return _gsem_wait(sem, 0);
}

gcl_api int gsem_post(gsem_t *sem)
{
	if ( sem->value > GSEM_VALUE_MAX ) return ERROR_ARITHMETIC_OVERFLOW;
	InterlockedIncrement( &sem->value );
	if ( !SetEvent(sem->h_sem) ) return GetLastError();
	return 0;
}

gcl_api int gsem_getvalue(gsem_t *sem, int *pval)
{
	*pval = sem->value;
	return 0;
}

#endif
