/*
 *  @packaage OpenGCL
 *
 *  @module gcldefs - Common Definitions
 */

#pragma once
#include <stdint.h>

//--- gcl API
#if defined( _WIN32 ) && defined(_DLL)
#ifdef GCL_EXPORTS
#define gcl_api		__declspec(dllexport)
#else
#define gcl_api		__declspec(dllimport)
#endif
#else
#define gcl_api
#endif

//--- debug utils
#if _DEBUG
gcl_api void dmsg(const char *fmt, ...);
#else
inline void dmsg(const char *fmt,...) {}
#endif

//--- types
typedef int8_t      i8_t;
typedef int16_t     i16_t;
typedef int32_t     i32_t;
typedef int64_t     i64_t;

typedef uint8_t     u8_t;
typedef uint16_t    u16_t;
typedef uint32_t    u32_t;
typedef uint64_t    u64_t;

typedef uintptr_t   ptr_t;
