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

using namespace gcl::time;

void precision_test()
{
    nsec_t time_to_wait = 1;
    for (int i=1; i<=9; i++) {
        time_to_wait *= 10;

        tick_t tm1 = ticks();
        nsleep(time_to_wait);
        nsec_t elapsed = elapsed_nsec(tm1);
        nsec_t error = elapsed - time_to_wait;

        printf("try[%02d] %lf(msec) actual:%zu(ns) %ld(us) %ld(ms)",
            i,
            ((long double)time_to_wait)/1000000,
            ticks_to_nsec(elapsed), ticks_to_usec(elapsed), ticks_to_msec(elapsed)
        );
        printf(" --> gap: %zu(ns), %zu(us) %zu(ms) [%.3lf%%]\n",
            error, error/1000, error/1000000,
            ((long double)error) / time_to_wait * 100
        );

        printf("try[%02d] %lf(msec) actual:%llu(ns) %lu(us) %lu(ms) --> gap: %llu(ns), %llu(us) %llu(ms) [%.3lf%%]\n",
            i,
            ((long double)time_to_wait)/1000000,
            ticks_to_nsec(elapsed), ticks_to_usec(elapsed), ticks_to_msec(elapsed),
            error, error/1000, error/1000000,
            ((long double)error) / time_to_wait * 100
        );

        // printf("try[%02d] %lf(msec) actual:%zu(ns) %ld(us) %ld(ms) --> gap: %u(ns), s()\n", //, %d(ms) [%.3lf%%]\n",
        //     i,
        //     ((long double)time_to_wait)/1000000,
        //     ticks_to_nsec(elapsed), ticks_to_usec(elapsed), ticks_to_msec(elapsed),
        //     error+1,
        //     "11111.0" // error/1000,
        //     // error/1000000,
        //     // ((long double)error) / time_to_wait * 100
        // );
    }
}

int main()
{
    precision_test();
    return 0;
}
