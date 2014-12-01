// Copyright (c) 2003-2014, LogMeIn, Inc. All rights reserved.
// This is part of Xively C library, it is under the BSD 3-Clause license.

#ifndef __XI_LAYER_TYPES_CONF_H__
#define __XI_LAYER_TYPES_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "xi_layer_type.h"

#define BEGIN_LAYER_TYPES_CONF() \
    const layer_type_t LAYER_TYPES[] = {

#define LAYER_TYPE( type_id, data_ready, on_data_ready, close, on_close, init, connect ) \
    { type_id, { data_ready, on_data_ready, close, on_close, init, connect } }

#define END_LAYER_TYPES_CONF() \
    };

#ifdef __cplusplus
}
#endif

#endif // __XI_LAYER_TYPES_CONF_H__
