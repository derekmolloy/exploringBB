// Copyright (c) 2003-2014, LogMeIn, Inc. All rights reserved.
// This is part of Xively C library, it is under the BSD 3-Clause license.

#ifndef __XI_LAYER_INTERFACE_H__
#define __XI_LAYER_INTERFACE_H__

#include <stdio.h>

#include "xi_layer_connectivity.h"

#ifdef __cplusplus
extern "C" {
#endif

// This is part of the standarized protocol of communication that
// has been designed in order to provide the minimum restrictions,
// in order to make the usage safe, with maximum capabilities. This
// structure may be used to provide very simple communication between layers.

typedef enum
{
    LAYER_STATE_OK = 0,     // everything went fine, the executor can progress further
    LAYER_STATE_TIMEOUT,    // timeout occured
    LAYER_STATE_WANT_READ,  // to be more specific layer's can demand reading
    LAYER_STATE_WANT_WRITE, // or writing
    LAYER_STATE_ERROR       // something went terribly wrong, most probably it's not possible to recover, please refer to the errno value or xi_error
} layer_state_t;

// the hints that are used to provide additional information to the layers that are going to precess the query
typedef enum
{
    LAYER_HINT_NONE = 0,    // no hint, default behaviour
    LAYER_HINT_MORE_DATA    // more data will come in the future do not change the mode (that will happen on default)
} layer_hint_t;

typedef layer_state_t ( data_ready_t )      ( layer_connectivity_t* context, const void* data, const layer_hint_t hint );
typedef layer_state_t ( on_data_ready_t )   ( layer_connectivity_t* context, const void* data, const layer_hint_t hint );
typedef layer_state_t ( connect_t )         ( layer_connectivity_t* context, const void* data, const layer_hint_t hint );
typedef layer_state_t ( init_t )            ( layer_connectivity_t* context, const void* data, const layer_hint_t hint );
typedef layer_state_t ( close_t )           ( layer_connectivity_t* context );
typedef layer_state_t ( on_close_t )        ( layer_connectivity_t* context );

/* The raw interface of the purly raw layer that combines both: simplicity and functionality
 * of the 'on demand processing' idea.
 *
 * The design idea is based on the assumption that the communication with the server
 * is bidirectional and the data is processed in packages. This interface design is
 * very generic and allows to implement different approaches as an extension of the
 * basic operations that the interface is capable of. In very basic scenario the client
 * wants to send some number of bytes to the server and then awaits for the response.
 * After receiving the response the response is passed by to the processing layer through
 * on_data_ready function which signals the processing layer that there is new data to process.
 *
 * Term 'on demand processing' refers to the processing managed by the connection.
 * With the observation that all of the decision in the software depends on the network
 * capabilities it is clear that only the connection has ability to give the signals about
 * the processing to the rest of the components. The 'demand' takes it source in the
 * connection and that is the layer that has command over the another layers.
 */
typedef struct layer_interface
{
    // called whenever the prev layer wants more data to process/send over some kind of the connection
    data_ready_t        *data_ready;

    // called whenever there is data that is ready and it's source is the prev layer
    on_data_ready_t     *on_data_ready;

    // whenever the processing chain supposed to be closed
    close_t             *close;

    // whenver the processing chain is going to be closed it's source is in the prev layer
    on_close_t          *on_close;

    // whenever we want to init the layer
    init_t              *init;

    // whenever we want to connect the layer
    connect_t           *connect;

} layer_interface_t;

#ifdef __cplusplus
}
#endif

#endif // __XI_LAYER_INTERFACE_H__
