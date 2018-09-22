// Copyright (c) 2003-2014, LogMeIn, Inc. All rights reserved.
// This is part of Xively C library, it is under the BSD 3-Clause license.

#ifndef __XI_LAYER_FACTORY_INTERFACE_H__
#define __XI_LAYER_FACTORY_INTERFACE_H__

#include "xi_layer.h"
#include "xi_macros.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    // describes the type to create, mainly for sainity checks and nice error reporting
    layer_type_id_t                  type_id_to_create;

    // enables the placement initialization which is separeted from the allocation/deallocation
    layer_t* ( *placement_create ) ( layer_t* layer, void* user_data );

    // placement delete same as create but with oposite effect, may be used to clean and/or deallocate memory
    layer_t* ( *placement_delete ) ( layer_t* layer );

    // strict layer allocation, may implement different strategies for allocation of the memory required for layer
    layer_t* ( *alloc )            ( const layer_type_t* type );

    // strict deallocation of layer's memory
    void ( *free )                 ( const layer_type_t* type
                                   , layer_t* layer );
} layer_factory_interface_t;

// simpliest layer initialiser that does simply nothing, but returns the pointer to the initialized layer
inline static layer_t* placement_layer_pass_create( layer_t* layer, void* user_data )
{
    layer->user_data = user_data;
    return layer;
}

// simpliest layer delete function that does simply nothing, but returns the pointer to given layer which can free()'ed
inline static layer_t* placement_layer_pass_delete( layer_t* layer )
{
    return layer;
}

#ifdef __cplusplus
}
#endif

#endif // __XI_LAYER_FACTORY_INTERFACE_H__
