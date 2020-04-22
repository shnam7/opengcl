/*
 *	* gtime.c
 *
 *	OpenGCL Module : Generic Time-related Services
 *
 *	Written by Soo-Hyuk Nam.
 *		2004. 7. 1. Tue.
 *
 *	History:
 *		2003/07/01: First written.
 *
 *	Notes:
 *		- gtime_t uses milli-sec unit and its value wraps around to zero after 49.7 days.
 *		- gtime64_t uses nano sec unit and its value wraps around to zero after 884.9 yesrs(infinit?)
 *			--> not supported yat. Is it necessary?
 */

#include "gtime.h"
#include "gdmsg.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "win32/gthreadp_.h"
#else
#include <sys/time.h>
#include <time.h>
#endif


gtime_t gtime_get_ticks()
{
#ifdef _WIN32
	LARGE_INTEGER pf, pc;
	if ( !QueryPerformanceFrequency(&pf) ) return GetTickCount();
	QueryPerformanceCounter( &pc );
	return (gtime_t)( pc.QuadPart*1000 / pf.QuadPart );
#else
	struct timeval tv;
	gettimeofday( &tv, 0 );
	return tv.tv_sec*1000 + (tv.tv_usec+500)/1000;
#endif
}

gtime_t gtime_get_diff(gtime_t tm1, gtime_t tm2)
{
	return tm2>=tm1 ? tm2-tm1 : 0xFFFFFFFFUL-(tm1-tm2);
}

void gtime_msleep(gtime_t msec)
{
#ifdef _WIN32
	__gthread_msleep( msec );
#else
	struct timespec ts = { msec/1000, (msec%1000)*1000000 };
	nanosleep( &ts, 0 );
#endif
}
