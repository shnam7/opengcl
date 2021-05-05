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

#include <pthread.h>
#include "gtime.h"
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace gcl;
using namespace gcl::time;


static int finished = 0;

void *foo(void *data)
{
    int tid = (int)(uintptr_t)data;
    printf("--- Starting: thread #%d\n", tid);
    tick_t tm0 = ticks();
    tick_t elapsed = 0;
    for (int i=0; i<100; i++)
    {
        tick_t tm = ticks();
        msleep(3);
        elapsed = elapsed_ticks(tm);
        printf("tid=%u count=%i tick=%zu elapsed=%u\n", tid, i, tm, ticks_to_msec(elapsed));
    }
    printf("--->thread #%03d finished. elapsed=%u(msec)\n", tid, elapsed_msec(tm0));

    finished++;
    return 0;
}

int main()
{
    const int TH_MAX = 512;
    pthread_t th[TH_MAX];
    for (int i=0; i<TH_MAX; i++) {
        int err = pthread_create(th+i, 0, foo, (void *)(uintptr_t)i);
        if (err) { printf("Error id=%d\n", i); return -1; }
    }

    for (int i=0; i<TH_MAX; i++) {
        pthread_join(th[i], 0);
    }

    printf("\n===> Total finished threads: %d\n", finished);
    return 0;
}
