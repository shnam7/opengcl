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

void conversion_test()
{
    printf("\nconversion test -----\n");
    tick_t tm = ticks();
    printf("ticks=%zu ns=%zu us=%zu ms=%zu\n", tm, t2n(tm), t2u(tm), t2m(tm));

    nsec_t ns = t2n(tm);
    printf("ticks=%zu ns=%zu us=%zu ms=%zu\n", n2t(ns), ns, n2u(ns), n2m(ns));

    usec_t us = t2u(tm);
    printf("ticks=%zu ns=%zu us=%zu ms=%zu\n", usec_to_ticks(us), u2n(us), us, u2m(us));

    msec_t ms = t2m(tm);
    printf("ticks=%zu ns=%zu us=%zu ms=%zu\n", m2t(ms), m2n(ms), m2u(ms), ms);
}

void precision_test()
{
    printf("\nprecision test -----\n");
    nsec_t time_to_wait = 1;
    for (int i=1; i<=9; i++) {
        time_to_wait *= 10;

        tick_t tm1 = ticks();
        nsleep(time_to_wait);
        nsec_t elapsed = elapsed_nsec(tm1);
        nsec_t error = elapsed - time_to_wait;

        printf("try[%02d] %lf(msec) actual:%zu(ns) %zu(us) %zu(ms) --> gap: %zu(ns), %zu(us) %zu(ms) [%.3lf%%]\n",
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
    conversion_test();
    precision_test();
    return 0;
}
