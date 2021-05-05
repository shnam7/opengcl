/*
 *	* gutil.cpp
 *
 *	OpenGCL Module : gutils - general utilities
 *
 *	Written by Soo-Hyuk Nam.
 *		2004. 04. 19. Mon.
 *
 *	History:
 *		2004/04/19: First written.
 *
 */

#include "gutil.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>


/**
 *  Convert a character into 2 bytes hex characters
 */
gcl_api void gcl::util::chr2hex(char c, char hex[2])
{
	int val = (c >> 4) & 0x0F;
	hex[0] = (char)(((val < 10) ? '0' : 'A'-10 ) + val);
	val = c & 0x0F;
	hex[1] = (char)(((val < 10) ? '0' : 'A'-10 ) + val);
}

// convert hex char to its hex value: ex)'A'-->10, 'B'-->11 etc
char gcl::util::hexval(char hexchar)
{
	hexchar = (char)toupper(hexchar);
	if ( hexchar>='0' && hexchar<='9' )
		return hexchar - '0';
	if ( hexchar>='A' && hexchar<='F' )
		return hexchar-'A' + 10;
	return 0;		// invalid hexChar
}

// convert binary data to hex string
gcl_api void gcl::util::byte2hexstr(const char *src, char *dst, int n)
{
	while ( --n >= 0 )
	{
		chr2hex( *src++, dst );
		dst += 2;
	}
}

// convert hex string to binary data
gcl_api void gcl::util::hexstr2byte(const char *src, char *dst, int n)
{
	while ( --n >= 0 )
	{
		*dst = hexval(*src++) << 4;
		if ( --n >= 0 ) *dst += hexval(*src++);
		dst++;
	}
}


// // generate time string
// static char *__strtime(char *buf, int bufsize, struct tm *ptm)
// {
// 	int n = snprintf( buf, bufsize, "%04d-%02d-%02d %02d:%02d:%02d",
// 			1900+ptm->tm_year, 1+ptm->tm_mon, ptm->tm_mday,
// 			ptm->tm_hour, ptm->tm_min, ptm->tm_sec );
// 	if ( n == -1 ) buf[bufsize-1] = '\0';
// 	return buf;
// }

// char *gcl_strtime(char *buf, int bufsize, time_t _tm)
// {
// 	return __strtime( buf, bufsize, localtime(&_tm) );
// }

// char *gcl_strgmtime(char *buf, int bufsize, time_t _tm)
// {
// 	return __strtime( buf, bufsize, gmtime(&_tm) );
// }


// char *gcl_chain_pathname(char *basepath, const char *extrapath)
// {
// 	size_t slen = strlen(basepath);
// 	if ( slen>0 && basepath[slen-1]!='/' ) basepath[slen++] = '/';
// 	if ( extrapath[0] == '/' ) ++extrapath;
// 	strcpy( basepath+slen, extrapath );
// 	return basepath;
// }
