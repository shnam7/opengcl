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

#if defined( _WIN32 ) && defined(_DLL)
#ifdef GCL_EXPORTS
#define gcl_api		__declspec(dllexport)
#else
#define gcl_api		__declspec(dllimport)
#endif
#else
#define gcl_api
#endif

#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif


gcl_api void gcl_chr2hex(char c, char hex[2]);

// convert hex char to its hex value: ex)'A'-->10, 'B'-->11 etc
gcl_api char gcl_hexval(char hexchar);

// convert binary data to hex string
gcl_api void gcl_byte2hexstr(const char *src, char *dst, int n);

// convert hex string to binary data
gcl_api void gcl_hexstr2byte(const char *src, char *dst, int n);

// generate time string
gcl_api char *gcl_strtime(char *buf, int bufsize, time_t _tm);
gcl_api char *gcl_strgmtime(char *buf, int bufsize, time_t _tm);

gcl_api char *gcl_chain_pathname(char *basepath, const char *extrapath);

#ifdef __cplusplus
}
#endif
