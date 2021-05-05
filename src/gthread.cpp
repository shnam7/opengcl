#include "gthread.h"
#include <semaphore.h>

#ifdef _WIN32
#include <windows.h>

#else
#include <semaphore.h>
#define ERROR_INVALID_HANDLE EINVAL
#include <errno.h>
#endif

using gcl::runnable;
using gcl::thread;

struct __thread_wrapper_data {
    thread         *instance;
    runnable       *pRunnable;
    void                *(*proc)(void *);
    void                *data;
    sem_t               *sem;   // signaling completion of thread creation
};

static pthread_key_t   __key_to_instance;
static unsigned        __instance_count = 0;
static thread          __main_thread;

void thread::__thread_destructor(void *data) {

    // dmsg("---thread #%d terminated\n", ((thread *)data)->m_tid);
    ((thread *)data)->m_tid = (unsigned)(-1);
    if (--__instance_count == 0) {
        pthread_key_delete(__key_to_instance);
        __key_to_instance = 0;
    }
}

void *thread::__thread_wrapper(void *data)
{
    struct __thread_wrapper_data args = *(struct __thread_wrapper_data *)data;
    pthread_setspecific(__key_to_instance, args.instance);
    args.instance->m_tid = ++__instance_count;
    sem_post(args.sem);
    // dmsg("---thread #%d started\n", __instance_count);

    pthread_cleanup_push(__thread_destructor, args.instance);
    data = args.pRunnable ? args.pRunnable->run() : args.proc ? args.proc(args.data) : 0;
    pthread_testcancel(); // call one of cancellation points to support slef-cancelling (cancel by current thread)
    pthread_cleanup_pop(1);
    return data;

}


//--------------------------------------------------------------------
//	class thread
//--------------------------------------------------------------------
thread::thread() {
    // if (__instance_count == 0
    if (__instance_count == 0) {
        pthread_key_create(&__key_to_instance, 0);
        // pthread_key_create(&__key_to_instance, __thread_destructor);
        __main_thread.m_t = pthread_self();
        __main_thread.m_tid = 0;
        pthread_setspecific(__key_to_instance, &__main_thread);
    }
}

thread::~thread() {
    stop();
}

gcl_api int thread::start(void *(*proc)(void *), void *data, pthread_attr_t *attr)
{
    if (is_running()) return EINVAL;

    sem_t sem;
    sem_init(&sem, 0, 0);
    struct __thread_wrapper_data args = { this, 0, proc, data, &sem };
    int err = pthread_create(&m_t, attr, __thread_wrapper, &args);
    if (!err) sem_wait(&sem);
    return err;
}

gcl_api int thread::start(runnable *runnable, pthread_attr_t *attr)
{
    if (is_running()) return EINVAL;

    sem_t sem;
    sem_init(&sem, 0, 0);
    struct __thread_wrapper_data args = { this, runnable, 0, 0, &sem };
    int err = pthread_create(&m_t, attr, __thread_wrapper, &args);
    if (!err) sem_wait(&sem);
    return err;
}

gcl_api int thread::stop(void *retval)
{
    if (is_running()) {
        // if stoppinig other thread, then cancel it
        thread *current = get_current();
        if (current != this) {
            // dmsg("EQUAL=%d %p %p\n", pthread_equal(pthread_self(), m_t), current, this);
            int err = pthread_cancel(m_t);
            if (!err) err = pthread_join(m_t, &retval);
            return err;
        }

        // if stoppinig current thread, cancel it and do not join
        // if (current != &__main_thread) --> main thread can also cencel itself
        int oldstat;
        pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &oldstat);    // POSIX.1 could require oldstat param
        pthread_cancel(m_t);
    }
    return 0;
}

thread *thread::get_current()
{
    // if __instance_count==0, __key_to_instance is invalid
    return __instance_count==0 ? &__main_thread
        : (thread *)pthread_getspecific(__key_to_instance);
}

unsigned thread::get_thread_counts()
{
    return __instance_count;
}
