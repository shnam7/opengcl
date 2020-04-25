/*
 *  @packaage OpenGCL
 *
 *  @module mutex - Mutex
 */

#pragma once
#include "gcldef.h"

#if defined (_WIN32)

#define GTHREAD_MUTEX_FAST			1
#define GTHREAD_MUTEX_RECURSIVE		2

typedef struct { int __mutexkind; void *h_mutex; } gthread_mutex_t;
typedef struct { int __mutexkind; } gthread_mutexattr_t;

// mutex
gcl_api int gthread_mutex_init(gthread_mutex_t *mtx, const gthread_mutexattr_t *attr);
gcl_api int gthread_mutex_destroy(gthread_mutex_t *mtx);
gcl_api int gthread_mutex_trylock(gthread_mutex_t *mtx);
gcl_api int gthread_mutex_lock(gthread_mutex_t *mtx);
gcl_api int gthread_mutex_unlock(gthread_mutex_t *mtx);
gcl_api int gthread_mutexattr_init(gthread_mutexattr_t *attr);
gcl_api int gthread_mutexattr_destroy(gthread_mutexattr_t *attr);
gcl_api int gthread_mutexattr_settype(gthread_mutexattr_t *attr, int kind);
gcl_api int gthread_mutexattr_gettype(gthread_mutexattr_t *attr, int *kind);


#else   // unix port: use pthread
#include <pthread.h>
typedef pthread_mutex_t			gthread_mutex_t;
typedef pthread_mutexattr_t		gthread_mutexattr_t;


/* mutex */
inline int gthread_mutex_init(gthread_mutex_t *mtx, const gthread_mutexattr_t *attr)
    { return pthread_mutex_init(mtx, attr); }

inline int gthread_mutex_destroy(gthread_mutex_t *mtx) { return pthread_mutex_destroy(mtx); }

inline int gthread_mutex_trylock(gthread_mutex_t *mtx) { return pthread_mutex_trylock(mtx); }

inline int gthread_mutex_lock(gthread_mutex_t *mtx) { return pthread_mutex_lock(mtx); }

inline int gthread_mutex_unlock(gthread_mutex_t *mtx) { return pthread_mutex_unlock(mtx); }

inline int gthread_mutexattr_init(gthread_mutexattr_t *attr) { return pthread_mutexattr_init(attr); }

inline int gthread_mutexattr_destroy(gthread_mutexattr_t *attr) { return pthread_mutexattr_destroy(attr); }

inline int gthread_mutexattr_settype(gthread_mutexattr_t *attr, int kind)
    { return pthread_mutexattr_settype(attr, kind); }

inline int gthread_mutexattr_gettype(gthread_mutexattr_t *attr, int *kind)
    { return pthread_mutexattr_gettype(attr, kind); }

#endif
