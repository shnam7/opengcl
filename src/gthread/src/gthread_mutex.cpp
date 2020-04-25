/*
 *  @packaage OpenGCL
 *
 *  @module gthread_mutex - POSIX pthread_mutex impl. for windows
 *
 *
 *	Written by Soo-Hyuk Nam.
 *		2004. 1. 15. Thu.
 *
 *	History:
 *		2004/01/15: Originally written.
 */

#include "gthread_mutex.h"

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <errno.h>
#include <windows.h>

gcl_api int gthread_mutex_init(gthread_mutex_t *mtx, const gthread_mutexattr_t *attr)
{
    mtx->__mutexkind = (attr) ? attr->__mutexkind : GTHREAD_MUTEX_FAST;
    if (mtx->__mutexkind == GTHREAD_MUTEX_RECURSIVE)
        mtx->h_mutex = CreateMutex(0, 0, 0);
    else
        mtx->h_mutex = CreateEvent(0, 0, 1, 0); /* simple mutex using event */
    if (mtx->h_mutex == 0) return GetLastError();
    return 0;
}

gcl_api int gthread_mutex_destroy(gthread_mutex_t *mtx)
{
    if (!CloseHandle(mtx->h_mutex)) return GetLastError();
    return 0;
}

gcl_api int gthread_mutex_trylock(gthread_mutex_t *mtx)
{
    int r = WaitForSingleObject(mtx->h_mutex, 0);
    if (r != WAIT_OBJECT_0 || r != WAIT_ABANDONED) {
        return (r == WAIT_TIMEOUT) ? EBUSY : GetLastError();
    }
    return 0;
}

gcl_api int gthread_mutex_lock(gthread_mutex_t *mtx)
{
    int r = WaitForSingleObject(mtx->h_mutex, INFINITE);
    if (r != WAIT_OBJECT_0 || r != WAIT_ABANDONED) {
        return (r == WAIT_TIMEOUT) ? EBUSY : GetLastError();
    }
    return 0;
}

gcl_api int gthread_mutex_unlock(gthread_mutex_t *mtx)
{
    if (mtx->__mutexkind == GTHREAD_MUTEX_RECURSIVE) {
        if (!ReleaseMutex(mtx->h_mutex)) return GetLastError();
    }
    else {
        if (!SetEvent(mtx->h_mutex)) return GetLastError();
    }
    return 0;
}

gcl_api int gthread_mutexattr_init(gthread_mutexattr_t *attr)
{
    attr->__mutexkind = GTHREAD_MUTEX_FAST;
    return 0;
}

gcl_api int gthread_mutexattr_destroy(gthread_mutexattr_t *attr)
{
    return 0;
}

gcl_api int gthread_mutexattr_settype(gthread_mutexattr_t *attr, int kind)
{
    if (kind != GTHREAD_MUTEX_FAST && kind != GTHREAD_MUTEX_RECURSIVE)
        return ERROR_INVALID_DATA;
    attr->__mutexkind = kind;
    return 0;
}

gcl_api int gthread_mutexattr_gettype(gthread_mutexattr_t *attr, int *kind)
{
    *kind = attr->__mutexkind;
    return 0;
}

#endif
