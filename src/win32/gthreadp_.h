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

#ifndef __GTHREADP__H
#define __GTHREADP__H

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

typedef struct ___gthread_tcb {
	HANDLE			h_thread;
	HANDLE			h_cancel;		/* cancel notify to wake blocked thread */
	int				canceled;		/* cancel signal */
	list_head		join_node;
	list_head		join_list;
	DWORD			join_retval;
} __gthread_tcb;

#ifdef __cplusplus
extern "C" {
#endif

gcl_api void __gthread_msleep(unsigned long msec);

#ifdef __cplusplus
}
#endif


#endif