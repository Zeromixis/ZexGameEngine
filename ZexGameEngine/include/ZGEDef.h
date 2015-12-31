#ifndef _ZGEDEF_H_
#define _ZGEDEF_H_

#include "ZGEDecl.h"

#include "External/boost/preprocessor/stringize.hpp"
#include "External/boost/preprocessor/cat.hpp"

namespace ZGE
{
    typedef signed char			I8;
    typedef unsigned char		U8;
    typedef signed short		I16;
    typedef unsigned short		U16;
    typedef signed int			I32;
    typedef unsigned int		U32;
    typedef signed long long	I64;
    typedef unsigned long long	U64;
    typedef float				F32;
    typedef double				F64;

    const F32 PI = 3.141692653f;

#define CODE_LOCATION_STRING BOOST_PP_STRINGIZE ( BOOST_PP_CAT( BOOST_PP_CAT ( Static Assert Failed! File:, __FILE__ ),  (__LINE__) ) )
#define ZGE_STATIC_ASSART( cond ) static_assert( cond, CODE_LOCATION_STRING );
/*#define ZGE_STATIC_ASSART( cond ) static_assert ( cond, "error" );*/
#define ZGE_STATIC_ASSART_MSG( cond, str ) static_assert ( cond, str );    // With Self-Defined String

}

#endif // !_ZGEDEF_H_
