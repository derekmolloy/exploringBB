// Copyright (c) 2003-2014, LogMeIn, Inc. All rights reserved.
// This is part of Xively C library, it is under the BSD 3-Clause license.

#ifndef __XI_LAYER_DEFAULT_ALLOCATORS_H__
#define __XI_LAYER_DEFAULT_ALLOCATORS_H__

#include "xi_layer.h"
#include "xi_layer_type.h"
#include "xi_allocator.h"
#include "xi_macros.h"

#ifdef __cplusplus
extern "C" {
#endif

static inline layer_t* default_layer_heap_alloc( const layer_type_t* type )
{
    layer_t* ret = ( layer_t* ) xi_alloc( sizeof( layer_t ) );

    XI_CHECK_MEMORY( ret );

    memset( ret, 0, sizeof( layer_t ) );

    ret->layer_functions                = &type->layer_interface;
    ret->layer_type_id                  = type->layer_type_id;
    ret->layer_connection.self          = ret;

    return ret;

err_handling:
    return 0;
}

static inline void default_layer_heap_free( const layer_type_t* type, layer_t* layer )
{
    XI_UNUSED( type );
    xi_free( layer );
}

static unsigned char stack_counter = 0;

#define MAX_LAYERS_ON_STACK 4

static inline layer_t* default_layer_stack_alloc( const layer_type_t* type )
{
    static layer_t layers_stack[ MAX_LAYERS_ON_STACK ];

    memset( &layers_stack[ stack_counter ], 0, sizeof( layer_t ) );

    layers_stack[ stack_counter ].layer_functions                = &type->layer_interface;
    layers_stack[ stack_counter ].layer_type_id                  = type->layer_type_id;
    layers_stack[ stack_counter ].layer_connection.self          = &layers_stack[ stack_counter ];

    stack_counter += 1;

    return &layers_stack[ stack_counter - 1 ];
}

static inline void default_layer_stack_free( const layer_type_t* type, layer_t* layer )
{
    XI_UNUSED( type );
    XI_UNUSED( layer );

    stack_counter -= 1;
}

#ifdef __cplusplus
}
#endif

#endif // __XI_LAYER_DEFAULT_ALLOCATORS_H__
