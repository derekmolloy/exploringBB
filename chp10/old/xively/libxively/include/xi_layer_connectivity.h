// Copyright (c) 2003-2014, LogMeIn, Inc. All rights reserved.
// This is part of Xively C library, it is under the BSD 3-Clause license.

#ifndef __XI_LAYER_CONNECTIVITY_H__
#define __XI_LAYER_CONNECTIVITY_H__

#ifdef __cplusplus
extern "C" {
#endif

struct layer;

typedef struct layer_connectivity
{
    struct layer* self;
    struct layer* next;
    struct layer* prev;
} layer_connectivity_t;

#ifdef __cplusplus
}
#endif

#endif // __XI_LAYER_CONNECTIVITY_H__
