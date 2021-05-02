/*
 *  @packaage OpenGCL
 *
 *  @module event - event and event emittor
 */

#pragma once
#include "glist.h"
#include "gque.h"

#ifndef GEVENT_NAME_LENGTH_MAXIMUM
#define GEVENT_NAME_LENGTH_MAXIMUM      15
#endif

namespace gcl {

//-----------------------------------------------------------------------------
//  class event_emitter
//-----------------------------------------------------------------------------
class gcl_api event_emitter {
public:
    struct _event_handler_block;
    typedef struct _event_handler_block event_handler_block;
    typedef void (*event_handler)(event_handler_block&);

    struct _event_handler_block {
        const char      *name;
        event_handler   handler;
        void            *data;
        u64_t           data_ex;        // extra data;
        bool            once;
    };

    typedef event_handler_block event;   // for external use

protected:
    typedef struct _event_name_t {
        char name[GEVENT_NAME_LENGTH_MAXIMUM + 1];
    } event_name_t;

    queue<event_name_t>             m_event_name_que;
    queue<event_handler_block>      m_event_handler_que;

public:
    event_emitter(unsigned max_event_types=20, unsigned event_queue_size=30)
        : m_event_name_que(max_event_types), m_event_handler_que(event_queue_size) {}
    ~event_emitter() {}

    bool on(const char *event_name, event_handler handler, void *data=0, u64_t data_ex=0)
        { return _on(event_name, handler, data, data_ex); }

    bool off(const char *event_name, event_handler handler);

    bool once(const char *event_name, event_handler handler, void *data=0, u64_t data_ex=0)
        { return _on(event_name, handler, data, data_ex, true); }

    bool emit(const char *event_name);

protected:
    bool _on(const char *event_name, event_handler handler, void *data=0,
            u64_t data_ex=0, bool once=false);

    const char *_find_event_name(const char *event_name);
    const char *_register_event_name(const char *event_name);
    const char *_unregister_event_name(const char *event_name);
};


} // namespace gcl
