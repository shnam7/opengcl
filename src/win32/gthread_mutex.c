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
 *	* gthread_mutex.c
 *
 *	OpenGCL Module : gthread - WIN32 POSIX compatable mutex support.
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


GCLAPI int gthread_mutex_init(gthread_mutex_t *mtx,
							  const gthread_mutexattr_t *attr)
{
	mtx->__mutexkind = (attr) ? attr->__mutexkind : GTHREAD_MUTEX_FAST;
	if ( mtx->__mutexkind == GTHREAD_MUTEX_RECURSIVE )
		mtx->h_mutex = CreateMutex(0, 0, 0);
	else
		mtx->h_mutex = CreateEvent(0, 0, 1, 0);	/* simple mutex using event */
	if ( mtx->h_mutex == 0 ) return GetLastError();
	return 0;
}

GCLAPI int gthread_mutex_destroy(gthread_mutex_t *mtx)
{
	if ( !CloseHandle(mtx->h_mutex) ) return GetLastError();
	return 0;
}

GCLAPI int gthread_mutex_trylock(gthread_mutex_t *mtx)
{
	int r = WaitForSingleObject(mtx->h_mutex, 0);
	if ( r!=WAIT_OBJECT_0 || r!=WAIT_ABANDONED )
	{
		return ( r == WAIT_TIMEOUT ) ? EBUSY : GetLastError();
	}
	return 0;
}

GCLAPI int gthread_mutex_lock(gthread_mutex_t *mtx)
{
	int r = WaitForSingleObject(mtx->h_mutex, INFINITE);
	if ( r!=WAIT_OBJECT_0 || r!=WAIT_ABANDONED )
	{
		return ( r == WAIT_TIMEOUT ) ? EBUSY : GetLastError();
	}
	return 0;
}

GCLAPI int gthread_mutex_unlock(gthread_mutex_t *mtx)
{
	if ( mtx->__mutexkind == GTHREAD_MUTEX_RECURSIVE )
	{
		if ( !ReleaseMutex(mtx->h_mutex) ) return GetLastError();
	}
	else
	{
		if ( !SetEvent(mtx->h_mutex) ) return GetLastError();
	}
	return 0;
}

GCLAPI int gthread_mutexattr_init(gthread_mutexattr_t *attr)
{
	attr->__mutexkind = GTHREAD_MUTEX_FAST;
	return 0;
}

GCLAPI int gthread_mutexattr_destroy(gthread_mutexattr_t *attr)
{
	return 0;
}

GCLAPI int gthread_mutexattr_settype(gthread_mutexattr_t *attr, int kind)
{
	if ( kind!=GTHREAD_MUTEX_FAST && kind!=GTHREAD_MUTEX_RECURSIVE )
		return ERROR_INVALID_DATA;
	attr->__mutexkind = kind;
	return 0;
}


GCLAPI int gthread_mutexattr_gettype(gthread_mutexattr_t *attr, int *kind)
{
	*kind = attr->__mutexkind;
	return 0;
}