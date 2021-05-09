/*
 *  @packaage OpenGCL
 *
 *  @module List -singly and doubly linked list
 *
 */

#include "glist.h"

using namespace gcl;

gcl_api void Linkable::append(Linkable *node)
{
    if (!node->is_alone()) node->detach();
    node->m_next = this;
    node->m_prev = this->m_prev;
    m_prev->m_next = node;
    m_prev = node;
}

gcl_api unsigned Linkable::length() const
{
    unsigned count = 0;     // does not count myself
    Linkable *node = m_next;
    while (node != this) {
        node = node->m_next;
        count++;
    }
    return count;
}
