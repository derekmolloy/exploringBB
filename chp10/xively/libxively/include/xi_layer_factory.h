// Copyright (c) 2003-2014, LogMeIn, Inc. All rights reserved.
// This is part of Xively C library, it is under the BSD 3-Clause license.

#ifndef __XI_LAYER_FACTORY_H__
#define __XI_LAYER_FACTORY_H__

#include "xi_layer.h"
#include "xi_layer_interface.h"
#include "xi_layer_factory_interface.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const layer_factory_interface_t FACTORY_ENTRIES[ ];
extern const layer_type_t LAYER_TYPES[];

static inline layer_t* alloc_layer( const layer_type_id_t layer_type_id )
{
    const layer_type_t* layer_type = &LAYER_TYPES[ layer_type_id ];
    return FACTORY_ENTRIES[ layer_type_id ].alloc( layer_type );
}

static inline void free_layer( layer_t* layer )
{
    const layer_type_t* layer_type = &LAYER_TYPES[ layer->layer_type_id ];
    return FACTORY_ENTRIES[ layer->layer_type_id ].free( layer_type, layer );
}

static inline layer_t* create_layer( layer_t* layer, void* user_data )
{
    // PRECONDITION
    assert( layer != 0 );

    return FACTORY_ENTRIES[ layer->layer_type_id ].placement_create( layer, user_data );
}

static inline void destroy_layer( layer_t* layer )
{
    // PRECONDITION
    assert( layer != 0 );

    FACTORY_ENTRIES[ layer->layer_type_id ].placement_delete( layer );
}

static inline layer_t* alloc_create_layer( const layer_type_id_t layer_type_id, void* user_data )
{
    layer_t* ret = alloc_layer( layer_type_id );

    XI_CHECK_MEMORY( ret );

    return create_layer( ret, user_data );

err_handling:
    return 0;
}

static inline void free_destroy_layer( layer_t* layer )
{
    destroy_layer( layer );
    free_layer( layer );
}

#ifdef __cplusplus
}
#endif

#endif // __XI_LAYER_FACTORY_H__
