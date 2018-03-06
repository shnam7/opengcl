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
 *	* gutils.c
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

#include "gutils.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#if defined ( _WIN32 )
#define snprintf		_snprintf
#endif

void gcl_chr2hex(char c, char hex[2])
{
	int val = (c >> 4) & 0x0F;
	hex[0] = ((val < 10) ? '0' : 'A'-10 ) + val;
	val = c & 0x0F;
	hex[1] = ((val < 10) ? '0' : 'A'-10 ) + val;
}

// convert hex char to its hex value: ex)'A'-->10, 'B'-->11 etc
char gcl_hexval(char hexchar)
{
	hexchar = toupper(hexchar);
	if ( hexchar>='0' && hexchar<='9' )
		return hexchar - '0';
	if ( hexchar>='A' && hexchar<='F' )
		return hexchar-'A' + 10;
	return 0;		// invalid hexChar
}

// convert binary data to hex string
void gcl_byte2hexstr(const char *src, char *dst, int n)
{
	while ( --n >= 0 )
	{
		gcl_chr2hex( *src++, dst );
		dst += 2;
	}
}

// convert hex string to binary data
void gcl_hexstr2byte(const char *src, char *dst, int n)
{
	while ( --n >= 0 )
	{
		*dst = gcl_hexval(*src++) << 4;
		if ( --n >= 0 ) *dst += gcl_hexval(*src++);
		dst++;
	}
}

// generate time string
static char *_strtime(char *buf, int bufsize, struct tm *ptm)
{
	int n = snprintf( buf, bufsize, "%04d-%02d-%02d %02d:%02d:%02d",
			1900+ptm->tm_year, 1+ptm->tm_mon, ptm->tm_mday,
			ptm->tm_hour, ptm->tm_min, ptm->tm_sec );
	if ( n == -1 ) buf[bufsize-1] = '\0';
	return buf;
}

char *gcl_strtime(char *buf, int bufsize, time_t _tm)
{
	return _strtime( buf, bufsize, localtime(&_tm) );
}

char *gcl_strgmtime(char *buf, int bufsize, time_t _tm)
{
	return _strtime( buf, bufsize, gmtime(&_tm) );
}


char *gcl_chain_pathname(char *basepath, const char *extrapath)
{
	int slen = strlen(basepath);
	if ( slen>0 && basepath[slen-1]!='/' ) basepath[slen++] = '/';
	if ( extrapath[0] == '/' ) ++extrapath;
	strcpy( basepath+slen, extrapath );
	return basepath;
}
