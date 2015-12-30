#ifndef _WIN32DEF_H_
#define _WIN32DEF_H_

#include <string>
#include <windows.h>
#include "glloader/glloader.h"
#include "Pattern/Object.h"

namespace ZGE
{
#ifdef _WIN32

    #ifdef _UNICODE
        #define ZGE_UNICODE
    #else

    #endif // _UNICODE

#endif // _WIN32

#ifdef ZGE_UNICODE
    typedef std::wstring String;
#else
    typedef std::string String;
#endif

#ifdef _WIN32
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
#endif // _WIN32

    const F32 PI = 3.141692653f;
}

#endif
