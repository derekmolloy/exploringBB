// Copyright (c) 2003-2014, LogMeIn, Inc. All rights reserved.
// This is part of Xively C library, it is under the BSD 3-Clause license.

#ifndef __XI_LAYER_CONNECTION_H__
#define __XI_LAYER_CONNECTION_H__

#include "xi_macros.h"
#include "xi_debug.h"
#include "xi_layer_api.h"
#include "xi_layer.h"
#include "xi_layer_type.h"
#include "xi_layer_factory.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SIZE_SUFFIX _SIZE

typedef struct
{
    layer_t* bottom;
    layer_t* top;
} layer_chain_t;

#define DEFINE_CONNECTION_SCHEME( name, args ) \
    static layer_type_id_t name[]       = { args }; \
    static size_t name##SIZE_SUFFIX     = sizeof( name ) / sizeof( layer_type_id_t )

#define CONNECTION_SCHEME_LENGTH( name ) name##SIZE_SUFFIX

static inline layer_chain_t connect_layers( layer_t* layers[], const size_t length )
{
    XI_UNUSED( layers );
    assert( length >= 2 && "you have to connect at least two layers to each other" );

    for( size_t i = 1; i < length; ++i )
    {
        CONNECT_LAYERS( layers[ i - 1 ], layers[ i ] );
    }

    layer_chain_t ret;

    ret.bottom  = layers[ 0 ];
    ret.top     = layers[ length -1 ];

    return ret;
}

static inline layer_chain_t create_and_connect_layers(
          const layer_type_id_t layers_ids[]
        , void* user_datas[]
        , const size_t length )
{
    layer_t* layers[ length ];
    memset( layers, 0, sizeof( layers ) );

    for( size_t i = 0; i < length; ++i )
    {
        layers[ i ] = alloc_create_layer( layers_ids[ i ], user_datas[ i ] );
    }

    return connect_layers( layers, length );
}

static inline void destroy_and_disconnect_layers( layer_chain_t* chain, const size_t length )
{
    layer_t* layers[ length ];
    memset( layers, 0, sizeof( layers ) );

    assert( chain != 0 && "layer chain must not be 0!" );
    assert( chain->bottom->layer_connection.next != 0 && "layer chain must have at least 2 elements!" );

    layer_t* prev   = chain->bottom;
    layer_t* tmp    = prev->layer_connection.next;

    unsigned char i = 0;
    layers[ i ]     = prev;

    while( tmp )
    {
        DISCONNECT_LAYERS( prev, tmp );

        prev          = tmp;
        tmp           = tmp->layer_connection.next;

        layers[ ++i ] = prev;
    }

    for( size_t i = 0; i < length; ++i )
    {
        free_destroy_layer( layers[ i ] );
    }
}

#ifdef __cplusplus
}
#endif

#endif // __XI_LAYER_CONNECTION_H__
