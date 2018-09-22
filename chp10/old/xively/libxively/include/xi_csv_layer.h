// Copyright (c) 2003-2014, LogMeIn, Inc. All rights reserved.
// This is part of Xively C library, it is under the BSD 3-Clause license.

#ifndef __XI_CSV_LAYER_H__
#define __XI_CSV_LAYER_H__

#include "xi_layer.h"
#include "xi_common.h"
#include "xively.h"
#include "xi_csv_layer_data.h"

#ifdef __cplusplus
extern "C" {
#endif

layer_state_t csv_layer_data_ready(
      layer_connectivity_t* context
    , const void* data
    , const layer_hint_t hint );

layer_state_t csv_layer_on_data_ready(
      layer_connectivity_t* context
    , const void* data
    , const layer_hint_t hint );

layer_state_t csv_layer_close(
    layer_connectivity_t* context );

layer_state_t csv_layer_on_close(
    layer_connectivity_t* context );

layer_state_t csv_layer_parse_datastream(
        csv_layer_data_t* csv_layer_data
      , const_data_descriptor_t* data
      , const layer_hint_t hint
      , xi_datapoint_t* dp );

layer_state_t csv_layer_parse_feed(
        csv_layer_data_t* csv_layer_data
      , const_data_descriptor_t* data
      , const layer_hint_t hint
      , xi_feed_t* dp );

const void* csv_layer_data_generator_datastream_get(
          const void* input
        , short* state );

const void* csv_layer_data_generator_datapoint(
          const void* input
        , short* state );

#ifdef __cplusplus
}
#endif

#endif // __XI_CSV_LAYER_H__
