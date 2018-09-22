// Copyright (c) 2003-2014, LogMeIn, Inc. All rights reserved.
// This is part of Xively C library, it is under the BSD 3-Clause license.

#ifndef __XI_LAYER_HELPERS_H__
#define __XI_LAYER_HELPERS_H__

#include "xi_common.h"

#ifdef __cplusplus
extern "C" {
#endif

// little helper to encapsulate error handling
static inline layer_state_t layer_sender(
          layer_connectivity_t* context
        , const char* const data
        , const layer_hint_t hint )
{
    const int len = strlen( (data) );
    const const_data_descriptor_t tmp_data = { (data), len, len, 0 };
    return CALL_ON_PREV_DATA_READY( context->self, ( const void* ) &tmp_data, hint );
}

// wrapper around layer_sender() to add some syntactic sugar
#define send_through( context, data, hint ) \
    { \
        layer_state_t ret = LAYER_STATE_OK; \
        if( ( ret = layer_sender( context, data, hint ) ) != LAYER_STATE_OK ) { return ret; } \
    }

#ifdef __cplusplus
}
#endif

#endif // __XI_LAYER_HELPERS_H__
