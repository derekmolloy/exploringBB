// Copyright (c) 2003-2014, LogMeIn, Inc. All rights reserved.
// This is part of Xively C library, it is under the BSD 3-Clause license.

#ifndef __XI_CSV_LAYER_DATA_H__
#define __XI_CSV_LAYER_DATA_H__

#include "xi_http_layer_input.h"
#include "xi_stated_csv_decode_value_state.h"
#include "xi_stated_sscanf_state.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    http_layer_input_t*                 http_layer_input;
    unsigned short                      datapoint_decode_state;
    unsigned short                      feed_decode_state;
    xi_stated_csv_decode_value_state_t  csv_decode_value_state;
    xi_stated_sscanf_state_t            stated_sscanf_state;
    xi_response_t*                      response;
} csv_layer_data_t;

#ifdef __cplusplus
}
#endif

#endif // __XI_CSV_LAYER_DATA_H__
