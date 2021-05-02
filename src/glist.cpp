/*
 *  @packaage OpenGCL
 *
 *  @module list -singly and doubly linked list
 *
 */

#include "glist.h"

using gcl::linkable;

gcl_api void gcl::linkable::append(linkable *node)
{
    if (!node->is_alone()) node->detach();
    node->m_next = m_next;
    node->m_prev = this;
    m_next->m_prev = node;
    m_next = node;
}

gcl_api unsigned gcl::linkable::length() const
{
    unsigned count = 0;     // does not count myself
    linkable *node = m_next;
    while (node != this) {
        node = node->m_next;
        count++;
    }
    return count;
}
