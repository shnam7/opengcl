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
#include "_gthread.h"
#include "gthread_mutex.h"
#include "gthread_cond.h"
#include "gthread_rwlock.h"

//--------------------------------------------------------------------
//	class wrappers
//--------------------------------------------------------------------
namespace gcl {

inline void sleep(u32_t msec) { gthread_sleep(msec); }
inline void nanosleep(u64_t nsec) { gthread_nanosleep(nsec); }


//--- runnable
class gcl_api runnable {
    friend class gthread;

protected:
    runnable() {}
    virtual ~runnable() {}
    virtual void *run() { return (void *)0; }

    // NOTE: these functions applies for gthread_self()
    //   and should not be called by other thread.
    void testCancel() { gthread_testcancel(); }
};


//--- thread
class gcl_api gthread : public runnable {
public:
    typedef runnable runnable;  // make gthread::runnable

protected:
    gthread_t       m_t;

    // Not-allowed operations
    gthread(gthread_t th);
    gthread(gthread &th);
    gthread &operator=(gthread &th);
    gthread &operator=(gthread_t th);

public:
    gthread();
    ~gthread();

    //--- manipulators
    int start() { return start(this); }
    int start(runnable *runnable);
    int start(gthread_func_t func, void *data = 0);

    int join(void **thread_return = 0) const;
    int stop();
    void cancel() { gthread_cancel(m_t); }

    //--- accessors
    bool isRunning() const { return m_t != GTHREAD_NULL; }
    uintptr_t threadID() const { return (uintptr_t)m_t; }
    gthread_t threadHandle() const { return m_t; }

    // Returns OS dependent handle.
    // On win32 platforms, it returns win32 handle to thread.
    uintptr_t getOSHandle() const { return gthread_get_oshandle(m_t); }

protected:
    int __start(void *pa);
    static void *_thread_wrapper(void *);
    static void _thread_cleanup(void *);

public:
    // static thread *getCurrent() { return (thread *)gthread_getobject(); }
    static gthread *getCurrent();
};


//--- mutex
class gcl_api gmutex {
protected:
    gthread_mutex_t m_mtx;

public:
    gmutex() { gthread_mutex_init(&m_mtx, 0); }
    gmutex(int kind);
    ~gmutex() { gthread_mutex_destroy(&m_mtx); }

    bool lock() { return gthread_mutex_lock(&m_mtx) == 0; }
    bool unlock() { return gthread_mutex_unlock(&m_mtx) == 0; }
    bool trylock() { return gthread_mutex_trylock(&m_mtx) == 0; }
    gthread_mutex_t *getMutex() { return &m_mtx; }
};


//--- cond
class cond {
protected:
	gthread_cond_t		m_cond;

public:
	cond() { gthread_cond_init(&m_cond, 0); }
	~cond() { gthread_cond_destroy(&m_cond); }

	bool signal() { return gthread_cond_signal(&m_cond)==0; }
	bool broadcast() { return gthread_cond_broadcast(&m_cond)==0; }
	bool wait(gthread_mutex_t *mtx) { return gthread_cond_wait(&m_cond, mtx)==0; }
	bool wait(gmutex& mtx) { return gthread_cond_wait(&m_cond, mtx.getMutex())==0; }
	bool timedwait(gthread_mutex_t *mtx, unsigned long timeout_msec)
		{ return gthread_cond_timedwait(&m_cond, mtx, timeout_msec)==0; }
	bool timedwait(gmutex& mtx, unsigned long timeout_msec)
		{ return timedwait(mtx.getMutex(), timeout_msec); }
	gthread_cond_t *getCond() { return &m_cond; }
};


//--- rwlock
class rwlock {
protected:
	gthread_rwlock_t		m_rwlock;

public:
	rwlock() { gthread_rwlock_init(&m_rwlock, 0); }
	~rwlock() { gthread_rwlock_destroy(&m_rwlock); }

	int rdlock() { return gthread_rwlock_rdlock(&m_rwlock); }
	int tryrdlock() { return gthread_rwlock_tryrdlock(&m_rwlock); }
	int wrlock() { return gthread_rwlock_wrlock(&m_rwlock); }
	int trywrlock() { return gthread_rwlock_trywrlock(&m_rwlock); }
	int unlock() { return gthread_rwlock_unlock(&m_rwlock); }
};

} // namespace gcl
