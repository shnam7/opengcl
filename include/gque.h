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
 *	* gque.h
 *
 *	OpenGCL Module : Generic Queue with extraordinary features.
 *
 *	Written by Soo-Hyuk Nam.
 *		2003. 11. 26. Wed.
 *
 *	History:
 *		2003/11/26: First written.
 *		2003/08/25: Prototype designed(IoQue).
 */

#ifndef __GQUE_H
#define __GQUE_H

#include <stddef.h>

#if defined( _WIN32 ) && defined(_DLL)
#ifdef GCL_EXPORTS
#define gcl_api		__declspec(dllexport)
#else
#define gcl_api		__declspec(dllimport)
#endif
#else
#define gcl_api
#endif

/*--------------------------------------------------------------------
 *	GQue: the type independent queue
 *--------------------------------------------------------------------*/
typedef struct _qque_t {
	size_t		begin;
	size_t		end;
	size_t		head;
	size_t		tail;
	size_t		item_size;
	size_t		remainder;
	void		*opt;		/* optional param, usally for locking */
} gque_t;


#ifdef __cplusplus
extern "C" {
#endif

gcl_api gque_t *gque_create(size_t item_size, size_t capacity);

gcl_api int gque_delete(gque_t *gq);

static inline size_t gque_calc_bufsize(size_t item_size, size_t capacity)
{
	return sizeof(gque_t)+(capacity+1)*item_size;
}

static inline size_t gque_calc_capacity(size_t item_size, size_t bufsize)
{
	return (bufsize-sizeof(gque_t))/item_size - 1;
}

gcl_api gque_t *gque_init(size_t item_size, size_t capacity, char *buf);

static inline gque_t *gque_init2(size_t item_size, char *buf, size_t bufsize)
	{ return gque_init(item_size, gque_calc_capacity(item_size, bufsize), buf); }

static inline void gque_reset(gque_t *gq)
	{ gq->head = gq->tail = gq->begin; }

static inline char *gque_begin(gque_t *gq)
	{ return ((char *)gq) + gq->begin; }

static inline char *gque_end(gque_t *gq)
	{ return ((char *)gq) + gq->end; }

static inline char *gque_head(gque_t *gq)
	{ return ((char *)gq) + gq->head; }

static inline char *gque_tail(gque_t *gq)
	{ return ((char *)gq) + gq->tail; }

static inline int gque_item_size(gque_t *gq)
	{ return gq->item_size; }

static inline size_t gque_remainder(gque_t *gq)
	{ return gq->remainder; }


gcl_api int gque_push_tail(gque_t *gq, const char *item);	// return type is bool

gcl_api int gque_pop_head(gque_t *gq);				// return type is bool

static inline void gque_pop_all(gque_t *gq)
{
	gq->head = gq->tail;
}

gcl_api int gque_shift_head(gque_t *gq, int n);	// return type is bool

gcl_api int gque_shift_tail(gque_t *gq, int n);	// return type is bool

static inline int gque_is_empty(gque_t *gq)		// return type is bool
{
	return gq->head==gq->tail;
}

gcl_api int gque_is_full(gque_t *gq);		// return type is bool

gcl_api size_t gque_get_entries(gque_t *gq);

gcl_api size_t gque_get_linear_entries(gque_t *gq);

gcl_api size_t gque_get_rooms(gque_t *gq);

gcl_api size_t gque_get_linear_rooms(gque_t *gq);

static inline size_t gque_get_capacity(gque_t *gq)
{
	return ((gq->end-gq->begin)/gq->item_size) - 1;
}

gcl_api int gque_request_linear_rooms(gque_t *gq,
		size_t size);

#ifdef __cplusplus
};


//--------------------------------------------------------------------
//	class GQue
//--------------------------------------------------------------------
class gcl_api GQue {
protected:
	gque_t		*m_q;

public:
	GQue() : m_q(0) {}
	~GQue() { destroy(); }

	//--- manipulators
	bool create(size_t item_size, size_t capacity)
	{
		if (m_q) gque_delete(m_q);
		m_q=gque_create(item_size, capacity);
		return m_q != 0;
	}
	bool create(size_t item_size, size_t capacity, char *qbuf)
	{
		if (m_q) gque_delete(m_q);
		m_q = gque_init(item_size, capacity, qbuf);
		return m_q != 0;
	}
	bool create(size_t item_size, char *qbuf, size_t bufsize)
	{
		if (m_q) gque_delete(m_q);
		m_q = gque_init2(item_size, qbuf, bufsize);
		return m_q != 0;
	}
	bool destroy() { if (m_q) { gque_delete(m_q); m_q=0; } return true; }

	void reset() const { gque_reset(m_q); }
	bool pushTail(const char *item) const
		{ return gque_push_tail(m_q, item) != 0; }
	bool popHead() const { return gque_pop_head(m_q) != 0; }
	void popAll() const {  gque_pop_all(m_q); }
	bool shiftHead(int n) { return gque_shift_head(m_q, n) != 0; }
	bool shiftTail(int n) { return gque_shift_tail(m_q, n) != 0; }
	bool requestLinearRooms(size_t size)
		{ return gque_request_linear_rooms(m_q, size) != 0; }
	bool attach(gque_t *gq)
	{
		if ( !gq ) return false;
		if ( gq==m_q ) return true;
		if (m_q) destroy();
		m_q=gq;
		return true;
	}
	bool detach() { m_q=0; return true; }

	//--- accessors
	char *head() const { return gque_head(m_q); }
	char *tail() const { return gque_tail(m_q); }
	char *begin() const { return gque_begin(m_q); }
	char *end() const { return gque_end(m_q); }
	size_t itemSize() { return gque_item_size(m_q); }
	size_t remainder() { return gque_remainder(m_q); }

	bool isEmpty() const { return gque_is_empty(m_q) != 0; }
	bool isFull() const { return gque_is_full(m_q) != 0; }
	size_t getEntries() const { return gque_get_entries(m_q); }
	size_t getLinearEntries() const { return gque_get_linear_entries(m_q); }
	size_t getRooms() const { return gque_get_rooms(m_q); }
	size_t getLinearRooms() const { return gque_get_linear_rooms(m_q); }
	size_t getCapacity() const { return gque_get_capacity(m_q); }

	bool isValid() const { return m_q != 0; }
	gque_t *getQue() { return m_q; }
};

#endif

#endif
