/*
 *	* gthread_cond.c
 *
 *	OpenGCL Module : gthread - WIN32 POSIX compatable cond support.
 *
 *	Written by Soo-Hyuk Nam.
 *		2004. 1. 15. Thu.
 *
 *	History:
 *		2004/01/15: Originally written.
 */

#include "gthread_cond.h"

#if defined (_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <errno.h>
#include <windows.h>

static int _cv_timedwait(gthread_cond_t *cond, unsigned long timeout)
{
    int ret = 0;
    for (;;) {
        if (cond->nr_wakers > 0) {
            --cond->nr_wakers;
            break;
        }

        /* unlock cv */
        ReleaseMutex(cond->h_mutex);

        /* wait for wake up signal */
        DWORD r = WaitForSingleObject(cond->h_event, timeout);
        if (r == WAIT_TIMEOUT) {
            ret = r;
            break;
        }
        if (r != WAIT_OBJECT_0 && r != WAIT_ABANDONED) {
            ret = GetLastError();
            break;
        }

        /* lock cv again */
        r = WaitForSingleObject(cond->h_mutex, timeout);
        if (r == WAIT_TIMEOUT) {
            ret = r;
            break;
        }
        if (r != WAIT_OBJECT_0 && r != WAIT_ABANDONED) {
            ret = GetLastError();
            break;
        }
    }
    return ret;
}

static int _gthread_cond_wait(gthread_cond_t *cond, gthread_mutex_t *mtx, unsigned long timeout)
{
    /* lock cv(condition variable) */
    DWORD r = WaitForSingleObject(cond->h_mutex, timeout);
    if (r == WAIT_TIMEOUT) return r;
    if (r != WAIT_OBJECT_0 && r != WAIT_ABANDONED)
        return GetLastError();

    /* release user mutex: this might fail */
    r = gthread_mutex_unlock(mtx);
    if (r != 0) {
        ReleaseMutex(cond->h_mutex);
        return r;
    }

    ++cond->nr_sleepers;
    r = _cv_timedwait(cond, timeout);
    if (r != 0) {
        ReleaseMutex(cond->h_mutex);
        return r;
    }
    if (--cond->nr_sleepers == 0) cond->nr_wakers = 0;

    /* unlock cv */
    ReleaseMutex(cond->h_mutex);

    return gthread_mutex_lock(mtx);
}

gcl_api int gthread_cond_init(gthread_cond_t *cond, gthread_condattr_t *attr)
{
    if (attr != 0) return ERROR_INVALID_DATA;
    cond->h_mutex = CreateMutex(0, 0, 0);
    if (!cond->h_mutex) return GetLastError();
    cond->h_event = CreateEvent(0, 1, 0, 0);
    if (!cond->h_event) {
        int r = GetLastError();
        CloseHandle(cond->h_mutex);
        return r;
    }
    cond->nr_sleepers = 0;
    cond->nr_wakers = 0;
    return 0;
}

gcl_api int gthread_cond_destroy(gthread_cond_t *cond)
{
    CloseHandle(cond->h_mutex);
    CloseHandle(cond->h_event);
    return 0;
}

gcl_api int gthread_cond_signal(gthread_cond_t *cond)
{
    DWORD r = WaitForSingleObject(cond->h_mutex, INFINITE);
    if (r != WAIT_OBJECT_0 && r != WAIT_ABANDONED) return GetLastError();

    int do_wakeup = 0;
    if (cond->nr_sleepers > 0) {
        if (++cond->nr_wakers == 0) /* check for overflow */
            cond->nr_wakers = 0xFFFFFFFF;
        do_wakeup = 1;
    }
    ReleaseMutex(cond->h_mutex);

    if (!SetEvent(cond->h_event))
        return GetLastError();
    return 0;
}

gcl_api int gthread_cond_broadcast(gthread_cond_t *cond)
{
    DWORD r = WaitForSingleObject(cond->h_mutex, INFINITE);
    if (r != WAIT_OBJECT_0 && r != WAIT_ABANDONED)
        return GetLastError();

    int do_wakeup = 0;
    if (cond->nr_sleepers > 0) {
        cond->nr_wakers = 0xFFFFFFFF;
        do_wakeup = 1;
    }
    ReleaseMutex(cond->h_mutex);

    if (!SetEvent(cond->h_event)) return GetLastError();
    return 0;
}

gcl_api int gthread_cond_wait(gthread_cond_t *cond, gthread_mutex_t *mtx)
{
    return _gthread_cond_wait(cond, mtx, INFINITE);
}

gcl_api int gthread_cond_timedwait(gthread_cond_t *cond, gthread_mutex_t *mtx, unsigned long timeout_msec)
{
    return _gthread_cond_wait(cond, mtx, timeout_msec);
}

#else   // unit port
#include <sys/time.h>

//-----------------------------------------------------------------------------
//  unix port: pthread wrapper
//-----------------------------------------------------------------------------
gcl_api int gthread_cond_timedwait(gthread_cond_t *cond, gthread_mutex_t *mtx, unsigned long timeout_msec)
{
	struct timespec tm;
	struct timeval	tv;

	gettimeofday( &tv, 0 );
	tm.tv_sec = tv.tv_sec + timeout_msec / 1000;
	tm.tv_nsec = tv.tv_usec*1000 + (timeout_msec%1000) * 1000000;
	if ( tm.tv_nsec > 1000000000 )
	{
		++tm.tv_sec;
		tm.tv_nsec -= 1000000000;
	}
	return pthread_cond_timedwait(cond, mtx, &tm);
}
#endif
