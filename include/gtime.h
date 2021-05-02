/*
 *  @packaage OpenGCL
 *
 *  @module time - time functions
 */

#pragma once
#include "gcldef.h"

namespace gcl
{

namespace time
{

typedef uint64_t tick_t;   // timer tick count type

//--- get time elapsed since system start in nanosec
gcl_api tick_t ticks();

gcl_api u64_t ticks_to_nsec(tick_t ticks);
gcl_api u32_t ticks_to_usec(tick_t ticks);
gcl_api u32_t ticks_to_msec(tick_t ticks);

gcl_api tick_t nsec_to_ticks(u64_t nsec);
gcl_api tick_t usec_to_ticks(u32_t usec);
gcl_api tick_t msec_to_ticks(u32_t msec);

inline tick_t elapsed_ticks(tick_t tm) { return ticks() - tm; }
inline u64_t elapsed_nsec(tick_t tm) { return ticks_to_nsec(elapsed_ticks(tm)); }
inline u32_t elapsed_usec(tick_t tm) { return ticks_to_usec(elapsed_ticks(tm)); }
inline u32_t elapsed_msec(tick_t tm) { return ticks_to_msec(elapsed_ticks(tm)); }

//--- thread-aware sleep functions
gcl_api void nanosleep(u64_t nsec);
inline void usleep(u32_t usec) { nanosleep(((u64_t)usec) * 1000); }
inline void sleep(u32_t msec) { nanosleep(((u64_t)msec) * 1000000); }

} // namespace timer

} // namespace gcl
