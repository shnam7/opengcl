/*
 *	* gsemaphore.h
 *
 *	OpenGCL Module : gsemaphore - POSIX semaphore compatables and C++ wrappers
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
#include <semaphore.h>

namespace gcl
{

//--------------------------------------------------------------------
//	class Semaphore
//--------------------------------------------------------------------
class gcl_api Semaphore
{
protected:
    sem_t m_sem;

public:
    Semaphore(unsigned int val = 0) { sem_init(&m_sem, 0, val); }
    ~Semaphore() { sem_destroy(&m_sem); }

    int wait() { return sem_wait(&m_sem); }
    int trywait() { return sem_trywait(&m_sem); }
    int post() { return sem_post(&m_sem); }
    int get_value(int *pval) { return sem_getvalue(&m_sem, pval); }

    sem_t *get_handle() { return &m_sem; }
};

} // namespace gcl
