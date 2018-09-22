// Copyright (c) 2003-2014, LogMeIn, Inc. All rights reserved.
// This is part of Xively C library, it is under the BSD 3-Clause license.

#ifndef __XI_HELPERS_H__
#define __XI_HELPERS_H__

#include <stdlib.h>
#include <limits.h>

#ifdef __cplusplus
extern "C" {
#endif

// Avoid using `strdup()` which can cause some problems with `free()`,
// because of buggy implementations of `realloc()`.
char* xi_str_dup( const char* s );

int xi_str_copy_untiln( char* dst, size_t dst_size, const char* src, char delim );

char* xi_replace_with( char p, char r, char* buffer, size_t max_chars );

#ifdef __cplusplus
}
#endif

#endif // __XI_HELPERS_H__
