/*
 *	OpenGCL Module : gthread - WIN32 support for thread-aware sleep.
 *
 *
 *	Written by Soo-Hyuk Nam.
 *		2004. 1. 15. Thu.
 *
 *	History:
 *		2004/01/15: Originally written.
 */

#include "_gthread.h"

gthread_key_t	_gkey_self    = GTHREAD_INVALID_KEY;
