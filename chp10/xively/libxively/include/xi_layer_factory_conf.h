// Copyright (c) 2003-2014, LogMeIn, Inc. All rights reserved.
// This is part of Xively C library, it is under the BSD 3-Clause license.

#ifndef __XI_FACTORY_CONF_H__
#define __XI_FACTORY_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "xi_layer_factory_interface.h"

#define BEGIN_FACTORY_CONF() \
const layer_factory_interface_t FACTORY_ENTRIES[] = {

#define FACTORY_ENTRY( ttc, pc, pd, a, f ) \
    { ttc, pc, pd, a, f }

#define END_FACTORY_CONF() };

#ifdef __cplusplus
}
#endif

#endif // __XI_FACTORY_CONF_H__
