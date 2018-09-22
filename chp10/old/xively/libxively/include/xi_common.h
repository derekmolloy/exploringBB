// Copyright (c) 2003-2014, LogMeIn, Inc. All rights reserved.
// This is part of Xively C library, it is under the BSD 3-Clause license.

#ifndef __XI_COMMON_H__
#define __XI_COMMON_H__

#include <stdlib.h>
#include <inttypes.h>

#include "xi_debug.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    char*           data_ptr;
    unsigned short  data_size;
    unsigned short  real_size;
    unsigned short  curr_pos;
} data_descriptor_t;

typedef struct
{    
    const char*     data_ptr;
    unsigned short  data_size;
    unsigned short  real_size;
    unsigned short  curr_pos;
} const_data_descriptor_t;

#ifdef __cplusplus
}
#endif

#endif // __XI_COMMON_H__
