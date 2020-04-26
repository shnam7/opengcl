/*
 *	* gutils.h
 *
 *	OpenGCL Module : gutils - general utilities
 *
 *	Written by Soo-Hyuk Nam.
 *		2004. 04. 19. Mon.
 *
 *	History:
 *		2004/04/19: Originally written.
 *
 */

#pragma once
#include "gcldef.h"

namespace gcl {

namespace util {

gcl_api void chr2hex(char c, char hex[2]);

// convert hex char to its hex value: ex)'A'-->10, 'B'-->11 etc
gcl_api char hexval(char hexchar);

// convert binary data to hex string
gcl_api void byte2hexstr(const char *src, char *dst, int n);

// convert hex string to binary data
gcl_api void hexstr2byte(const char *src, char *dst, int n);

} // namespace util

} // namespace gcl


namespace GUtil = gcl::util;
