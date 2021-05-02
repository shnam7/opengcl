/*
 *  @packaage OpenGCL
 *
 *  @module time - time functions
 */
#include "gtime.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <pthread.h>

gcl_api void gcl::time::nanosleep(u64_t nsec) {
    tick_t tm = ticks();
    tick_t duration = nsec_to_ticks(nsec);

    // To save cpu time, Sleep() can be used. But, it's low resolution shows around 3ms deviation.
    // So, it is not used here.
    //
    // DWORD msec = nsec / 1000000;
    // nsec %= 1000000;
    // if (msec > 0) Sleep(msec);

    while (elapsed_ticks(tm) <= duration) {
        Sleep(0);
        pthread_testcancel();
    }
}


#else
#include <time.h>
gcl_api void gcl::time::nanosleep(u64_t nsec)
{
    struct timespec ts = {(long)(nsec / 1000000000L), (long)((nsec % 1000000000))};
    ::nanosleep(&ts, 0);
}

#endif
