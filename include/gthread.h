/*
 *  @packaage OpenGCL
 *
 *  @module thread - POSIX pthread compatables and C++ wrappers
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
#include "gtime.h"
#include <pthread.h>

//--------------------------------------------------------------------
//	class wrappers
//--------------------------------------------------------------------
namespace gcl
{

//--- runnable
class gcl_api runnable
{
    friend class thread;

protected:
    runnable() {}
    virtual ~runnable() {}
    virtual void *run() { return (void *)0; }

    // NOTE: these functions applies for gthread_self()
    //   and should not be called by other thread.
    void testCancel() { pthread_testcancel(); }
};

//--- thread
class gcl_api thread : public runnable
{
protected:
    pthread_t       m_t;                        // pthread instance handle
    u32_t           m_tid = (unsigned)(-1);     // thread id

protected:
    static pthread_key_t    __key_to_instance;  // key to store pointer to thread object
    static unsigned         __instance_count;   // # of active thread instances
    static void *__thread_wrapper(void *);
    static void __thread_destructor(void *);

public:
    thread();
    ~thread();

    //--- manipulators
    int start(pthread_attr_t *attr=0) { return start(this, attr); };

    int start(void *(*proc)(void *), void *data=0, pthread_attr_t *attr=0);

    int start(runnable *runnable, pthread_attr_t *attr=0);
    int start(runnable &runnable, pthread_attr_t *attr=0) { return start(&runnable, attr); }

    int join(void **retval = 0) const { return is_running() ? pthread_join(m_t, retval) : 0; }

    int stop(void *retval = 0);

    void cancel() { if (is_running()) pthread_cancel(m_t); }

    //--- accessors
    bool is_running() const { return m_tid != (unsigned)(-1); }

    int thread_id() const { return m_tid; }

    pthread_t get_handle() const { return m_t; }

public:
    static thread *get_current();

protected:
    // not-allowed operations
    thread(pthread_t t);
    thread(thread &t);
    thread &operator=(thread &t);
    thread &operator=(pthread_t t);
};


//--- mutex
class gcl_api mutex
{
protected:
    pthread_mutex_t m_mtx;

public:
    mutex() { pthread_mutex_init(&m_mtx, 0); }
    mutex(int kind);
    ~mutex() { pthread_mutex_destroy(&m_mtx); }

    bool lock() { return pthread_mutex_lock(&m_mtx) == 0; }

    bool unlock() { return pthread_mutex_unlock(&m_mtx) == 0; }

    bool trylock() { return pthread_mutex_trylock(&m_mtx) == 0; }

    pthread_mutex_t *get_handle() { return &m_mtx; }
};


//--- cond
class gcl_api condition
{
protected:
    pthread_cond_t m_cond;

public:
    condition() { pthread_cond_init(&m_cond, 0); }
    ~condition() { pthread_cond_destroy(&m_cond); }

    bool signal() { return pthread_cond_signal(&m_cond) == 0; }

    bool broadcast() { return pthread_cond_broadcast(&m_cond) == 0; }

    bool wait(pthread_mutex_t *mtx) { return pthread_cond_wait(&m_cond, mtx) == 0; }

    bool wait(mutex &mtx) { return pthread_cond_wait(&m_cond, mtx.get_handle()) == 0; }

    bool timedwait(pthread_mutex_t *mtx, const struct timespec *ts) { return pthread_cond_timedwait(&m_cond, mtx, ts) == 0; }

    bool timedwait(pthread_mutex_t *mtx, unsigned long timeout_msec)
    {
        struct timespec ts;
        ts.tv_sec = timeout_msec / 1000;
        ts.tv_nsec = (timeout_msec % 1000) * 1000000;
        return pthread_cond_timedwait(&m_cond, mtx, &ts) == 0;
    }

    bool timedwait(mutex &mtx, const struct timespec *ts) { return timedwait(mtx.get_handle(), ts); }

    bool timedwait(mutex &mtx, unsigned long timeout_msec) { return timedwait(mtx.get_handle(), timeout_msec); }

    pthread_cond_t *get_handle() { return &m_cond; }
};


//--- rwlock
class gcl_api rwlock
{
protected:
    pthread_rwlock_t m_rwlock;

public:
    rwlock() { pthread_rwlock_init(&m_rwlock, 0); }
    ~rwlock() { pthread_rwlock_destroy(&m_rwlock); }

    int rdlock() { return pthread_rwlock_rdlock(&m_rwlock); }

    int tryrdlock() { return pthread_rwlock_tryrdlock(&m_rwlock); }

    int wrlock() { return pthread_rwlock_wrlock(&m_rwlock); }

    int trywrlock() { return pthread_rwlock_trywrlock(&m_rwlock); }

    int unlock() { return pthread_rwlock_unlock(&m_rwlock); }

    pthread_rwlock_t *get_handle() { return &m_rwlock; }
};


//--- autolock: locked by constructor, unlocked by destructor
class gcl_api autolock {
protected:
    mutex &m_mtx;

public:
    autolock(mutex &mtx) : m_mtx(mtx) { mtx.lock(); }
    ~autolock() { m_mtx.unlock(); }
};


//--- autolock_rd: autolock for read
class gcl_api autolock_rd {
protected:
    rwlock &m_lock;

public:
    autolock_rd(rwlock &lock) : m_lock(lock) { lock.rdlock(); }
    ~autolock_rd() { m_lock.unlock(); }
};


//--- autolock_wr: autolock for read
class gcl_api autolock_wr {
protected:
    rwlock &m_lock;

public:
    autolock_wr(rwlock &lock) : m_lock(lock) { lock.wrlock(); }
    ~autolock_wr() { m_lock.unlock(); }
};

} // namespace gcl
