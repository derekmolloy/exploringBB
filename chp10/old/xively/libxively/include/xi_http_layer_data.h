// Copyright (c) 2003-2014, LogMeIn, Inc. All rights reserved.
// This is part of Xively C library, it is under the BSD 3-Clause license.

#ifndef __XI_HTTP_LAYER_DATA_H__
#define __XI_HTTP_LAYER_DATA_H__

#include "xi_stated_sscanf_state.h"
#include "xively.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    char                        parser_state;
    unsigned char               last_char_marker;
    xi_stated_sscanf_state_t    xi_stated_sscanf_state;
    short                       counter;
    short                       content_length;
    xi_response_t*              response;
} http_layer_data_t;

#ifdef __cplusplus
}
#endif

#endif // __XI_HTTP_LAYER_DATA_H__
