/*
 *	* gmbox.h
 *
 *	OpenGCL Module : gmbox - General purpose message box
 *
 *	Written by Soo-Hyuk Nam.
 *		2004. 4. 21. Wed.
 *
 *	History:
 *		2004/04/21: First Written.
 *
 */

#pragma once
#include <stdint.h>

#if defined( _WIN32 ) && defined(_DLL)
#ifdef GCL_EXPORTS
#define gcl_api		__declspec(dllexport)
#else
#define gcl_api		__declspec(dllimport)
#endif
#else
#define gcl_api
#endif


typedef uintptr_t		gmbox_t;
typedef uintptr_t		gmboxv_t;

#ifdef __cplusplus
extern "C" {
#endif

/* mbox for fixed size messages */
gcl_api gmbox_t gmbox_create(unsigned msg_size, unsigned que_size, int use_lock);

gcl_api int gmbox_delete(gmbox_t hmb);					/* return type is bool */

gcl_api int gmbox_put(gmbox_t hmb, const void *msg, unsigned long timeout_msec);    /* return type is bool */

gcl_api int gmbox_get(gmbox_t hmb, void *buf);			/* return type is bool */

gcl_api int gmbox_peek(gmbox_t hmb, void *buf);			/* return type is bool */


/* mbox for variable size messages */
gcl_api gmboxv_t gmboxv_create(unsigned mbox_size, int use_lock);

gcl_api int gmboxv_delete(gmboxv_t hmb);				/* return type is bool */

gcl_api int gmboxv_put(gmboxv_t hmb, const void *msg,	/* return type is bool */
		unsigned msg_size, unsigned long timeout_msec);

gcl_api int gmboxv_get(gmboxv_t hmb, void *buf,			/* return type is bool */
		int bufsize,int *msg_size);

gcl_api int gmboxv_peek(gmboxv_t hmb, void *buf,		/* return type is bool */
		int bufsize, int *msg_size);

#ifdef __cplusplus
};


//--------------------------------------------------------------------
//	class GMBox
//--------------------------------------------------------------------
class GMBox {
protected:
	gmbox_t		m_mb;

public:
	GMBox() : m_mb(0) {}
	~GMBox() { if (m_mb) destroy(); }

	bool create(unsigned msg_size, unsigned que_size, bool use_lock)
	{
		if ( m_mb ) destroy();
		m_mb = gmbox_create(msg_size, que_size, use_lock);
		return m_mb != 0;
	}

	bool destroy()
	{
		int r = gmbox_delete(m_mb);
		if ( r ) m_mb = 0;
		return r != 0;
	}

	bool put(const void *msg, unsigned long msec)
		{ return gmbox_put(m_mb, msg, msec) != 0; }

	bool get(void *msg) { return gmbox_get(m_mb, msg) != 0; }

	//--- accessors
	gmbox_t getHandle() const { return m_mb; }
};


//--------------------------------------------------------------------
//	class GMBoxv
//--------------------------------------------------------------------
class GMBoxv {
protected:
	gmboxv_t		m_mb;

public:
	GMBoxv() : m_mb(0) {}
	~GMBoxv() { if (m_mb) destroy(); }

	bool create(unsigned mbox_size, bool use_lock)
	{
		if ( m_mb ) destroy();
		m_mb = gmboxv_create(mbox_size, use_lock);
		return m_mb != 0;
	}

	bool destroy()
	{
		int r = gmboxv_delete(m_mb);
		if ( r ) m_mb = 0;
		return r != 0;
	}

	bool put(const void *msg, int msg_size, unsigned long msec)
		{ return gmboxv_put(m_mb, msg, msg_size, msec) != 0; }

	bool get(void *buf, int bufsize, int *msg_len=0)
		{ return gmboxv_get(m_mb, buf, bufsize, msg_len) != 0; }

	//--- accessors
	gmboxv_t getHandle() const { return m_mb; }
};

#endif
