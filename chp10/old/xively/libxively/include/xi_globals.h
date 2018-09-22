// Copyright (c) 2003-2014, LogMeIn, Inc. All rights reserved.
// This is part of Xively C library, it is under the BSD 3-Clause license.

#ifndef __XI_GLOBALS_H__
#define __XI_GLOBALS_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// This struct is used for run-time config
typedef struct
{
    uint32_t network_timeout;
} xi_globals_t;

extern xi_globals_t xi_globals;

#ifdef __cplusplus
}
#endif

#endif // __XI_GLOBALS_H__
