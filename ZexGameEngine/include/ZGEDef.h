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

#define CODE_LOCATION_STRING BOOST_PP_STRINGIZE ( BOOST_PP_CAT( BOOST_PP_CAT ( Static Assert Failed! File:, __FILE__ ),  BOOST_PP_CAT ( Line:, __LINE__) ) )
#define ZGE_STATIC_ASSART( cond ) static_assert( cond, CODE_LOCATION_STRING );
#define ZGE_STATIC_ASSART_MSG( cond, str ) static_assert ( cond, str );    // With Self-Defined String

}
