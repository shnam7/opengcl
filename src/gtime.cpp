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

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <pthread.h>

using namespace gcl;
using namespace gcl::time;

gcl_api tick_t gcl::time::ticks()
{
    LARGE_INTEGER ticks;
    return QueryPerformanceCounter(&ticks) ? (tick_t)ticks.QuadPart : 0;
}

gcl_api u64_t gcl::time::ticks_to_nsec(tick_t ticks)
{
    LARGE_INTEGER pf;
    return QueryPerformanceFrequency(&pf) ? (ticks * 1000000000)/pf.QuadPart : 0;
}

gcl_api u32_t gcl::time::ticks_to_usec(tick_t ticks)
{
    LARGE_INTEGER pf;
    return QueryPerformanceFrequency(&pf) ? (u32_t)((ticks * 1000000)/pf.QuadPart) : 0;
}

gcl_api u32_t gcl::time::ticks_to_msec(tick_t ticks)
{
    LARGE_INTEGER pf;
    return QueryPerformanceFrequency(&pf) ? (u32_t)((ticks * 1000)/pf.QuadPart) : 0;
}


gcl_api tick_t gcl::time::nsec_to_ticks(u64_t nsec)
{
    LARGE_INTEGER pf;
    return QueryPerformanceFrequency(&pf) ? (nsec * pf.QuadPart) / 1000000000 : 0;
}

gcl_api tick_t gcl::time::usec_to_ticks(u32_t usec)
{
    LARGE_INTEGER pf;
    return QueryPerformanceFrequency(&pf) ? (usec * pf.QuadPart) / 1000000 : 0;
}

gcl_api tick_t gcl::time::msec_to_ticks(u32_t msec)
{
    LARGE_INTEGER pf;
    return QueryPerformanceFrequency(&pf) ? (msec * pf.QuadPart) / 1000 : 0;
}


#else
#include <time.h>

gcl_api tick_t gcl::time::ticks()
{
    timespec tm;
    clock_gettime(CLOCK_MONOTONIC, &tm);
    return tm.tv_sec * 1000000000 + tm.tv_nsec;
}

#endif
