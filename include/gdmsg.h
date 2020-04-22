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

#pragma once

#if _DEBUG
#define _DMSG	1

#ifdef __cplusplus
extern "C" {
#endif

void dmsg(const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#else
inline void dmsg(const char *fmt,...) {}
#endif
