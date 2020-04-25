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

#pragma once
#include "gcldef.h"

#if defined( _WIN32 )	/* win32 port */

typedef struct {
	long			value;
	void			*h_sem;
} gsem_t;


/* current gsem implementation ignores pshared argument */
gcl_api int gsem_init(gsem_t *sem, int pshared, unsigned int val);

gcl_api int gsem_destroy(gsem_t *sem);

gcl_api int gsem_wait(gsem_t *sem);

gcl_api int gsem_trywait(gsem_t *sem);

gcl_api int gsem_post(gsem_t *sem);

gcl_api int gsem_getvalue(gsem_t *sem, int *pval);

#else	/* unix port */
#include <semaphore.h>
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


namespace gcl {

//--------------------------------------------------------------------
//	class semaphore
//--------------------------------------------------------------------
class gcl_api gsem {
protected:
	gsem_t		m_sem;

public:
	gsem(unsigned int val=0) { gsem_init(&m_sem, 0, val); }

	~gsem() { gsem_destroy(&m_sem); }

	int wait() { return gsem_wait(&m_sem); }

	int tryWait() { return gsem_wait(&m_sem); }

	int post() { return gsem_post(&m_sem); }

	int getValue(int *pval) { return gsem_getvalue(&m_sem, pval); }
};

} // namespace gcl
