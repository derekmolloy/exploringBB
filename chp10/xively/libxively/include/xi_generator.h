// Copyright (c) 2003-2014, LogMeIn, Inc. All rights reserved.
// This is part of Xively C library, it is under the BSD 3-Clause license.

#ifndef __XI_GENERATOR_H__
#define __XI_GENERATOR_H__

#include <string.h>

#include "xi_coroutine.h"
#include "xi_common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef const void* ( xi_generator_t )( const void* input, short* curr_state );

extern const_data_descriptor_t __xi_tmp_desc;

#define ENABLE_GENERATOR() \
    unsigned char __xi_len = 0; \
    static short __xi_gen_sub_state = 0; \
    const const_data_descriptor_t* __xi_gen_data = 0; \
    ( void )( __xi_len ); \
    ( void )( __xi_gen_sub_state ); \
    ( void )( __xi_gen_data );

#define gen_ptr_text( state, ptr_text ) \
{ \
    __xi_len = strlen( ptr_text ); \
    __xi_tmp_desc.data_ptr = ptr_text; \
    __xi_tmp_desc.data_size = __xi_len; \
    __xi_tmp_desc.real_size = __xi_len; \
    YIELD( state, ( void* ) &__xi_tmp_desc ); \
}

#define gen_ptr_text_and_exit( state, ptr_text ) \
{ \
    __xi_len = strlen( ptr_text ); \
    __xi_tmp_desc.data_ptr  = ptr_text; \
    __xi_tmp_desc.data_size = __xi_len; \
    __xi_tmp_desc.real_size = __xi_len; \
    EXIT( state, ( void* ) &__xi_tmp_desc ); \
}

#define gen_static_text( state, text ) \
{ \
    static const char* const tmp_str = text; \
    __xi_tmp_desc.data_ptr  = tmp_str; \
    __xi_tmp_desc.real_size = __xi_tmp_desc.data_size = sizeof( text ) - 1; \
    YIELD( state, ( void* ) &__xi_tmp_desc ); \
}

#define call_sub_gen( state, input, sub_gen ) \
{ \
    __xi_gen_sub_state = 0; \
    while( __xi_gen_sub_state != 1 ) \
    { \
        YIELD( state, sub_gen( input, &__xi_gen_sub_state) ); \
    } \
}

#define call_sub_gen_and_exit( state, input, sub_gen ) \
{ \
    __xi_gen_sub_state = 0; \
    while( __xi_gen_sub_state != 1 ) \
    { \
        __xi_gen_data = sub_gen( input, &__xi_gen_sub_state ); \
        if( __xi_gen_sub_state != 1 ) \
        { \
            YIELD( state, __xi_gen_data ); \
        } \
        else \
        { \
            EXIT( state, __xi_gen_data ); \
        } \
    } \
}

#define gen_static_text_and_exit( state, text ) \
{ \
    static const char* const tmp_str = text; \
    __xi_tmp_desc.data_ptr = tmp_str; \
    __xi_tmp_desc.real_size = __xi_tmp_desc.data_size = sizeof( text ) - 1; \
    EXIT( state, ( void* ) &__xi_tmp_desc ); \
}

#ifdef __cplusplus
}
#endif

#endif // __XI_GENERATOR_H__
