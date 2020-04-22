/*
 *	* gthread.h
 *
 *	OpenGCL Module : gthread private header
 *
 *	Written by Soo-Hyuk Nam.
 *		2004. 1. 15. Thu.
 *
 *	History:
 *		2004/01/15: Originally written.
 *
 *	Notes:
 *		- This module was separated from gthread.h to provide
 *		  light weight thread-aware sleep function - gtime_msleep.
 *		  Refer to gtime.c for implementation details.
 *		- gthread_msleep() implementation resides in gthread_self.c
 */

#pragma once

#include "glist.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


#if defined( _WIN32 ) && defined(_DLL)
#ifdef GCL_EXPORTS
#define gcl_api		__declspec(dllexport)
#else
#define gcl_api		__declspec(dllimport)
#endif
#else
#define gcl_api
#endif

typedef struct {
	HANDLE			h_thread;
	HANDLE			h_cancel;		/* cancel notify to wake blocked thread */
	int				canceled;		/* cancel signal */
	list_head		join_node;
	list_head		join_list;
	uintptr_t		join_retval;
} __gthread_tcb;

#ifdef __cplusplus
extern "C" {
#endif

gcl_api void __gthread_msleep(unsigned long msec);

#ifdef __cplusplus
}
#endif
