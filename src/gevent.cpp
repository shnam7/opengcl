/**
 *  @module GEvent
 *
 *  Written by Hyeon-min Lee
 *      2020. 4. 17
 */

#include "gevent.h"
#include "gutil.h"
#include <string.h>

//-----------------------------------------------------------------------------
//  class GEventEmitter
//-----------------------------------------------------------------------------
bool gcl::event_emitter::off(const char *event_name, event_handler handler)
{
    autolock_wr lock(m_lock);
    const char *name = __unregister_event_name(event_name);
    if (!name) return false;

    // erase relevant event handlers
    int len = m_event_handler_que.length();
    while (len-- > 0) {
        event_handler_block hb;
        m_event_handler_que.get(hb);
        if (hb.name==name && hb.handler==handler) continue;  // remove from the que
        m_event_handler_que.put(hb);    // put back to the que
    }
    return true;
}

bool gcl::event_emitter::emit(const char *event_name)
{
    autolock_wr lock(m_lock);
    const char *name = __find_event_name(event_name);
    if (!name) return false;

    int len = m_event_handler_que.length();
    while (len-- > 0) {
        event_handler_block hb;
        m_event_handler_que.get(hb);
        if (hb.name == name) {
            hb.handler(hb);     // call handler function
            if (hb.once) continue;  // if once, then do not put back to the quw
        }
        m_event_handler_que.put(hb);    // put back to the que
    }
    return true;
}

bool gcl::event_emitter::_on(const char *event_name, event_handler handler,
        void *data, u64_t data_ex, bool once)
{
    if (strlen(event_name) > GEVENT_NAME_LENGTH_MAXIMUM) {
        dmsg("event_emitter::_on: event_name is too long. max length=%d\n",
            GEVENT_NAME_LENGTH_MAXIMUM);
        return false;
    }

    autolock_wr lock(m_lock);
    const char *name = __register_event_name(event_name);
    if (!name) {
        dmsg("event_emitter::_on: register_event_name failed\n");
        return false;
    }

    _event_handler_block entry = { name, handler, data, data_ex, once };
    return m_event_handler_que.put(entry);
}

const char *gcl::event_emitter::__find_event_name(const char *event_name)
{
    event_name_t *peek = m_event_name_que.peek();
    while (peek) {
        if (strcmp(peek->name, event_name) == 0) return peek->name;
        peek = m_event_name_que.peek_next(peek);
    }
    return 0;
}

const char *gcl::event_emitter::__register_event_name(const char *event_name)
{
    const char *name = __find_event_name(event_name);
    if (name) return name;  // already registered

    // find empty slot first
    event_name_t *peek = m_event_name_que.peek();
    while (peek) {
        if (strlen(peek->name) == 0) {
            strncpy(peek->name, event_name, GEVENT_NAME_LENGTH_MAXIMUM);
            peek->name[GEVENT_NAME_LENGTH_MAXIMUM] = 0;
            return peek->name;
        }
        peek = m_event_name_que.peek_next(peek);
    }

    // if no empty slot, then add too queue
    event_name_t entry;
    strncpy(entry.name, event_name, GEVENT_NAME_LENGTH_MAXIMUM);
    entry.name[GEVENT_NAME_LENGTH_MAXIMUM] = 0;

    return m_event_name_que.put(entry) ? m_event_name_que.last()->name : 0;
}

const char *gcl::event_emitter::__unregister_event_name(const char *event_name)
{
    event_name_t *peek = m_event_name_que.peek();
    while (peek) {
        if (strcmp(peek->name, event_name) == 0) {
            peek->name[0] = 0;
            return peek->name;
        }
        peek = m_event_name_que.peek_next(peek);
    }
    return 0;
}
