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
 *	* gthread_self.c
 *
 *	OpenGCL Module : gthread - WIN32 support for thread-aware sleep.
 *
 *	Written by Soo-Hyuk Nam.
 *		2004. 1. 15. Thu.
 *
 *	History:
 *		2004/01/15: Originally written.
 */

#include "gthread.h"
#include "gthreadp_.h"

gthread_key_t	_gkey_self    = GTHREAD_INVALID_KEY;

void __gthread_msleep(unsigned long msec)
{
	__gthread_tcb *tcb = (__gthread_tcb *)TlsGetValue(_gkey_self);
	if ( tcb == 0 ) { Sleep(msec); return; }
	WaitForSingleObject( tcb->h_cancel, msec );
}
