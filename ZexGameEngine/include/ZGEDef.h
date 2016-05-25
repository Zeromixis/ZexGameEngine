#pragma once

#include "ZGEDecl.h"

#include "External/boost/preprocessor/stringize.hpp"
#include "External/boost/preprocessor/cat.hpp"

namespace ZGE
{
    typedef char			    I8;
    typedef unsigned char		U8;
    typedef short		        I16;
    typedef unsigned short		U16;
    typedef int			        I32;
    typedef unsigned int		U32;
    typedef long long	        I64;
    typedef unsigned long long	U64;
    typedef float				F32;
    typedef double				F64;

    typedef Vector< I32, 2 > Vector2i;
    typedef Vector< F32, 2 > Vector2f;
    typedef Vector< U32, 2 > Vector2u;
    typedef Vector< I32, 3 > Vector3i;
    typedef Vector< F32, 3 > Vector3f;
    typedef Vector< U32, 3 > Vector3u;
    typedef Vector< I32, 4 > Vector4i;
    typedef Vector< F32, 4 > Vector4f;
    typedef Vector< U32, 4 > Vector4u;

    typedef Matrix44< F32 > Float44;

#define CODE_LOCATION_STRING BOOST_PP_STRINGIZE ( BOOST_PP_CAT( BOOST_PP_CAT ( Static Assert Failed! File:, __FILE__ ),  BOOST_PP_CAT ( Line:, __LINE__) ) )
#define ZGE_STATIC_ASSART( cond ) static_assert( cond, CODE_LOCATION_STRING );
#define ZGE_STATIC_ASSART_MSG( cond, str ) static_assert ( cond, str );    // With Self-Defined String

}
