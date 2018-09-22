// Copyright (c) 2003-2014, LogMeIn, Inc. All rights reserved.
// This is part of Xively C library, it is under the BSD 3-Clause license.

#ifndef __XI_HTTP_LAYER_H__
#define __XI_HTTP_LAYER_H__

#include "xi_layer.h"
#include "xi_http_layer_input.h"

#ifdef __cplusplus
extern "C" {
#endif

layer_state_t http_layer_data_ready(
      layer_connectivity_t* context
    , const void* data
    , const layer_hint_t hint );

layer_state_t http_layer_on_data_ready(
      layer_connectivity_t* context
    , const void* data
    , const layer_hint_t hint );

layer_state_t http_layer_close(
      layer_connectivity_t* context );

layer_state_t http_layer_on_close(
      layer_connectivity_t* context );

layer_t* init_http_layer(
      layer_t* layer );

const void* http_layer_data_generator_datastream_get(
      const void* input
    , short* state );

#ifdef __cplusplus
}
#endif

#endif // __XI_HTTP_LAYER_H__
