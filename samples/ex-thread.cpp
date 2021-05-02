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

void *foo(void *data)
{
    thread *th = thread::get_current();
    unsigned tid = th->thread_id();

    tick_t tm0 = ticks();
    tick_t elapsed = 0;
    for (int i=0; i<100; i++)
    {
        tick_t tm = ticks();
        sleep(3);
        elapsed = elapsed_ticks(tm);
        printf("tid=#%05d count=%i tick=%zu elapsed=%lu\n", tid, i, tm, ticks_to_msec(elapsed));
    }
    printf("--->thread #%05d finished. elapsed=%umsec\n", tid, elapsed_msec(tm0));
    finished++;
    return 0;
}

int main()
{
    const int TH_MAX = 512;
    thread th[TH_MAX];
    for (int i = 0; i < TH_MAX; i++)
        th[i].start(foo);

    // check if main thread is running
    for (int i=0; i<100; i++) {
        printf(">>> main thread id=%d count=%d\n", thread::get_current()->thread_id(), i);
        sleep(3);
    }

    for (int i = 0; i < TH_MAX; i++)
        th[i].join();

    printf("End of main --- Total %d threads finished\n", finished);
    return 0;
}
