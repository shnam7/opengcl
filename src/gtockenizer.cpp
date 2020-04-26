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

#include "gtockenizer.h"
#include <string.h>

void gcl::gtockenizer::_findNextToken()
{
    const char *p = m_pos;
    while (p < m_tail) {
        if ( strchr(m_sep, *p) ) break;
        p++;
    }
	m_end = p;
}

gcl_api bool gcl::gtockenizer::bind(const char *str, const char *sep)
{
    return bind(str, (unsigned)strlen(str), sep);
}

gcl_api bool gcl::gtockenizer::bind(const char *str, unsigned slen, const char *sep)
{
    m_head = str;
    m_tail = (str) ? str+slen : 0;
	m_pos = str;
	if ( sep && !setSeparators(sep) ) return false;
	_findNextToken();
	return true;
}

gcl_api bool gcl::gtockenizer::setSeparators(const char *sep)
{
    unsigned slen = (unsigned)strlen(sep);
	if ( slen >= sizeof(m_sep) ) { m_sep[0] = 0; return false; }
    strncpy( m_sep, sep, sizeof(m_sep) );
    m_sep[sizeof(m_sep)-1] = 0;
	return true;
}

gcl_api bool gcl::gtockenizer::getNext(char *buf, unsigned bufSize, char *pSepBy, unsigned *pTokenLen)
{
    if ( m_pos >= m_tail ) return false;
	unsigned tokLen = (unsigned)(m_end - m_pos);
	if ( buf )
	{
		if ( bufSize <= tokLen ) return false;
		memcpy( buf, m_pos, tokLen );
		buf[tokLen] = 0;
	}
	if ( pSepBy ) *pSepBy = *m_end;
	if ( pTokenLen ) *pTokenLen = tokLen;
	m_pos = m_end + 1;
	_findNextToken();
	return 1;
}

gcl_api void gcl::gtockenizer::rewind()
{
	m_pos = m_head;
	_findNextToken();
}
