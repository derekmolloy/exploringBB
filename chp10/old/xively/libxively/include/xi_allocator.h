// Copyright (c) 2003-2014, LogMeIn, Inc. All rights reserved.
// This is part of Xively C library, it is under the BSD 3-Clause license.

#ifndef __XI_ALLOCATOR_H__
#define __XI_ALLOCATOR_H__

#include "xi_config.h"

#ifdef __cplusplus
extern "C" {
#endif

static inline void* xi_alloc( size_t b );

static inline void xi_free( void* p );

#ifdef __cplusplus
}
#endif

#if (!defined(XI_ALLOCATOR)) || (XI_ALLOCATOR == 0)
#include "xi_allocator_stdlib.h"
#elif XI_ALLOCATOR == 1
#include "xi_allocator_wmsdk.h"
#endif

#endif // __XI_ALLOCATOR_H__
