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
 *	* gtime.h
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

#ifndef _GTIME_H
#define _GTIME_H

#if defined( _WIN32 ) && defined(_DLL)
#ifdef GCL_EXPORTS
#define gcl_api		__declspec(dllexport)
#else
#define gcl_api		__declspec(dllimport)
#endif
#else
#define gcl_api
#endif

#define GTIME_INFINITE	((unsigned long)-1)


#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned long int		gtime_t;

gcl_api gtime_t gtime_get_ticks();				/* returns current time in msec */
gcl_api gtime_t gtime_get_diff(gtime_t tm1, gtime_t tm2);	/* returns elapsed time since tm */

static inline gtime_t gtime_get_elapsed(gtime_t tm)			/* returns elapsed time since tm */
	{ return gtime_get_diff(tm, gtime_get_ticks()); }

/* thread-aware sleep function: The calling thread will be awakened if it is canceled. */
gcl_api void gtime_msleep(gtime_t msec);


#ifdef __cplusplus
};
#endif

#endif
