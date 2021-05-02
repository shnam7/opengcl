#include "gthread.h"

#ifdef _WIN32
#include <windows.h>

#else
#include <semaphore.h>
#define ERROR_INVALID_HANDLE EINVAL
#include <errno.h>
#endif
#include <semaphore.h>

using gcl::runnable;
using gcl::thread;

//--------------------------------------------------------------------
//	class thread
//--------------------------------------------------------------------
struct __thread_wrapper_data {
    gcl::thread         *instance;
    gcl::runnable       *pRunnable;
    void                *(*proc)(void *);
    void                *data;
    sem_t               *sem;   // signaling completion of thread creation
};

static struct __thread_wrapper_data __thread_wrapper_args;

pthread_key_t   gcl::thread::__key_to_instance;
unsigned        gcl::thread::__instance_count = 0;
thread          __main_thread;

void *gcl::thread::__thread_wrapper(void *data)
{
    struct __thread_wrapper_data args = *(struct __thread_wrapper_data *)data;
    pthread_setspecific(__key_to_instance, args.instance);
    args.instance->m_tid = ++__instance_count;
    sem_post(args.sem);
    // dmsg("---thread #%d started\n", __instance_count);

    if (args.pRunnable) return args.pRunnable->run();
    if (args.proc) return args.proc(args.data);
    return 0;
}

void gcl::thread::__thread_destructor(void *data) {

    // dmsg("---thread #%d terminated\n", ((gcl::thread *)data)->m_tid);
    ((gcl::thread *)data)->m_tid = (unsigned)(-1);
    if (--__instance_count == 0) pthread_key_delete(__key_to_instance);
}

gcl::thread::thread() {
    if (__instance_count == 0) {
        pthread_key_create(&__key_to_instance, __thread_destructor);
        __main_thread.m_t = pthread_self();
        __main_thread.m_tid = 0;
        int err = pthread_setspecific(__key_to_instance, &__main_thread);
    }
}

gcl::thread::~thread() {
    stop();
}

gcl_api int gcl::thread::start(void *(*proc)(void *), void *data, pthread_attr_t *attr)
{
    if (is_running()) return EINVAL;

    sem_t sem;
    sem_init(&sem, 0, 0);
    struct __thread_wrapper_data args = { this, 0, proc, data, &sem };
    int err = pthread_create(&m_t, attr, __thread_wrapper, &args);
    if (!err) sem_wait(&sem);
    return err;
}

gcl_api int gcl::thread::start(runnable *runnable, pthread_attr_t *attr)
{
    if (is_running()) return EINVAL;

    sem_t sem;
    sem_init(&sem, 0, 0);
    struct __thread_wrapper_data args = { this, runnable, 0, 0, &sem };
    int err = pthread_create(&m_t, attr, __thread_wrapper, &args);
    if (!err) sem_wait(&sem);
    return err;
}

gcl_api int gcl::thread::stop(void *retval)
{
    if (is_running()) {
        // if stoppinig other thread, then cancel it
        thread *current = get_current();
        if (current != this) {
            int err = pthread_cancel(m_t);
            if (!err) err = pthread_join(m_t, &retval);
            return err;
        }

        // if stopping current thread, then exit if it's not main thread
        if (current != &__main_thread) pthread_exit(retval);
    }
    return 0;
}

gcl::thread *gcl::thread::get_current()
{
    return (gcl::thread *)pthread_getspecific(__key_to_instance);
}
