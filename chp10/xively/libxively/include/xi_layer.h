// Copyright (c) 2003-2014, LogMeIn, Inc. All rights reserved.
// This is part of Xively C library, it is under the BSD 3-Clause license.

#ifndef __XI_LAYER_H__
#define __XI_LAYER_H__

#include <stdint.h>

#include "xi_layer_connectivity.h"
#include "xi_layer_debug_info.h"
#include "xi_layer_interface.h"
#include "xi_layer_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct layer
{
    const layer_interface_t*    layer_functions;
    layer_connectivity_t        layer_connection;
    layer_type_id_t             layer_type_id;
    void*                       user_data;
#ifdef XI_DEBUG_LAYER_API
    layer_debug_info_t          debug_info;
#endif
} layer_t;


#ifdef __cplusplus
}
#endif

#endif // __XI_LAYER_H__
