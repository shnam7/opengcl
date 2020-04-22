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

#if defined ( _MSC_VER )
#pragma warning(disable:4996)   // enable _CRT_SECURE_NO_WARNINGS
#endif


static inline void _set_epos(gtok_t *gt, const char *ppos)
{
	for (; ppos<gt->tail; ++ppos)
		if ( strchr(gt->sep, *ppos) ) break;
	gt->end = ppos;
}

gcl_api int gtok_associate(gtok_t *gt, const char *str, size_t slen, const char *sep)
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

gcl_api int gtok_set_separators(gtok_t *gt, const char *sep)
{
	if ( strlen(sep)>=sizeof(gt->sep) )
	{
	   gt->sep[0] = 0;
	   return 0;
	}
	strncpy( gt->sep, sep, MAX_SEPARATORS );
    gt->sep[MAX_SEPARATORS] = 0;
	return 1;
}

gcl_api int gtok_get_next(gtok_t *gt, char *buf, unsigned bufsize, char *sep_by, unsigned *token_len)
{
	unsigned tok_len;

	if ( gt->pos>=gt->tail ) return 0;
	tok_len = (unsigned)(gt->end - gt->pos);
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

gcl_api void gtok_rewind(gtok_t *gt)
{
	gt->pos = gt->head;
	_set_epos( gt, gt->pos );
}
