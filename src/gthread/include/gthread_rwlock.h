/*
 *  @packaage OpenGCL
 *
 *  @module rwlock - POSIX pthread_rwlock implementation for windows
 *
 *	Written by Soo-Hyuk Nam.
 *		2004. 1. 15. Thu.
 *
 *	History:
 *		2004/01/15: Originally written.
 */

#pragma once
#include "gcldef.h"
#include "gthread_mutex.h"

#if defined (_WIN32)
typedef struct { void *h_wrlock; void *h_rdlock; long readers; } gthread_rwlock_t;
typedef struct { int dummy; } gthread_rwlockattr_t;

// rwlock
// #if defined(_WIN32) || defined(__USE_UNIX98)
gcl_api int gthread_rwlock_init(gthread_rwlock_t *rwlock, gthread_rwlockattr_t *attr);
gcl_api int gthread_rwlock_destroy(gthread_rwlock_t *rwlock);
gcl_api int gthread_rwlock_rdlock(gthread_rwlock_t *rwlock);
gcl_api int gthread_rwlock_tryrdlock(gthread_rwlock_t *rwlock);
gcl_api int gthread_rwlock_wrlock(gthread_rwlock_t *rwlock);
gcl_api int gthread_rwlock_trywrlock(gthread_rwlock_t *rwlock);
gcl_api int gthread_rwlock_unlock(gthread_rwlock_t *rwlock);

#else   // unix port: use pthread
typedef pthread_rwlock_t		gthread_rwlock_t;
typedef pthread_rwlockattr_t	gthread_rwlockattr_t;

// #ifdef __USE_UNIX98
inline int gthread_rwlock_init(gthread_rwlock_t *rwlock, gthread_rwlockattr_t *attr)
    { return pthread_rwlock_init(rwlock, attr); }

inline int gthread_rwlock_destroy(gthread_rwlock_t *rwlock) { return pthread_rwlock_destroy(rwlock); }

inline int gthread_rwlock_rdlock(gthread_rwlock_t *rwlock) { return pthread_rwlock_rdlock(rwlock); }

inline int gthread_rwlock_tryrdlock(gthread_rwlock_t *rwlock) { return pthread_rwlock_tryrdlock(rwlock); }

inline int gthread_rwlock_wrlock(gthread_rwlock_t *rwlock) { return pthread_rwlock_wrlock(rwlock); }

inline int gthread_rwlock_trywrlock(gthread_rwlock_t *rwlock) { return pthread_rwlock_trywrlock(rwlock); }

inline int gthread_rwlock_unlock(gthread_rwlock_t *rwlock) { return pthread_rwlock_unlock(rwlock); }
// #endif

#endif
