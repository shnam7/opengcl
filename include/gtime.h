/*
 *  @packaage OpenGCL
 *
 *  @module time - time functions
 */

#pragma once
#include "gcldef.h"
#ifndef _WIN32
#include <unistd.h>     // include for sleep()
#endif

namespace gcl
{

typedef uint64_t tick_t;   // timer tick count type
typedef uint64_t nsec_t;   // nanosec time type
typedef unsigned usec_t;   // microsec time type
typedef unsigned msec_t;   // millisec time type

//--- get time elapsed since system start in nanosec
gcl_api tick_t ticks();

inline nsec_t ticks_to_nsec(tick_t ticks) { return (nsec_t)ticks; }
inline usec_t ticks_to_usec(tick_t ticks) { return (usec_t)(ticks_to_nsec(ticks) / 1000); }
inline msec_t ticks_to_msec(tick_t ticks) { return (msec_t)(ticks_to_nsec(ticks) / 1000000); }

inline tick_t nsec_to_ticks(nsec_t nsec) { return (tick_t)nsec; }
inline tick_t usec_to_ticks(usec_t usec) { return nsec_to_ticks(((nsec_t)usec) * 1000); }
inline tick_t msec_to_ticks(msec_t msec) { return nsec_to_ticks(((nsec_t)msec) * 1000000); }

inline tick_t elapsed_ticks(tick_t tm) { return ticks() - tm; }
inline nsec_t elapsed_nsec(tick_t tm) { return ticks_to_nsec(elapsed_ticks(tm)); }
inline usec_t elapsed_usec(tick_t tm) { return ticks_to_usec(elapsed_ticks(tm)); }
inline msec_t elapsed_msec(tick_t tm) { return ticks_to_msec(elapsed_ticks(tm)); }

gcl_api void nanosleep(nsec_t nsec);
inline void nsleep(nsec_t nsec) { nanosleep(nsec); }
inline void usleep(usec_t usec) { nanosleep(((nsec_t)usec) * 1000); }
inline void msleep(msec_t msec) { nanosleep(((nsec_t)msec) * 1000000); }
#ifdef _WIN32
//--- thread-aware sleep functions
inline void sleep(unsigned sec) { nanosleep(((usec_t)sec) * 1000000000); }
#endif

} // namespace gcl
