/*
 *  @packaage OpenGCL
 *
 *  @module list -singly and doubly linked list
 *
 */

#include "glist.h"

unsigned gcl::_node::length()
{
    unsigned count = 0;
    _node *nod = m_next;
    while (nod->m_next != m_next) {
        nod = nod->m_next;
        count++;
    }
    return count;
}

//-----------------------------------------------------------------------------
// GSList::Core
//-----------------------------------------------------------------------------
// gcl_api void gcl::gslist::append(snode *node)
// {
//     if (!m_head) {
//         node->m_next = m_head; m_head = node;
//         return;
//     }

//     snode *p = m_head;
//     while (p->m_next) p = p->m_next;
//     p->m_next = node;
// }

// gcl_api void gcl::gslist::detach(snode *node) {
//     if (!node) return;
//     if (node == m_head) { m_head = m_head->m_next; return; }

//     snode *p = m_head;
//     while (p && p->m_next != node) p = p->m_next;
//     if (p) p->m_next = node->m_next;
// }

// gcl_api gcl::snode *gcl::gslist::last() {
//     snode *p = m_head;
//     while (p->m_next) p = p->m_next;
//     return p;
// }
