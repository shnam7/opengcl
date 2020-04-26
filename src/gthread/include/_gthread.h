/*
 *  @packaage OpenGCL
 *
 *  @module thread - POSIX pthread implementation for windows
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

#define	GTHREAD_NULL				((gthread_t)0)
typedef void                        *(*gthread_func_t)(void *data);


#if defined( _WIN32 )
/*--------------------------------------------------------------------
 * WIN32 port
 *--------------------------------------------------------------------*/
#define GTHREAD_CANCELED			((void *) -1)
#define GTHREAD_INVALID_KEY			((gthread_key_t)(-1))
// #define GTHREAD_INVALID_KEY			((gthread_key_t)TLS_OUT_OF_INDEXES)
#define GTHREAD_CREATE_JOINABLE		0
#define GTHREAD_CREATE_DETACHED		1

typedef	uintptr_t                   gthread_t;
typedef struct { int __detachstate; size_t __stacksize; } gthread_attr_t;
typedef	unsigned long               gthread_key_t;

// cleanup buffers
typedef struct __gthread_cleanup_buffer {
    void        (*__routine)(void *);
    void        *__arg;
    int         __canceltype;
    struct __gthread_cleanup_buffer *__prev;
} _gthread_cleanup_buffer;

gcl_api int gthread_create(gthread_t *th, gthread_attr_t *attr, gthread_func_t func, void *data);
gcl_api void gthread_exit(void *retval);
gcl_api int gthread_join(gthread_t gt, void **return_val);
gcl_api int gthread_cancel(gthread_t th);
gcl_api void gthread_testcancel();
gcl_api void gthread_detach(gthread_t th);

// thread private storage
gcl_api int gthread_key_create(gthread_key_t *key, void (*destr_func)(void *) );
gcl_api int gthread_key_delete(gthread_key_t key);
gcl_api int gthread_setspecific(gthread_key_t key, void *ptr);
gcl_api void *gthread_getspecific(gthread_key_t key);
gcl_api gthread_t gthread_self();

// gthread attributes
gcl_api int gthread_attr_init(gthread_attr_t *attr);
gcl_api int gthread_attr_destroy(gthread_attr_t *attr);
gcl_api int gthread_attr_setdetachstate(gthread_attr_t *attr, int _detachstate);
gcl_api int gthread_attr_getdetachstate(gthread_attr_t *attr, int *_detachstate);
gcl_api int gthread_attr_setstacksize(gthread_attr_t *attr, size_t stacksize);
gcl_api int gthread_attr_getstacksize(gthread_attr_t *attr, size_t *stacksize);


/* gthread_yield: GNU extension - not POSIX standard.
 *
 * Win32 implementation uses Sleep() and this may take
 * fairly long time (about 10-20 msec).
 * The return value is always zero.
 *
 * GNU implementation uses sched_yield().
 * Refer to man relevant pages for details.
 */
#if defined( _WIN32 ) || defined( __USE_GNU )
gcl_api int gthread_yield();
#endif

#define gthread_cleanup_push(routine,arg) \
	{ _gthread_cleanup_buffer __buffer_; \
		_gthread_cleanup_push(&__buffer_, (routine), (arg));

gcl_api void _gthread_cleanup_push(_gthread_cleanup_buffer *buf, void (*routine)(void *), void *arg);

#define gthread_cleanup_pop(execute) \
    _gthread_cleanup_pop(&__buffer_, (execute)); }

gcl_api void _gthread_cleanup_pop(_gthread_cleanup_buffer *buf, int execute);

// sem
// pthread once
// spin
/*	barrier
	Software objects used to ensure that all threads in a
	group are ready before any of them proceed.
	--> not supported
*/
/* cancel state */
/* atfork */
/* non-POSIX extensions */
/*
gcl_api int gthread_setobject(void *obj);
gcl_api void *gthread_getobject();
*/

gcl_api uintptr_t gthread_get_oshandle(gthread_t th);


#else	/* unix port: wrapper to pthread */
/*--------------------------------------------------------------------
 * unix port
 *--------------------------------------------------------------------*/
#include <pthread.h>

#define GTHREAD_CANCELED            PTHREAD_CANCELED
#define GTHREAD_INVALID_KEY         ((gthread_key_t)(-1))
#define GTHREAD_CREATE_JOINABLE     PTHREAD_CREATE_JOINABLE
#define GTHREAD_CREATE_DETACHED     PTHREAD_CREATE_DETACHED

typedef pthread_t                   gthread_t;
typedef pthread_attr_t              gthread_attr_t;
typedef pthread_key_t               gthread_key_t;

inline int gthread_create(gthread_t *th, gthread_attr_t *attr, gthread_func_t func, void *data)
    { return pthread_create(th, attr, func, data); }

inline void gthread_exit(void *retval) { pthread_exit(retval); }

inline int gthread_join(gthread_t th, void **return_val) { return pthread_join(th, return_val); }

inline int gthread_cancel(gthread_t th) { return pthread_cancel(th); }

inline void gthread_testcancel() { return pthread_testcancel(); }

inline void gthread_detach(gthread_t th) { pthread_detach(th); }

/* thread private storage */
inline int gthread_key_create(gthread_key_t *key, void (*destr_func)(void *) )
    { return pthread_key_create(key, destr_func); }

inline int gthread_key_delete(gthread_key_t key) { return pthread_key_delete(key); }

inline int gthread_setspecific(gthread_key_t key, void *ptr) { return pthread_setspecific(key, ptr); }

inline void *gthread_getspecific(gthread_key_t key) { return pthread_getspecific(key); }

inline gthread_t gthread_self() { return pthread_self(); }

/* gthread attributes */
inline int gthread_attr_init(gthread_attr_t *attr) { return pthread_attr_init(attr); }

inline int gthread_attr_destroy(gthread_attr_t *attr) { return pthread_attr_destroy(attr); }

inline int gthread_attr_setdetachstate(gthread_attr_t *attr, int _detachstate)
    { return pthread_attr_setdetachstate(attr, _detachstate); }

inline int gthread_attr_getdetachstate(gthread_attr_t *attr, int *_detachstate)
    { return pthread_attr_getdetachstate(attr, _detachstate); }

inline int gthread_attr_setstacksize(gthread_attr_t *attr, size_t stacksize)
    { return pthread_attr_setstacksize(attr, stacksize); }

inline int gthread_attr_getstacksize(gthread_attr_t *attr, size_t *stacksize)
    { return pthread_attr_getstacksize(attr, stacksize); }

#ifdef __USE_GNU
/* GNU specific - not POSIX standard * It returns always zero. */
inline int gthread_yield() { return pthread_yield(); }
#endif

/* gthread cleanup */
#define gthread_cleanup_push(routine,arg) pthread_cleanup_push(routine, arg)
#define gthread_cleanup_pop(execute) pthread_cleanup_pop(execute)

/* pthread once */
/* spin */
/*	barrier
	Software objects used to ensure that all threads in a
	group are ready before any of them proceed.

	--> not supported yat.
*/
/* cancel state */
/* atfork */
/* non-POSIX extensions */

inline uintptr_t gthread_get_oshandle(gthread_t th) { return (uintptr_t)th; }

#endif	/* unix port */


gcl_api void gthread_sleep(u32_t msec);
gcl_api void gthread_nanosleep(u64_t nsec);
