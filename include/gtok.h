/*
 *	* gtokenizer.h
 *
 *	OpenGCL Module : Generic String Tokenizer
 *
 *	Written by Soo-Hyuk Nam.
 *		2001. 8. 2. Fri.
 *
 *	History:
 *		2001/08/02: Originally written.
 */

#pragma once

#include <string.h>

#if defined( _WIN32 ) && defined(_DLL)
#ifdef GCL_EXPORTS
#define gcl_api		__declspec(dllexport)
#else
#define gcl_api		__declspec(dllimport)
#endif
#else
#define gcl_api
#endif

#define MAX_SEPARATORS		31	/* actually, null take */

typedef struct _gtok {
	const char		*head;		 /* head of target string */
	const char		*tail;		 /* end of target string */
	const char		*pos;		 /* current token position */
	const char		*end;		 /* end of currnet token */
	char			sep[MAX_SEPARATORS+1];	/* separators, 1 is for null */
} gtok_t;

#ifdef __cplusplus
extern "C" {
#endif

/*
 * str: target string to tokenize
 * slen: valid length of str
 * sep: separators, this should no be null
 */
gcl_api int gtok_associate(gtok_t *gt, const char *str, size_t slen, const char *sep);
gcl_api int gtok_set_separators(gtok_t *gt, const char *sep);

/*
 * buf<out>: receives current token
 * sep_by<out>: receives separator following the current token(buf)
 * token_len<out>: receives current token length
 *
 * Note: buf, sep_by, token_len can be null.
 *		In this case, just data is not written to them.
 */
gcl_api int gtok_get_next(gtok_t *gt, char *buf, unsigned bufsize, char *sep_by, unsigned *token_len);
gcl_api void gtok_rewind(gtok_t *gt);

inline int gtok_has_more_tokens(const gtok_t *gt)
{
   return gt->pos<gt->tail;
}

inline size_t gtok_get_next_token_length(const gtok_t *gt)
{
	return gt->end - gt->pos;
}


#ifdef __cplusplus
};


/*--------------------------------------------------------------------*
 *	class _GTokenizer
 *--------------------------------------------------------------------*/
class gcl_api _GTokenizer {
protected:
	gtok_t			m_gt;

public:
	_GTokenizer() { memset(&m_gt, 0, sizeof(m_gt)); }
	_GTokenizer(const char *str, const char *sep)
		{ gtok_associate(&m_gt, str, strlen(str), sep); }
	_GTokenizer(const char *str, int slen, const char *sep)
		{ gtok_associate(&m_gt, str, slen, sep); }
	~_GTokenizer() {}

	//--- manipulators
	bool associate(const char *str, const char *sep=0)
		{ return gtok_associate(&m_gt, str, strlen(str), sep) != 0; }
	bool associate(const char *str, int slen, const char *sep=0)
		{ return gtok_associate(&m_gt, str, slen, sep) != 0; }
	bool setSeparators(const char *sep)
		{ return gtok_set_separators(&m_gt, sep) != 0; }
	bool getNext(char *buf, unsigned bufsize, char *sep_by=0, unsigned *token_len=0)
		{ return gtok_get_next(&m_gt, buf, bufsize, sep_by, token_len) != 0; }
	void rewind() { gtok_rewind(&m_gt); }

	//--- accessors
	bool hasMoreTokens() const { return gtok_has_more_tokens(&m_gt) != 0; }
	size_t getNextTokenLength() const { return gtok_get_next_token_length(&m_gt); }
	const char *getTargetString() const { return m_gt.head; }
	size_t getTargetStringLength() const { return m_gt.tail-m_gt.head; }
	const char *getSeparators() const { return m_gt.sep; }
};


#ifdef GTOK_SUPPORT_STL
/*
 *	When STL enabled, all the STL functions are inlined to bypass
 *	the WIN32 DLL linkage problem.
 *	The following class hierarchy is also for that.
 */
#include <string>

/*--------------------------------------------------------------------*
 *	class GTokenizer
 *--------------------------------------------------------------------*/
class GTokenizer : public _GTokenizer {
public:
	GTokenizer() {};
	GTokenizer(const char *str, const char *sep)
		: _GTokenizer(str, sep) {}
	GTokenizer(const char *str, int slen, const char *sep)
		: _GTokenizer(str, slen, sep) {}

	bool getNext(char *buf, unsigned bufsize, char *sep_by=0, unsigned *token_len=0)
		{ return _GTokenizer::getNext(buf, bufsize, sep_by, token_len); }
	bool getNext(std::string& ss, char *sep_by=0)
	{
		if ( m_gt.pos>=m_gt.tail ) return false;
		ss.assign( m_gt.pos, m_gt.end-m_gt.pos );
		return _GTokenizer::getNext( 0, 0, sep_by );
	}
};

#else

typedef _GTokenizer		GTokenizer;

#endif

#endif
