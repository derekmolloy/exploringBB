// Copyright (c) 2003-2014, LogMeIn, Inc. All rights reserved.
// This is part of Xively C library, it is under the BSD 3-Clause license.

#ifndef __XI_MACROS_H__
#define __XI_MACROS_H__

#include <string.h>
#include "xi_err.h"

#define XI_STR_EXPAND(tok) #tok
#define XI_STR(tok) XI_STR_EXPAND(tok)
#define XI_MIN(a,b) (a)<(b)?(a):(b)
#define XI_MAX(a,b) (a)<(b)?(b):(a)
#define XI_UNUSED(x) (void)(x)

#define XI_GUARD_EOS(s,size) { (s)[ (size) - 1 ] = '\0'; }

#define XI_CLAMP(a,b,t) XI_MIN( XI_MAX( (a), (b) ), (t) )

#define XI_CHECK_CND(cnd,e) if( (cnd) ) { xi_set_err( (e) ); goto err_handling; }
#define XI_CHECK_ZERO(a,e) XI_CHECK_CND((a) == 0,(e))
#define XI_CHECK_NEG(a) if( (a) < 0 ) )
#define XI_CHECK_PTR(a,b) if( (a) == (b) )
#define XI_SAFE_FREE(a) if( (a) ) { xi_free(a); (a) = 0; }
#define XI_CHECK_MEMORY(a) XI_CHECK_CND((a) == 0,XI_OUT_OF_MEMORY)
#define XI_CHECK_SIZE(a,b,e) XI_CHECK_CND(((a) >= (b) || (a) < 0 ),e)
#define XI_CLEAR_STATIC_BUFFER(a) memset( (a), 0, sizeof(a) )
#define XI_CHECK_S(s,size,o,e) {\
    XI_CHECK_SIZE(s,size-o,e)\
    else\
    {\
        (o) += (s);\
    }}

#endif //__XI_MACROS_H__
