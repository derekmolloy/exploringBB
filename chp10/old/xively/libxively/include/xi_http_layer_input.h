// Copyright (c) 2003-2014, LogMeIn, Inc. All rights reserved.
// This is part of Xively C library, it is under the BSD 3-Clause license.

#ifndef __XI_HTTP_LAYER_INPUT_H__
#define __XI_HTTP_LAYER_INPUT_H__

#include "xively.h"
#include "xi_layer.h"
#include "xi_generator.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
      HTTP_LAYER_INPUT_DATASTREAM_GET = 0
    , HTTP_LAYER_INPUT_DATASTREAM_UPDATE
    , HTTP_LAYER_INPUT_DATASTREAM_CREATE
    , HTTP_LAYER_INPUT_DATASTREAM_DELETE
    , HTTP_LAYER_INPUT_DATAPOINT_DELETE
    , HTTP_LAYER_INPUT_DATAPOINT_DELETE_RANGE
    , HTTP_LAYER_INPUT_FEED_UPDATE
    , HTTP_LAYER_INPUT_FEED_GET
    , HTTP_LAYER_INPUT_FEED_GET_ALL
} xi_query_type_t;

typedef struct
{
    xi_query_type_t         query_type;
    xi_context_t*           xi_context;
    xi_generator_t*         payload_generator;

    union http_union_data_t
    {
        struct xi_get_datastream_t
        {
            const char*     datastream;
            const xi_datapoint_t* value;
        } xi_get_datastream;

        struct xi_update_datastream_t
        {
            const char*     datastream;
            const xi_datapoint_t* value;
        } xi_update_datastream;

        struct xi_create_datastream_t
        {
            const char*         datastream;
            const xi_datapoint_t*     value;
        } xi_create_datastream;

        struct xi_delete_datastream_t
        {
            const char*         datastream;
        } xi_delete_datastream;

        struct xi_delete_datapoint_t
        {
            const char*         datastream;
            const xi_datapoint_t*     value;
        } xi_delete_datapoint;

        struct xi_delete_datapoint_range_t
        {
            const char*         datastream;
            const xi_timestamp_t*     value_start;
            const xi_timestamp_t*     value_end;
        } xi_delete_datapoint_range;

        struct xi_get_feed_t
        {
            const xi_feed_t*      feed;
        } xi_get_feed;

        struct xi_update_feed_t
        {
            const xi_feed_t*      feed;
        } xi_update_feed;
    } http_union_data;

} http_layer_input_t;

#ifdef __cplusplus
}
#endif

#endif // __XI_HTTP_LAYER_INPUT_H__
