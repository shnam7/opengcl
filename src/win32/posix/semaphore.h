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

typedef struct {
	long			value;
	void			*h_sem;
} sem_t;


/* current gsem implementation ignores pshared argument */
gcl_api int sem_init(sem_t *sem, int pshared, unsigned int val);

gcl_api int sem_destroy(sem_t *sem);

gcl_api int sem_wait(sem_t *sem);

gcl_api int sem_trywait(sem_t *sem);

gcl_api int sem_post(sem_t *sem);

gcl_api int sem_getvalue(sem_t *sem, int *pval);
