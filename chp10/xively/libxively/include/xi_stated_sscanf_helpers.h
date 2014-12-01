// Copyright (c) 2003-2014, LogMeIn, Inc. All rights reserved.
// This is part of Xively C library, it is under the BSD 3-Clause license.

#ifndef __XI_STATED_SSCANF_HELPERS_H__
#define __XI_STATED_SSCANF_HELPERS_H__

#include "xi_common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char ( accept_char_t )( const char c );

static inline unsigned char is_digit( const char c )
{
    return ( c >= 48 && c <= 57 ) ? 1 : 0;
}

static inline unsigned char is_alpha( const char c )
{
    return ( ( c >= 65 && c <= 90 ) || ( c >= 97 && c <= 122 ) ) ? 1 : 0;
}

static inline unsigned char is_header( const char c )
{
    return ( ( c >= 65 && c <= 122 ) || c == 45 ) ? 1 : 0;
}

static inline unsigned char is_any( const char c )
{
    return ( c >= 32 && c <= 122 ) ? 1 : 0;
}

static inline unsigned char is_channel_id( const char c )
{
    return ( is_alpha( c ) || is_digit( c ) || c == 43 || c == 45 || c == 95 ) ? 1 : 0;
}

static inline void to_int( xi_stated_sscanf_state_t* s, int* out )
{
    int base = 10;
    *out     = ( s->buffer[ s->buff_len - 1 ] - 48 );

    for( unsigned char j = 1; j < s->buff_len; ++j, base *= 10 )
    {
        *out += base * ( s->buffer[ s->buff_len - j - 1 ] - 48 );
    }
}

static inline void to_unsigned_char( xi_stated_sscanf_state_t* s, unsigned char* out )
{
    int base = 10;
    *out     = ( s->buffer[ s->buff_len - 1 ] - 48 );

    for( unsigned char j = 1; j < s->buff_len; ++j, base *= 10 )
    {
        *out += base * ( s->buffer[ s->buff_len - j - 1 ] - 48 );
    }
}

static inline unsigned char pass_all( const char c )
{
    ( void ) c;
    return 1;
}

static inline signed char safe_until_copier(
          unsigned char* dst_i
        , unsigned short* src_i
        , data_descriptor_t* dst
        , const const_data_descriptor_t* src
        , accept_char_t* fun )
{
    while( fun( src->data_ptr[ *src_i ] ) )
    {
        dst->data_ptr[ (*dst_i)++ ] = src->data_ptr[ (*src_i)++ ];

        if( *dst_i == dst->real_size )
        {
            return -1;
        }

        if( *src_i == src->real_size )
        {
            return 0; // more data needed
        }
    }

    return 1;
}

#ifdef __cplusplus
}
#endif

#endif // __XI_STATED_SSCANF_HELPERS_H__
