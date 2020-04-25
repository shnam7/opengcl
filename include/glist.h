/*
 *  @packaage OpenGCL
 *
 *  @module list -singly and doubly linked list
 *
 */

#pragma once

#include "gcldef.h"

namespace gcl {

//-----------------------------------------------------------------------------
// doublely linked list
//-----------------------------------------------------------------------------
class _node {
protected:
    _node       *m_prev = this;
    _node       *m_next = this;
    template<typename T> friend class list;

public:
    _node() {}
    _node(_node *prev, _node *next) : m_prev(prev), m_next(next) {}
    ~_node() { detach(); }

    void append(_node *nod) { nod->m_next = m_next; nod->m_prev = this; m_next->m_prev = nod; m_next = nod; }
    void prepend(_node *nod) { m_prev->append(nod); }
    void detach() { m_prev->m_next = m_next; m_next->m_prev = m_prev; }
    bool isDetached() { return m_next == this; }
    unsigned length();  // count the length of the list it's atached
};


template <class T = void>
class list {
public:
    typedef _node   node;

protected:
    _node           m_head;

public:
    list() {}
    ~list() { while (m_head.m_next) del((T *)m_head.m_next); }

    void add(T *nod) { append(nod); }
    void remove(T *node) { node->detach(); }
    void del(T *node) { node->detach(); delete node; }

    void append(T *nod) { m_head.prepend(nod); }
    void prepend(T *nod) { m_head.append(nod); }

    T *first() { return (T *)(m_head.m_next == &m_head ? 0 : m_head.m_next); }
    T *last() { return (T *)(m_head.m_prev == &m_head ? 0 : m_head.m_prev); }
    T *nextOf(T *nod = 0) {
        if (!nod) { nod = (T *)&m_head; }
        return (T *)((nod->m_next == (T *)&m_head) ? 0 : nod->m_next);
    }

    unsigned length() { return m_head.length(); }
    bool isEmpty() { return m_head.isDetached(); }
};

typedef list<> glist;



//-----------------------------------------------------------------------------
// singly linked list
//-----------------------------------------------------------------------------
class snode {
protected:
    snode    *m_next = 0;
    friend class gslist;
    template<class T> friend class slist;

public:
    snode() {}
};


class gcl_api gslist {
protected:
    snode           *m_head = 0;

public:
    gslist() : m_head(0) {}
    void append(snode *node);
    void detach(snode *node);
    snode *last();
};


template <typename T = void>
class slist : public gslist {
public:
    typedef snode   node;

public:
    slist() {}
    ~slist() { while (m_head) del((T *)m_head); }

    void add(T *node) { prepend(node); }
    void remove(T *node) { gslist::detach((snode *)node); delete node; }
    void del(T *node) { remove(node); delete node; }

    void append(T *node) { gslist::append((snode *)node); }
    void prepend(T *node) { node->m_next = m_head; m_head = node; }

    T *first() { return (T *)m_head; }
    T *last() { return (T *)gslist::last(); }
    T *nextOf(T *nod) { return (T *)(nod ? nod->m_next : 0); }

    unsigned length()
    {
        unsigned count = 0;
        snode *nod = m_head;
        while (nod) { count++; nod = nod->m_next; }
        return count;
    }
    bool isEmpty() { return m_head == 0; }
};

} // namespace gcl
