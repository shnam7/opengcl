/*
 *  @packaage OpenGCL
 *
 *  @module tockenizer - String tockenizer *
 */

#pragma once
#include "gcldef.h"
#include <string.h>

namespace gcl {

#define MAX_SEPSARATORS		31	/* actually, null take */

class gcl_api tockenizer {
protected:
    const char		*m_head = 0;      // head of target string
	const char		*m_tail = 0;      // end of target string
	const char		*m_pos = 0;       // current token position
	const char		*m_end = 0;       // end of currnet token
	char	        m_sep[MAX_SEPSARATORS+1];  // list of separator characters as string

public:
	tockenizer() { m_sep[0] = 0; }
	tockenizer(const char *str, const char *sep) { bind(str, sep); }
	tockenizer(const char *str, int slen, const char *sep) { bind(str, slen, sep); }
	~tockenizer() {}

	//--- manipulatorss
	bool bind(const char *str, const char *sep=0);
	bool bind(const char *str, unsigned slen, const char *sep=0);
	bool setSeparators(const char *sep);
	bool getNext(char *buf, unsigned bufSize, char *pSepBy=0, unsigned *pTokenLen=0);
	void rewind();

    //--- accessors
	bool hasMoreTokens() const { return m_pos < m_tail; }
	unsigned  getNextTokenLength() const { return (unsigned)(m_end - m_pos); }
	const char *getTargetString() const { return m_head; }
    unsigned getTargetStringLength() const { return (unsigned)(m_tail - m_head); }
    const char *getSeparators() const { return m_sep; }

protected:
    void _findNextToken();
};

} // namespace gcl
