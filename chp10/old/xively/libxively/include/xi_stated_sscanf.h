// Copyright (c) 2003-2014, LogMeIn, Inc. All rights reserved.
// This is part of Xively C library, it is under the BSD 3-Clause license.

#ifndef __XI_STATED_SSCANF_H__
#define __XI_STATED_SSCANF_H__

#include "xi_stated_sscanf_state.h"
#include "xi_common.h"

#ifdef __cplusplus
extern "C" {
#endif

signed char xi_stated_sscanf(
          xi_stated_sscanf_state_t* s
        , const const_data_descriptor_t* pattern
        , const_data_descriptor_t* source
        , void** variables );

#ifdef __cplusplus
}
#endif

#endif // __XI_STATED_SSCANF_H__
