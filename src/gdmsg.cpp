/*
 *	* gdmsg.cpp
 *
 *	OpenGCL Module : Generic Debug Message Interface
 *
 *	Written by Soo-hyuk Nam.
 *		2003. 7. 12. Sat.
 *
 *	History:
 *		2003/07/12: First written.
 */

#include "gcldef.h"

#ifdef _DEBUG

#include <stdio.h>
#include <stdarg.h>
gcl_api void gcl::dmsg(const char *fmt, ...)
{
	va_list va; va_start( va, fmt );
	vprintf( fmt, va );
	va_end(va);
}

#endif
