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
 *	* gtok.c
 *
 *	OpenGCL Module : Generic String Tokenizer
 *
 *	Written by Soo-Hyuk Nam.
 *		2001. 8. 2. Fri.
 *
 *	History:
 *		2001/08/02: Originally written.
 */

#include "gtok.h"
#include <string.h>

static inline void _set_epos(gtok_t *gt, const char *ppos)
{
	for (; ppos<gt->tail; ++ppos)
		if ( strchr(gt->sep, *ppos) ) break;
	gt->end = ppos;
}

int gtok_associate(gtok_t *gt, const char *str, size_t slen, const char *sep)
{
	if ( str==0 && sep==0 )
	{
		memset( gt, 0, sizeof(*gt) );
		return 1;
	}
	gt->head = str;
	gt->tail = (str) ? str+slen : 0;
	gt->pos = str;
	if ( sep && !gtok_set_separators(gt, sep) ) return 0;
	_set_epos(gt, gt->pos);
	return 1;
}

int gtok_set_separators(gtok_t *gt, const char *sep)
{
	if ( strlen(sep)>=sizeof(gt->sep) )
	{
	   gt->sep[0] = 0;
	   return 0;
	}
	strcpy( gt->sep, sep );
	return 1;
}

int gtok_get_next(gtok_t *gt, char *buf, int bufsize, char *sep_by, int *token_len)
{
	int tok_len;

	if ( gt->pos>=gt->tail ) return 0;
	tok_len = gt->end - gt->pos;
	if ( buf )
	{
		if ( bufsize <= tok_len ) return 0;
		memcpy( buf, gt->pos, tok_len );
		buf[tok_len] = '\0';
	}
	if ( sep_by ) *sep_by = *gt->end;
	if ( token_len ) *token_len = tok_len;
	gt->pos = gt->end+1;
	_set_epos( gt, gt->pos );
	return 1;
}

void gtok_rewind(gtok_t *gt)
{
	gt->pos = gt->head;
	_set_epos( gt, gt->pos );
}
