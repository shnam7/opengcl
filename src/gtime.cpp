/*
 *	* gutil.cpp
 *
 *	OpenGCL Module : gutils - general utilities
 *
 *	Written by Soo-Hyuk Nam.
 *		2004. 04. 19. Mon.
 *
 *	History:
 *		2004/04/19: First written.
 *
 */

#include "gtime.h"

using namespace gcl;
using namespace gcl::time;

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <pthread.h>

static int __hpc = 0;   // flag: high precision counter (-1: no hpc, 0: hpc need init, 1: hpc init done)
static LARGE_INTEGER __pf = { 0, 0 };   // cache for performance frequency
inline nsec_t _ticks2nsec(tick_t ticks) { return (ticks * 1000000000)/__pf.QuadPart; }
// inline tick_t _nsec2ticks(tick_t nsec) { return (nsec * __pf.QuadPart) / 1000000000; }

// return tick count as nsec
gcl_api tick_t gcl::time::ticks()
{
    LARGE_INTEGER ticks;
    if (__hpc > 0) {
        QueryPerformanceCounter(&ticks);
        return _ticks2nsec(ticks.QuadPart);
    }
    if (__hpc == 0) {
        __hpc = QueryPerformanceFrequency(&__pf) ? 1 : -1;
        if (__hpc > 0) {
            QueryPerformanceCounter(&ticks);
            return _ticks2nsec(ticks.QuadPart);
        }
    }
    return ((nsec_t)GetTickCount()) * 1000000;    // return ticks in msec (no hpc available)
}


#else
#include <time.h>

gcl_api tick_t gcl::time::ticks()
{
    // returns nanosec time as tick count
    timespec tm;
    clock_gettime(CLOCK_MONOTONIC, &tm);
    return tm.tv_sec * 1000000000 + tm.tv_nsec;
}

#endif
