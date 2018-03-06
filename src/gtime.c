/*
***************************************************************************
* This File is a part of OpenGCL.
* Copyright (c) 2004 Soo-Hyuk Nam (shnam7@gmail.com)
* 
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU Library General Public License
* as published by the Free Software Foundation: either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Library General Public License for more details.
*
* Should this software be used in another application, an acknowledgement
* that OpenGCL code is used would be appreciated, but it is not mandatory.
*
***************************************************************************
*/

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
