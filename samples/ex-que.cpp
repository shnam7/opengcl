//
//  * ex-que.cpp
//
//  OpenGCL Example: queue operations
//

#include "gque.h"
#include <memory.h>
#include <stdio.h>

using namespace gcl;

typedef struct _vdata {
    unsigned dataSize;
    char buf[16];
} vdata;


void que_test()
{
    queue<int> q(10);
    printf("q status: capacity=%d entries=%d rooms=%d\n", q.capacity(), q.length(), q.available());

    int count = 0;
    while (!q.is_full()) {
        q.put(count);
        printf("%d added, capacity=%d entries=%d rooms=%d\n", count, q.capacity(), q.length(), q.available());
        ++count;
    }
    printf("--> queue is filled.\n\n");

    for (int i=0; i<5; ++i) {
        int val;
        q.pop(val);
        printf("%d poped, capacity=%d entries=%d rooms=%d\n", val, q.capacity(), q.length(), q.available());
    }
    printf("--> 5 entries were poped.\n\n");

    for (int i=0; i<5; ++i) {
        q.put(count);
        printf("%d added, capacity=%d entries=%d rooms=%d\n", count, q.capacity(), q.length(), q.available());
        count++;
    }
    printf("--> 5 entries were added again.\n\n");

    printf("*----- print queue -----*\n");
    printf("Total entries=%d\n", q.length());
    count = 0;
    int *peek = q.peek();
    while (peek) {
        printf("q[%d] = %d\n", count++, *peek);
        peek = q.peek_next(peek);
    }

    printf("*----- print queue reverse -----*\n");
    printf("Total entries=%d\n", q.length());
    count = 0;
    peek = q.last();
    while (peek) {
        printf("q[%d] = %d\n", count++, *peek);
        peek = q.peek_prev(peek);
    }
}

void step_test()
{
    //--- inititialize queue entries
    queue<int> q(10);
    for (int i=0; i<10; i++) q.put(i);      // add entries
    for (int i=0; i<5; i++) q.get();        // get half of it
    for (int i=10; i<15; i++) q.put(i);     // add again

    int index = 0;
    int *peek = q.peek();
    while (peek) {
        printf("q[%d] = %d\n", index++, *peek);
        peek = q.peek_next(peek);
    }

    unsigned count = q.length();
    printf("queue stat: length=%d available=%d cpapcity=%d\n", count, q.available(), q.capacity());

    //--- remove entries at even number positions
    printf("\nNow remove entries at even number positions...\n");
    while (count-- > 0) {
        int val;
        q.pop(val);
        if (val % 2 == 0) {
            printf("val=%d removed. queue lenght=%d\n", val, q.length());
            continue;
        }
        q.put(val);
    }

    // print queue using peek()
    index = 0;
    peek = q.head();
    while (peek) {
        printf("q[%d] = %d\n", index++, *peek);
        peek = q.peek_next(peek);
    }
}

void running_test()
{
    queue<vdata> q(30);
    vdata val;

    int loop_count = 300;
    for (int loop=1; loop<=loop_count; loop++) {
        for (int i=0; i<10; i++) {
            q.put(val);
            printf("loop=%d put=%d\n", loop, i);
        }

        for (int i=0; i<10; i++) {
            q.get(val);
            printf("loop=%d pop=%d\n", loop, i);
        }
    }
}


int main()
{
    que_test();
//     step_test();
//     running_test();
}
