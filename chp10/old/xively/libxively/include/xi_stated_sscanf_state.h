// Copyright (c) 2003-2014, LogMeIn, Inc. All rights reserved.
// This is part of Xively C library, it is under the BSD 3-Clause license.

#ifndef __XI_STATED_SSCANF_STATE_H__
#define __XI_STATED_SSCANF_STATE_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    char            buffer[ 8 ];
    unsigned short  p;
    unsigned short  state;
    unsigned char   max_len;
    unsigned char   buff_len;
    unsigned char   tmp_i;
    unsigned char   vi;
} xi_stated_sscanf_state_t;

#ifdef __cplusplus
}
#endif

#endif // __XI_STATED_SSCANF_STATE_H__
