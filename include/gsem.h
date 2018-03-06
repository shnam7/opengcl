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
 *	* gsem.h
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

#ifndef __GSEM_H
#define __GSEM_H

#if !defined( _WIN32 )
#include <semaphore.h>
#endif

#if defined( _WIN32 ) && defined(_DLL)
#ifdef GCL_EXPORTS
#define GCLAPI		__declspec(dllexport)
#else
#define GCLAPI		__declspec(dllimport)
#endif
#else
#define GCLAPI
#endif

#ifdef __cplusplus
extern "C" {
#endif


#if defined( _WIN32 )	/* win32 port */

typedef struct {
	long			value;
	void			*h_sem;
} gsem_t;


/* current gsem implementation ignores pshared argument */
GCLAPI int gsem_init(gsem_t *sem, int pshared, unsigned int val);

GCLAPI int gsem_destroy(gsem_t *sem);

GCLAPI int gsem_wait(gsem_t *sem);

GCLAPI int gsem_trywait(gsem_t *sem);

GCLAPI int gsem_post(gsem_t *sem);

GCLAPI int gsem_getvalue(gsem_t *sem, int *pval);

#else	/* unix port */

typedef sem_t		gsem_t;


/* current gsem implementation ignores pshared argument
 * and so does current GNU implementation */
static inline int gsem_init(gsem_t *sem, int pshared, unsigned int val)
	{ return sem_init(sem, pshared, val); }

static inline int gsem_destroy(gsem_t *sem)
	{ return sem_destroy(sem); }

static inline int gsem_wait(gsem_t *sem)
	{ return sem_wait(sem); }

static inline int gsem_trywait(gsem_t *sem)
	{ return sem_trywait(sem); }

static inline int gsem_post(gsem_t *sem)
	{ return sem_post(sem); }

static inline int gsem_getvalue(gsem_t *sem, int *sval)
	{ return sem_getvalue(sem, sval); }

#endif


#ifdef __cplusplus
}

//--------------------------------------------------------------------
//	class GSem
//--------------------------------------------------------------------
class GCLAPI GSem {
protected:
	gsem_t		m_sem;

public:
	GSem(unsigned int val=0) { gsem_init(&m_sem, 0, val); }

	~GSem() { gsem_destroy(&m_sem); }

	int wait() { return gsem_wait(&m_sem); }

	int tryWait() { return gsem_wait(&m_sem); }

	int post() { return gsem_post(&m_sem); }

	int getValue(int *pval) { return gsem_getvalue(&m_sem, pval); }
};

#endif

#endif
