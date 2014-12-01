// Copyright (c) 2003-2014, LogMeIn, Inc. All rights reserved.
// This is part of Xively C library, it is under the BSD 3-Clause license.

#ifndef __XI_LAYER_TYPE_H__
#define __XI_LAYER_TYPE_H__

#include "xi_layer_interface.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char layer_type_id_t;

typedef struct
{
    layer_type_id_t     layer_type_id;
    layer_interface_t   layer_interface;
} layer_type_t;


#define LAYER_LOCAL_TYPE( type_name, layer_type_id, data_ready, on_read, on_close, close )\
    static layer_type_t type_name = { layer_type_id, { data_ready, on_read, on_close, close } }

#ifdef __cplusplus
}
#endif

#endif // __XI_LAYER_TYPE_H__
