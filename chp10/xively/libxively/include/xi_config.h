// Copyright (c) 2003-2014, LogMeIn, Inc. All rights reserved.
// This is part of Xively C library, it is under the BSD 3-Clause license.

#ifndef __XI_CONFIG_H__
#define __XI_CONFIG_H__

#ifdef MBED_USERNAME
#include "xi_config_mbed.h"
#ifndef XI_IO_LAYER
#define XI_IO_LAYER XI_IO_MBED
#endif
#endif

#if XI_USER_CONFIG
#include "xi_user_config.h"
#endif

#ifndef XI_HTTP_MAX_HEADERS
#define XI_HTTP_MAX_HEADERS                16
#endif

#ifndef XI_HTTP_STATUS_STRING_SIZE
#define XI_HTTP_STATUS_STRING_SIZE         32
#endif

#ifndef XI_HTTP_HEADER_NAME_MAX_SIZE
#define XI_HTTP_HEADER_NAME_MAX_SIZE       32
#endif

#ifndef XI_HTTP_HEADER_VALUE_MAX_SIZE
#define XI_HTTP_HEADER_VALUE_MAX_SIZE      64
#endif

#ifndef XI_HTTP_MAX_CONTENT_SIZE
#define XI_HTTP_MAX_CONTENT_SIZE           512
#endif

#ifndef XI_MAX_DATASTREAMS
#define XI_MAX_DATASTREAMS                 16
#endif

#ifndef XI_MAX_DATAPOINTS
#define XI_MAX_DATAPOINTS                  16
#endif

#ifndef XI_MAX_DATASTREAM_NAME
#define XI_MAX_DATASTREAM_NAME             16
#endif

#ifndef XI_VALUE_STRING_MAX_SIZE
#define XI_VALUE_STRING_MAX_SIZE           32
#endif

#ifndef XI_QUERY_BUFFER_SIZE
#define XI_QUERY_BUFFER_SIZE               512
#endif

#ifndef XI_ID_BUFFER_SIZE
#define XI_ID_BUFFER_SIZE                  256
#endif

#ifndef XI_CONTENT_BUFFER_SIZE
#define XI_CONTENT_BUFFER_SIZE             256
#endif

#ifndef XI_CSV_BUFFER_SIZE
#define XI_CSV_BUFFER_SIZE                 128
#endif

#ifndef XI_HOST
#define XI_HOST                            "api.xively.com"
#endif

#ifndef XI_PORT
#define XI_PORT                            80
#endif

#endif // __XI_CONFIG_H__
