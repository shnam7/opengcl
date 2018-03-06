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
 *	* gdmsg.h
 *
 *	OpenGCL Module : Generic Debug Message Interface
 *
 *	Written by Soo-hyuk Nam.
 *		2003. 7. 12. Sat.
 *
 *	History:
 *		2003/07/12: First written.
 */

#ifndef __GDMSG_H
#define __GDMSG_H


#if _DEBUG
#define _DMSG	1

#include <stdio.h>
#include <stdarg.h>
static inline void dmsg(const char *fmt, ...)
{
	va_list va; va_start( va, fmt );
	vprintf( fmt, va );
	va_end(va);
}
#else
static inline void dmsg(const char *fmt,...) {}
#endif

#endif
