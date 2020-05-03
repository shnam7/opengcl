/*
 *  @packaage OpenGCL
 *
 *  @module time - time functions
 */
#include "gtimer.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <time.h>
#endif

gcl_api nanotick_t gcl::gtimer::nanoTicks() {
#ifdef _WIN32
	LARGE_INTEGER pf, pc;
	if ( !QueryPerformanceFrequency(&pf) ) return GetTickCount() * 1000;
	QueryPerformanceCounter( &pc );
    return (*(u64_t *)&pc * 1000000) / *(u64_t *)&pf;
#else
    timespec tm;
    clock_gettime(CLOCK_MONOTONIC, &tm);
    return tm.tv_sec * 1000000000 + tm.tv_nsec;
#endif
}
