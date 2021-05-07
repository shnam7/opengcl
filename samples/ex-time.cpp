//
//		* extime.cpp
//
//		OpenGCL Example: gtime operations
//
//		Written by Soo-Hyuk Nam.
//			2003. 7. 1. Tue.
//

#include "gtime.h"
#include <stdio.h>

using namespace gcl;

void precision_test()
{
    nsec_t time_to_wait = 1;
    for (int i=1; i<=9; i++) {
        time_to_wait *= 10;

        tick_t tm1 = ticks();
        nsleep(time_to_wait);
        nsec_t elapsed = elapsed_nsec(tm1);
        nsec_t error = elapsed - time_to_wait;

        printf("try[%02d] %lf(msec) actual:%zu(ns) %u(us) %u(ms) --> gap: %zu(ns), %zu(us) %zu(ms) [%.3lf%%]\n",
            i,
            ((double)time_to_wait)/1000000,
            ticks_to_nsec(elapsed), ticks_to_usec(elapsed), ticks_to_msec(elapsed),
            error, error/1000, error/1000000,
            ((double)error) / time_to_wait * 100
        );
    }
}

int main()
{
    precision_test();
    return 0;
}
