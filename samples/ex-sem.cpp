//
//		* exthread_x.cpp
//
//		OpenGCL Example: gcl::thread operations
//
//		Written by Soo-Hyuk Nam.
//			2003. 4. 29 Tue.
//

/* refer to notes in gthread.h and /usr/include/features.h */

#include "gcl.h"
#include <stdio.h>

using namespace gcl;
using namespace gcl::time;

semaphore sem(2);

void *foo(void *)
{
    int val = 0;
    unsigned tid = thread::get_current()->thread_id();

    printf("\nthread #%03u started. waiting for sem...\n", tid);
    sem.wait();
    sem.get_value(&val);
    printf("thread #%03u aquired sem. sem value=%d\n", tid, val);

    msleep(100);

    sem.post();
    sem.get_value(&val);
    printf("thread #%03u released sem, sem value=%d\n", tid, val);
    return (void *)0x12345;
}

int main()
{
    int val = 0;
    printf("Starting...\n");
    sem.get_value(&val);
    printf("--- initial sem value=%d\n", val);

    const int TH_MAX = 10;
    thread th[TH_MAX];
    for (int i=0; i<TH_MAX; i++)
        th[i].start(foo);

    sem.wait();
    sem.get_value(&val);
    printf("\nmain: acquired sem. sem val=%d.\n", val);

    msleep(1000);
    sem.post();
    printf("main: sem released\n");

    for (int i=0; i<TH_MAX; i++) th[i].join();

    sem.get_value(&val);
    printf("--- finall sem value=%d\n", val);
    return 0;
}
