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
 *	* gthread_rwlock.c
 *
 *	OpenGCL Module : gthread - WIN32 POSIX compatable rwlock support.
 *
 *	Written by Soo-Hyuk Nam.
 *		2004. 1. 15. Thu.
 *
 *	History:
 *		2004/01/15: Originally written.
 */

#include "gthread.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <errno.h>


static int _gthread_rwlock_rdlock(gthread_rwlock_t *rwlock,
								  unsigned long timeout)
{
	/* wait until write finished, if any */
	DWORD r = WaitForSingleObject(rwlock->h_wrlock, timeout);
	if ( r == WAIT_TIMEOUT ) return ERROR_TIMEOUT;
	if ( r == WAIT_FAILED ) return GetLastError();

	InterlockedIncrement( &rwlock->readers );
	if ( !ResetEvent(rwlock->h_rdlock) )	/* get read lock: block writers */
		return GetLastError();

	if ( !ReleaseMutex(rwlock->h_wrlock) )
		return GetLastError();
	return 0;
}

static int _gthread_rwlock_wrlock(gthread_rwlock_t *rwlock,
								  unsigned long timeout)
{
	/* first, get write lock: block all the readers and writers */
	DWORD r = WaitForSingleObject(rwlock->h_wrlock, timeout);
	if ( r == WAIT_TIMEOUT ) return ERROR_TIMEOUT;
	if ( r == WAIT_FAILED ) return GetLastError();

	/* next, check if there is any readers */
	if ( rwlock->readers > 0 )
	{
		if ( timeout == 0 )
		{
			if ( !ReleaseMutex(rwlock->h_rdlock) )
				return GetLastError();
			return ERROR_TIMEOUT;
		}

		/* now, we must wait until all the reader to finish */
		r = WaitForSingleObject(rwlock->h_rdlock, timeout);
		if ( r == WAIT_TIMEOUT )
		{
			ReleaseMutex( rwlock->h_wrlock );
			return ERROR_TIMEOUT;
		}
		if ( r == WAIT_FAILED )
		{
			ReleaseMutex( rwlock->h_wrlock );
			return GetLastError();
		}
	}
	return 0;
}

GCLAPI int gthread_rwlock_init(gthread_rwlock_t *rwlock,
							   gthread_rwlockattr_t *attr)
{
	if ( attr != 0 ) return ERROR_INVALID_DATA;

	rwlock->h_rdlock = CreateEvent(0, 1, 0, 0);
	if ( !rwlock->h_rdlock ) return GetLastError();

	rwlock->h_wrlock = CreateMutex(0, 0, 0);
	if ( !rwlock->h_wrlock )
	{
		CloseHandle( rwlock->h_rdlock );
		return GetLastError();
	}
	rwlock->readers = 0;
	return 0;
}

GCLAPI int gthread_rwlock_destroy(gthread_rwlock_t *rwlock)
{
	if ( !CloseHandle(rwlock->h_wrlock) )
		return GetLastError();
	if ( !CloseHandle(rwlock->h_rdlock) )
		return GetLastError();
	return 0;
}

GCLAPI int gthread_rwlock_rdlock(gthread_rwlock_t *rwlock)
{
	return _gthread_rwlock_rdlock(rwlock, INFINITE);
}

GCLAPI int gthread_rwlock_tryrdlock(gthread_rwlock_t *rwlock)
{
	return _gthread_rwlock_rdlock(rwlock, 0);
}

GCLAPI int gthread_rwlock_wrlock(gthread_rwlock_t *rwlock)
{
	return _gthread_rwlock_wrlock(rwlock, INFINITE);
}

GCLAPI int gthread_rwlock_trywrlock(gthread_rwlock_t *rwlock)
{
	return _gthread_rwlock_wrlock(rwlock, 0);
}

GCLAPI int gthread_rwlock_unlock(gthread_rwlock_t *rwlock)
{
	/* try to unlock write lock */
	DWORD r = 0;
	if ( !ReleaseMutex(rwlock->h_wrlock) ) r = GetLastError();

	if ( r == ERROR_NOT_OWNER ) /* Maybe, we have read lock */
	{
		if ( rwlock->readers == 0 ) return 0;	/* we do not have read lock */
		InterlockedDecrement(&rwlock->readers);

		/* release read lock if there are no readers */
		if ( rwlock->readers==0 && !SetEvent(rwlock->h_rdlock) )
			return GetLastError();
	}
	return r;
}
