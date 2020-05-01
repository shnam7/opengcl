//
//		* exque.cpp
//
//		OpenGCL Example: queue operations
//
//		Written by Soo-Hyuk Nam.
//			2001. 1. 9. Tue.
//

#include "gque.h"
#include <memory.h>
#include <stdio.h>

typedef struct _vdata {
    unsigned dataSize;
    char buf[16];
} vdata;

using IQue = gcl::que<int>;
using VQue = gcl::que<vdata>;


void gque_test()
{
    IQue gq(10);

    printf("GQue: capacity=%d entries=%d rooms=%d\n",
           gq.capacity(), gq.length(), gq.available());

    int count = 0;
    while (!gq.isFull()) {
        gq.put(&count);
        printf("%d added, capacity=%d entries=%d rooms=%d\n",
               count, gq.capacity(), gq.length(), gq.available());
        ++count;
    }
    printf("--> GQue filled.\n\n");

    int i;
    for (i = 0; i < 5; ++i) {
        int val;
        gq.get(&val);
        printf("%d poped, capacity=%d entries=%d rooms=%d\n",
               val, gq.capacity(), gq.length(), gq.available());
    }
    printf("--> 5 entries were poped.\n\n");

    for (i = 0; i < 5; ++i) {
        gq.put(&count);
        printf("%d added, capacity=%d entries=%d rooms=%d\n",
               count, gq.capacity(), gq.length(), gq.available());
        count++;
    }
    printf("--> 5 entries were added again.\n\n");

    printf("*----- Full list -----*\n");
    count = 0;
    unsigned entries = gq.length();
    printf("entries=%d\n", entries);
    int *pItem = gq.peekNext();
    while (pItem) {
        printf("m_q[%d]=%d\n", count++, *pItem);
        pItem = gq.peekNext(pItem);
    }
}


void gque_running_test()
{
    VQue gq(30);
    vdata val;

    int loopCount = 30;
    while (loopCount-- > 0) {
        for (int i = 0; i < 10; i++) {
            gq.put(&val);
            printf("loop=%d put=%d\n", loopCount, i);
        }

        for (int i = 0; i < 10; i++) {
            gq.put(&val);
            printf("loop=%d pop=%d\n", loopCount, i);
        }
    }
}

void gque_step_test()
{
    GQue q(10, sizeof(int));
    for (int i=0; i<10; i++) q.put(&i);
    for (int i=0; i<5; i++) q.get();
    for (int i=10; i<15; i++) q.put(&i);


    unsigned count = q.length();
    dmsg("Count=%d ===\n", count);
    while (count-- > 0) {
        int val;
        q.pop(&val);
        if (val % 2 == 0) {
            printf("val=%d removed. len=%d\n", val, q.length());
            continue;
        }
        q.put(&val);
    }

    int *p = (int *)q.peek();
    dmsg("Que length=%d\n", q.length());
    void *marker = q.tail();
    int index = 0;
    while (p) {
        printf("index=%i odd val=%d\n", index++, *p);
        p = (int *)q.peekNext(p);
    }
}

int main()
{
    gque_step_test();
    // gque_test();
    // gque_running_test();
}
