/*
 *  @packaage OpenGCL
 *
 *  @module time - time functions
 */

#pragma once
#include "gcldef.h"

namespace gcl {

typedef u32_t       tick_t;
typedef u64_t       nanotick_t;

namespace chrono {

// get time elapsed since system start in nanosec
gcl_api nanotick_t nanoTicks();
inline nanotick_t nanoElapsed(nanotick_t tm) { return nanoTicks() - tm; }

// get time elapsed since system start in msec
inline tick_t ticks() { return (tick_t)(nanoTicks() / 1000); };
inline tick_t elapsed(tick_t tm) { return ticks() - tm; }

} // namespace chrono

} // namespace gcl
