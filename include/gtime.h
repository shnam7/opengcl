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

#pragma once

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
