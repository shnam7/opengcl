/*
 *  @packaage OpenGCL
 *
 *  @module gque - circular queue
 *
 */

#pragma once
#include "gcldef.h"

namespace gcl {

typedef struct _gque_t {
    unsigned begin;     // constant: start of que (end of que contro block)
    unsigned end;       // constant: end of que
    unsigned head;      // variable: que head
    unsigned tail;      // varibale: que tail
    unsigned item_size; // constant: entry item size
} gque_t;


class gcl_api gque {
protected:
    gque_t          *m_q;
    template<class T> friend class queue;

public:
    gque();
    gque(unsigned capacity, unsigned item_size);
    ~gque();

	void reset(unsigned capacity, unsigned itemSize);
	void reset() const { m_q->head = m_q->tail = m_q->begin; }

	bool put(const void *item) const;
	bool get(void *item) const { return pop(item); }
    bool push(const void *item) const;
	bool pop(void *item=0) const;
	void clear() const { m_q->head = m_q->tail; }
	void *peek() const { return isEmpty() ? 0 : _ptr(m_q->head); }
	void *peekNext(const void *current=0) const;

	//--- accessors
	void *head() const { return _ptr(m_q->head); }
	void *tail() const { return _ptr(m_q->tail); }
	void *begin() const { return _ptr(m_q->begin); }
	void *end() const { return _ptr(m_q->end); }
	unsigned itemSize() { return m_q->item_size; }

    bool isEmpty() const { return m_q->head == m_q->tail; }
    bool isFull() const;
	unsigned length() const;        // # of entries
    unsigned available() const;
    unsigned capacity() const;

protected:
    void *_ptr(unsigned offs) const { return (void *)((char *)m_q + offs); }
};


template <class T>
class queue : public gque {
public:
    queue() {}
    queue(unsigned capacity): gque(capacity, sizeof(T)) {}

    void reset(unsigned capacity) { gque::reset(capacity, sizeof(T)); };

	bool put(const T *item) const { return gque::put(item); }
	bool get(T *item) const { return gque::get(item); }
	bool push(const T *item) const { return gque::push(item); }
	bool pop(T *item) const { return gque::pop(item); }
    T *peek(T) const { return (T *)gque::peek(); }
	T *peekNext(const T *peek=0) const { return (T *)gque::peekNext(peek); }

	//--- accessors
	T *head() const { return (T *)gque::head(); }
	T *tail() const { return (T *)gque::tail(); }
	T *begin() const { return (T *)gque::begin(); }
	T *end() const { return (T *)gque::end(); }
};

}
