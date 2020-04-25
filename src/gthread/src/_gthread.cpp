/*
 *	* gthread.c
 *
 *	OpenGCL Module : gthread - POSIX pthread compatables and C++ wrappers
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

#include "gthread.h"
#include "gsem.h"

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <errno.h>
#include <process.h>
#include <stdlib.h>
#include <windows.h>

/*
	TLS_MINIMUM_AVAIABLE per process is:
		linux			: 1024
		windows 98/Me	: 80
		windows 2000/XP	: 1088
*/
#define GTHREAD_KEYS_MAX TLS_MINIMUM_AVAILABLE

struct _key_entry {
    int             used;
    void            (*destr)(void *);
};

typedef struct {
    gthread_func_t  func;
    void            *data;
    HANDLE          h_thread;   /* OS handle */
    gsem_t          *sem;       /* parent --> thread */
    gsem_t          *sem_th;    /* thread --> parent */
    __gthread_tcb   *_tcb;      /* thread control block */
} _thread_param;

extern gthread_key_t        _gkey_self;     /* this is referenced by gthread_sleep.cpp */
static gthread_key_t        _gkey_cleanup = GTHREAD_INVALID_KEY;
//static gthread_key_t	    _gkey_object  = GTHREAD_INVALID_KEY;
static gthread_key_t        _gkey_tcb = GTHREAD_INVALID_KEY;
static struct _key_entry    _gthread_keys[GTHREAD_KEYS_MAX] = { {0, 0}, };
static __gthread_tcb        _tcb_main;

static void _main_thread_exit(void) { gthread_exit(0); }

static int _destroy_syskeys()
{
    if (_gkey_cleanup != GTHREAD_INVALID_KEY) {
        if (gthread_key_delete(_gkey_cleanup) != 0) return GetLastError();
        _gkey_cleanup = GTHREAD_INVALID_KEY;
    }
    /*	if ( _gkey_object != GTHREAD_INVALID_KEY )
	{
		if ( gthread_key_delete(_gkey_object) != 0 ) return GetLastError();
		_gkey_object = GTHREAD_INVALID_KEY;
	}
    */
    if (_gkey_self != GTHREAD_INVALID_KEY) {
        if (gthread_key_delete(_gkey_self) != 0) return GetLastError();
        _gkey_self = GTHREAD_INVALID_KEY;
    }
    if (_gkey_tcb != GTHREAD_INVALID_KEY) {
        if (gthread_key_delete(_gkey_tcb) != 0) return GetLastError();
        _gkey_tcb = GTHREAD_INVALID_KEY;
    }
    return 0;
}

static int _init_syskeys()
{
    if (_gkey_cleanup != GTHREAD_INVALID_KEY) return 0;
    int r = gthread_key_create(&_gkey_cleanup, 0);
    //	if ( r==0 ) r = gthread_key_create( &_gkey_object, 0 );
    if (r == 0) r = gthread_key_create(&_gkey_self, 0);
    if (r == 0) r = gthread_key_create(&_gkey_tcb, 0);
    if (r != 0) _destroy_syskeys();
    return r;
}

static int _init_tcb(__gthread_tcb *tcb, HANDLE h_thread)
{
    INIT_LIST_HEAD(&tcb->join_list);
    INIT_LIST_HEAD(&tcb->join_node);
    tcb->h_thread = h_thread;
    tcb->join_retval = 0;
    tcb->h_cancel = CreateEvent(0, 1, 0, 0);
    tcb->canceled = 0;
    if (!tcb->h_cancel) return GetLastError();
    return 0;
}

static int _destroy_tcb(__gthread_tcb *tcb)
{
    if (tcb->h_thread) CloseHandle(tcb->h_thread);
    if (tcb->h_cancel) CloseHandle(tcb->h_cancel);
    tcb->h_thread = 0;
    tcb->h_cancel = 0;
    return 0;
}

inline int _check_init()
{
    /* check for internal keys */
    int r = _init_syskeys();
    if (r != 0) return r;

    /* we need to init main thread. */
    if (_tcb_main.h_thread == 0) {
        HANDLE h_process = GetCurrentProcess();
        HANDLE h_thread = GetCurrentThread();
        if (!DuplicateHandle(h_process, h_thread, h_process, &h_thread, 0, FALSE, DUPLICATE_SAME_ACCESS)) {
            int r = GetLastError();
            _destroy_syskeys();
            return r;
        }
        int r = _init_tcb(&_tcb_main, h_thread);
        if (r != 0) { _destroy_syskeys(); return r; }

        gthread_setspecific(_gkey_self, &_tcb_main);
        atexit(_main_thread_exit);
    }
    return 0;
}

static void *_dummy_wrapper(void *data)
{
    _thread_param *pa = (_thread_param *)data;
    gthread_func_t func = pa->func;
    data = pa->data;

    /* wist until pa->h_thread is set. */
    gsem_wait(pa->sem);

    /*
		create thread control block
		_tcb resides in thread stack and is vaild as long as
		the thread is alive
	*/
    __gthread_tcb _tcb;
    int r = _init_tcb(&_tcb, pa->h_thread);
    if (r != 0) /* tcb init failed */
    {
        pa->data = (void *)(uintptr_t)r;
        pa->_tcb = (__gthread_tcb *)1; /* dummy */
        return 0;
    }
    _tcb.h_thread = pa->h_thread;
    gthread_setspecific(_gkey_self, &_tcb);

    pa->_tcb = &_tcb;

    /* notify that tcb is ready */
    gsem_post(pa->sem_th);

    gthread_exit(func(data));
    return 0;
}

static void _gthread_do_cleanup()
{
    _gthread_cleanup_buffer *pp;
    pp = (_gthread_cleanup_buffer *)gthread_getspecific(_gkey_cleanup);
    while (pp) {
        pp->__routine(pp->__arg);
        pp = pp->__prev;
    }
}

static void _gthread_destroy_specifics()
{
    for (int i = 0; i < GTHREAD_KEYS_MAX; ++i) {
        if (!_gthread_keys[i].used) continue;
        if (_gthread_keys[i].destr) {
            void *data = (void *)TlsGetValue(i);
            if (data) (*_gthread_keys[i].destr)(data);
        }
    }
}

gcl_api int gthread_create(gthread_t *th, gthread_attr_t *attr,
                           gthread_func_t func, void *data)
{
    int r = _check_init();
    if (r != 0) return r;

    /* create temporary semaphore */
    gsem_t sem, sem_th;
    r = gsem_init(&sem, 0, 0);
    if (r != 0) return r;
    r = gsem_init(&sem_th, 0, 0);
    if (r != 0) {
        gsem_destroy(&sem);
        return r;
    }

    unsigned int th_addr;
    _thread_param pa = {func, data, 0, &sem, &sem_th};
    pa.h_thread = (HANDLE)_beginthreadex(NULL, 0,
                                         (unsigned int(__stdcall *)(void *))_dummy_wrapper,
                                         &pa, 0, &th_addr);
    if (pa.h_thread == 0) {
        gsem_destroy(&sem);
        return errno;
    }

    /* notify the new thread that pa.h_thread was set. */
    gsem_post(&sem);

    /* wait until the new thread is ready(until pa._tcb is set).
	   Should it be faster if we use mutex instead of yield?
	   gthread_yield actually calls Sleep() and normally takes
	   about 20msec until it awakes.
	   --> Using semaphores seems to be faster. At least it increases
	       thread response time when created.
	*/
    gsem_wait(&sem_th);
    /* while ( pa._tcb == 0 ) gthread_yield(); */

    /* sanity check for w32 control block */
    if (pa._tcb == (__gthread_tcb *)1) /* error: set to 1 by new thread */
    {
        CloseHandle(pa.h_thread);
        gsem_destroy(&sem);
        gsem_destroy(&sem_th);
        return (int)(uintptr_t)pa.data;
    }

    *th = (gthread_t)pa._tcb;
    if (attr && attr->__detachstate == GTHREAD_CREATE_DETACHED) {
        CloseHandle(pa.h_thread);
        pa._tcb->h_thread = 0;
    }
    gsem_destroy(&sem);
    gsem_destroy(&sem_th);
    return 0;
}

gcl_api void gthread_exit(void *retval)
{
    _gthread_do_cleanup();
    _gthread_destroy_specifics();
    __gthread_tcb *_tcb = (__gthread_tcb *)gthread_self();

    if (_tcb) /* --> redundant sanity check: _tcb should never be null */
    {
        /* set retval to joined threads */
        if (!list_empty(&_tcb->join_list)) {
            list_head *pos, *tmp;
            list_for_each_safe(pos, tmp, &_tcb->join_list)
            {
                __gthread_tcb *_ent = list_entry(pos, __gthread_tcb, join_node);
                _ent->join_retval = (uintptr_t)retval;
                list_del_init(&_ent->join_node);
            }
        }

        list_del_init(&_tcb->join_node);
        _destroy_tcb(_tcb);
    }

    if (_tcb == &_tcb_main) {
        _destroy_syskeys(); /* destroy keys when main thread exits */
    }
    else {
        /* main thread does not explicitly call _endthread */
        _endthreadex((unsigned)(uintptr_t)retval);
    }
}

gcl_api int gthread_join(gthread_t gt, void **return_val)
{
    __gthread_tcb *_tcb_to = (__gthread_tcb *)gt;
    __gthread_tcb *_tcb_me = (__gthread_tcb *)gthread_self();
    if (IsBadWritePtr(_tcb_to, sizeof(*_tcb_to)))
        return ERROR_INVALID_HANDLE;

    list_add(&_tcb_me->join_node, &_tcb_to->join_list);
    DWORD r = WaitForSingleObject(_tcb_to->h_thread, INFINITE);
    if (r == WAIT_OBJECT_0 || r == WAIT_ABANDONED) {
        if (return_val)
            *return_val = (void *)_tcb_me->join_retval;
        return 0;
    }
    return GetLastError();
}

gcl_api int gthread_cancel(gthread_t gt)
{
    __gthread_tcb *_tcb = (__gthread_tcb *)gt;
    if (IsBadWritePtr(_tcb, sizeof(*_tcb))) return ERROR_INVALID_HANDLE;
    SetEvent(_tcb->h_cancel);   // awake if sleeping
    _tcb->canceled = true;      // mark cancel signaled
    return 0;
}

gcl_api void gthread_testcancel()
{
    __gthread_tcb *_tcb = (__gthread_tcb *)gthread_self();
    if (IsBadWritePtr(_tcb, sizeof(*_tcb))) return;
    if (_tcb->canceled) gthread_exit(GTHREAD_CANCELED);
}

gcl_api void gthread_detach(gthread_t th)
{
    __gthread_tcb *_tcb = (__gthread_tcb *)gthread_self();
    if (IsBadWritePtr(_tcb, sizeof(*_tcb))) return;

    CloseHandle(_tcb->h_thread);
}

gcl_api int gthread_key_create(gthread_key_t *key, void (*destr_func)(void *))
{
    DWORD tlsIndex = TlsAlloc();
    if (tlsIndex == TLS_OUT_OF_INDEXES) return -1; // error...
    _gthread_keys[tlsIndex].used = 1;
    _gthread_keys[tlsIndex].destr = destr_func;
    *key = (gthread_key_t)tlsIndex;
    return 0;
}

gcl_api int gthread_key_delete(gthread_key_t key)
{
    if (key >= GTHREAD_KEYS_MAX || !_gthread_keys[key].used)
        return ERROR_INVALID_DATA;
    if (!TlsFree(key)) return GetLastError();
    _gthread_keys[key].used = 0;
    _gthread_keys[key].destr = 0;
    return 0;
}

gcl_api int gthread_setspecific(gthread_key_t key, void *ptr)
{
    if (key >= GTHREAD_KEYS_MAX || !_gthread_keys[key].used)
        return ERROR_INVALID_DATA;
    if (!TlsSetValue(key, ptr)) return GetLastError();
    return 0;
}

gcl_api void *gthread_getspecific(gthread_key_t key)
{
    if (key >= GTHREAD_KEYS_MAX || !_gthread_keys[key].used)
        return 0;
    return TlsGetValue(key);
}

gcl_api gthread_t gthread_self()
{
    if (_check_init() != 0) return 0;
    return (gthread_t)gthread_getspecific(_gkey_self);
}

gcl_api void _gthread_cleanup_push(_gthread_cleanup_buffer *buf,
                                   void (*routine)(void *), void *arg)
{
    buf->__routine = routine;
    buf->__arg = arg;
    buf->__prev = (_gthread_cleanup_buffer *)gthread_getspecific(
        _gkey_cleanup);
    gthread_setspecific(_gkey_cleanup, buf);
}

gcl_api void _gthread_cleanup_pop(_gthread_cleanup_buffer *buf, int execute)
{
    if (execute) (*buf->__routine)(buf->__arg);
    gthread_setspecific(_gkey_cleanup, buf->__prev);
}

gcl_api int gthread_attr_init(gthread_attr_t *attr)
{
    attr->__detachstate = GTHREAD_CREATE_JOINABLE;
    attr->__stacksize = 0;
    return 0;
}

gcl_api int gthread_attr_destroy(gthread_attr_t *attr)
{
    return 0;
}

gcl_api int gthread_attr_setdetachstate(gthread_attr_t *attr,
                                        int _detachstate)
{
    if (_detachstate != GTHREAD_CREATE_JOINABLE && _detachstate != GTHREAD_CREATE_DETACHED) {
        return ERROR_INVALID_DATA;
    }
    attr->__detachstate = _detachstate;
    return 0;
}

gcl_api int gthread_attr_getdetachstate(gthread_attr_t *attr,
                                        int *_detachstate)
{
    *_detachstate = attr->__detachstate;
    return 0;
}

gcl_api int gthread_attr_setstacksize(gthread_attr_t *attr,
                                      size_t stacksize)
{
    attr->__stacksize = stacksize;
    return 0;
}

gcl_api int gthread_attr_getstacksize(gthread_attr_t *attr,
                                      size_t *stacksize)
{
    *stacksize = attr->__stacksize;
    return 0;
}

gcl_api int gthread_yield()
{
    // Sleep(0);
    gthread_sleep(0);
    return 0;
}

/* general pointer storage: primary used for gcl::thread class */
/*
int gthread_setobject(void *obj)
{
	int r = _check_init();
	if ( r != 0 ) return r;

	return gthread_setspecific( _gkey_object, obj );
}

void *gthread_getobject()
{
	return gthread_getspecific( _gkey_object );
}
*/

gcl_api uintptr_t gthread_get_oshandle(gthread_t gt)
{
    __gthread_tcb *_tcb = (__gthread_tcb *)gt;
    if (IsBadWritePtr(_tcb, sizeof(*_tcb))) return 0;

    return (uintptr_t)_tcb->h_thread;
}

#else   // unix port
#include <errno.h>
#define ERROR_INVALID_HANDLE	EINVAL

#endif
