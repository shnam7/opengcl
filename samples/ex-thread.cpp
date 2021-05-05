//
//		* exthread.c
//
//		OpenGCL Example: gthread operations
//
//		Written by Soo-Hyuk Nam.
//			2003. 4. 29 Tue.
//
//
//
//

#include "gthread.h"
#include "gtime.h"
#include <stdio.h>

using namespace gcl;
using namespace gcl::time;

int finished = 0;

//--- thread function
void *foo(void *data)
{
    thread *th = thread::get_current();
    unsigned tid = th->thread_id();

    tick_t tm0 = ticks();
    tick_t elapsed = 0;
    for (int i=0; i<100; i++)
    {
        tick_t tm = ticks();
        msleep(1);
        elapsed = elapsed_ticks(tm);
        printf("tid=#%05d count=%i tick=%zu elapsed=%lu\n", tid, i, tm, ticks_to_msec(elapsed));
    }
    printf("--->thread #%05d finished. elapsed=%umsec\n", tid, elapsed_msec(tm0));
    finished++;
    return 0;
}

//--- thread class
class MyThread : public thread {
protected:
    unsigned    m_time_to_sleep;

public:
    MyThread(unsigned time_to_sleep=0) : m_time_to_sleep(time_to_sleep)
    {
        printf("MyThread is crteated.\n");
        start();
    }

    ~MyThread()
    {
        printf("MyThread::Destructor called.\n");
    }

    void *run() {
        unsigned tid = thread::get_current()->thread_id();
        printf("MyThread #%d started, and gong to sleep for %d msec.\n", tid, m_time_to_sleep);
        msleep(m_time_to_sleep);
        printf("MyThread #%d is awake.\n", m_tid);

        printf("MyThread #%d is now deleting its own object instance.\n", m_tid);
        delete this;
        return 0;
    }
};


void running_test()
{
    const int TH_MAX = 512;
    thread th[TH_MAX];
    for (int i = 0; i < TH_MAX; i++)
        th[i].start(foo);

    // check if main thread is running
    for (int i=0; i<100; i++) {
        printf(">>> main thread id=%d count=%d\n", thread::get_current()->thread_id(), i);
        msleep(3);
    }

    for (int i = 0; i < TH_MAX; i++)
        th[i].join();

    printf("End of main --- Total %d threads finished\n", finished);
}

void cancel_test()
{
    printf("main thread is creating an instance of MyThread...\n");
    thread *t1 = new MyThread(1000);    // this instance will be deleted by its own thread
    printf("main thread going to sleep...\n");
    msleep(500);
    printf("main thread now awake, and cancelling sleeping thread\n");

    thread *tmain = thread::get_current();
    dmsg("main: tid=%d\n", tmain->thread_id());
    dmsg("main: waiting for the child thread to finish (joining to the child).\n\n");
    t1->join();
    dmsg("main: now child thread is finished.\n");
    dmsg("main: cancelling the main thread...\n");
    pthread_cancel(pthread_self());
    msleep(0);      // call one of cancellation points
    dmsg("main thread is cancelled and this SHOULD NOT be printed!\n");
}


int main() {
    running_test();
    cancel_test();

    return 0;
}
