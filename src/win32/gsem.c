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
 *	* gsem.c
 *
 *	OpenGCL Module : gsem - POSIX semaphore compatables and C++ wrappers
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

#include "gsem.h"
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

GCLAPI int gsem_init(gsem_t *sem, int pshared, unsigned int val)
{
	if ( val > GSEM_VALUE_MAX ) val = GSEM_VALUE_MAX;
	sem->value = val;
	sem->h_sem = CreateEvent( 0, 0, (val>0), 0 );
	if ( !sem->h_sem ) GetLastError();
	return 0;
}

GCLAPI int gsem_destroy(gsem_t *sem)
{
	if ( !CloseHandle(sem->h_sem) ) return GetLastError();
	return 0;
}

GCLAPI int gsem_wait(gsem_t *sem)
{
	return _gsem_wait(sem, INFINITE);
}

GCLAPI int gsem_trywait(gsem_t *sem)
{
	return _gsem_wait(sem, 0);
}

GCLAPI int gsem_post(gsem_t *sem)
{
	if ( sem->value > GSEM_VALUE_MAX ) return ERROR_ARITHMETIC_OVERFLOW;
	InterlockedIncrement( &sem->value );
	if ( !SetEvent(sem->h_sem) ) return GetLastError();
	return 0;
}

GCLAPI int gsem_getvalue(gsem_t *sem, int *pval)
{
	*pval = sem->value;
	return 0;
}
