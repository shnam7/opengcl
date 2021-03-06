/*
 *  @packaage OpenGCL
 *
 *  @module gthread - POSIX pthread compatables and C++ wrappers
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

//--- Runnable
class gcl_api Runnable
{
    friend class Thread;

protected:
    Runnable() {}
    virtual ~Runnable() {}
    virtual void *run() { return (void *)0; }

    // NOTE: these functions applies for gthread_self()
    //   and should not be called by other thread.
    void testCancel() { pthread_testcancel(); }
};

//--- Thread
class gcl_api Thread : public Runnable
{
protected:
    pthread_t       m_t;                        // pthread instance handle
    u32_t           m_tid = (unsigned)(-1);     // thread id

protected:
    static void __thread_destructor(void *);
    static void *__thread_wrapper(void *);

public:
    Thread();
    Thread(void *(*proc)(void *), void *data=0, pthread_attr_t *attr=0) { start(proc, data, attr); }
    Thread(Runnable *Runnable, pthread_attr_t *attr=0) { start(Runnable, attr); }
    ~Thread();

    //--- manipulators
    int start(pthread_attr_t *attr=0) { return start(this, attr); };

    int start(void *(*proc)(void *), void *data=0, pthread_attr_t *attr=0);

    int start(Runnable *Runnable, pthread_attr_t *attr=0);

    int start(Runnable &Runnable, pthread_attr_t *attr=0) { return start(&Runnable, attr); }

    int join(void **retval = 0) const { return is_running() ? pthread_join(m_t, retval) : 0; }

    int stop(void *retval = 0);

    int cancel() { return is_running() ? pthread_cancel(m_t) : 0; }

    //--- accessors
    bool is_running() const { return m_tid != (unsigned)(-1); }

    int thread_id() const { return m_tid; }

    pthread_t get_handle() const { return m_t; }

public:
    static Thread *get_current();
    static unsigned get_thread_counts();    // return running threads count

protected:
    // not-allowed operations
    Thread(pthread_t t);
    Thread(Thread &t);
    Thread &operator=(Thread &t);
    Thread &operator=(pthread_t t);
};


//--- Mutex
class gcl_api Mutex
{
protected:
    pthread_mutex_t m_mtx;

public:
    Mutex() { pthread_mutex_init(&m_mtx, 0); }
    Mutex(int kind);
    ~Mutex() { pthread_mutex_destroy(&m_mtx); }

    bool lock() { return pthread_mutex_lock(&m_mtx) == 0; }

    bool unlock() { return pthread_mutex_unlock(&m_mtx) == 0; }

    bool trylock() { return pthread_mutex_trylock(&m_mtx) == 0; }

    pthread_mutex_t *get_handle() { return &m_mtx; }
};


//--- cond
class gcl_api Condition
{
protected:
    pthread_cond_t m_cond;

public:
    Condition() { pthread_cond_init(&m_cond, 0); }
    ~Condition() { pthread_cond_destroy(&m_cond); }

    bool signal() { return pthread_cond_signal(&m_cond) == 0; }

    bool broadcast() { return pthread_cond_broadcast(&m_cond) == 0; }

    bool wait(pthread_mutex_t *mtx) { return pthread_cond_wait(&m_cond, mtx) == 0; }

    bool wait(Mutex &mtx) { return pthread_cond_wait(&m_cond, mtx.get_handle()) == 0; }

    bool timedwait(pthread_mutex_t *mtx, const struct timespec *ts) { return pthread_cond_timedwait(&m_cond, mtx, ts) == 0; }

    bool timedwait(pthread_mutex_t *mtx, unsigned long timeout_msec)
    {
        struct timespec ts;
        ts.tv_sec = timeout_msec / 1000;
        ts.tv_nsec = (timeout_msec % 1000) * 1000000;
        return pthread_cond_timedwait(&m_cond, mtx, &ts) == 0;
    }

    bool timedwait(Mutex &mtx, const struct timespec *ts) { return timedwait(mtx.get_handle(), ts); }

    bool timedwait(Mutex &mtx, unsigned long timeout_msec) { return timedwait(mtx.get_handle(), timeout_msec); }

    pthread_cond_t *get_handle() { return &m_cond; }
};


//--- RWLock
class gcl_api RWLock
{
protected:
    pthread_rwlock_t m_rwlock;

public:
    RWLock() { pthread_rwlock_init(&m_rwlock, 0); }
    ~RWLock() { pthread_rwlock_destroy(&m_rwlock); }

    int rdlock() { return pthread_rwlock_rdlock(&m_rwlock); }

    int tryrdlock() { return pthread_rwlock_tryrdlock(&m_rwlock); }

    int wrlock() { return pthread_rwlock_wrlock(&m_rwlock); }

    int trywrlock() { return pthread_rwlock_trywrlock(&m_rwlock); }

    int unlock() { return pthread_rwlock_unlock(&m_rwlock); }

    pthread_rwlock_t *get_handle() { return &m_rwlock; }
};


//--- AutoLock: locked by constructor, unlocked by destructor
class gcl_api AutoLock {
protected:
    Mutex &m_mtx;

public:
    AutoLock(Mutex &mtx) : m_mtx(mtx) { mtx.lock(); }
    ~AutoLock() { m_mtx.unlock(); }
};


//--- AutoLockRead: AutoLock for read
class gcl_api AutoLockRead {
protected:
    RWLock &m_lock;

public:
    AutoLockRead(RWLock &lock) : m_lock(lock) { lock.rdlock(); }
    ~AutoLockRead() { m_lock.unlock(); }
};


//--- AutoLockWrite: AutoLock for write
class gcl_api AutoLockWrite {
protected:
    RWLock &m_lock;

public:
    AutoLockWrite(RWLock &lock) : m_lock(lock) { lock.wrlock(); }
    ~AutoLockWrite() { m_lock.unlock(); }
};

} // namespace gcl
