/*
 *  @packaage OpenGCL
 *
 *  @module list -singly and doubly linked list
 *
 */

#include "glist.h"

gcl_api unsigned gcl::_node::length()
{
    unsigned count = 0;
    _node *nod = m_next;
    while (nod->m_next != m_next) {
        nod = nod->m_next;
        count++;
    }
    return count;
}
