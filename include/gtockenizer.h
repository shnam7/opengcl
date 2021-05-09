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

class gcl_api Tockenizer {
protected:
    const char		*m_head = 0;      // head of target string
	const char		*m_tail = 0;      // end of target string
	const char		*m_pos = 0;       // current token position
	const char		*m_end = 0;       // end of currnet token
	char	        m_sep[MAX_SEPSARATORS+1];  // list of separator characters as string

public:
	Tockenizer() { m_sep[0] = 0; }
	Tockenizer(const char *str, const char *sep) { bind(str, sep); }
	Tockenizer(const char *str, int slen, const char *sep) { bind(str, slen, sep); }
	~Tockenizer() {}

	//--- manipulatorss
	bool bind(const char *str, const char *sep=0);
	bool bind(const char *str, unsigned slen, const char *sep=0);
	bool set_separators(const char *sep);
	bool get_next(char *buf, unsigned bufSize, char *pSepBy=0, unsigned *pTokenLen=0);
	void rewind();

    //--- accessors
	bool has_more_tokens() const { return m_pos < m_tail; }
	unsigned  get_next_token_length() const { return (unsigned)(m_end - m_pos); }
	const char *get_target_string() const { return m_head; }
    unsigned get_target_ttring_length() const { return (unsigned)(m_tail - m_head); }
    const char *get_separators() const { return m_sep; }

protected:
    void _find_next_token();
};

} // namespace gcl
