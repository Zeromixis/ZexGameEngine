#ifndef _UTIL_H_
#define _UTIL_H_

#include <string>
#include <codecvt>
#include <iostream>
#include <fstream>
#include <sstream>

namespace ZGE
{

    static std::wstring LoadShaderSourceFromFile ( const std::wstring& path )
    {
        std::wifstream wif ( path );
        wif.imbue ( std::locale ( std::locale::empty (), new std::codecvt_utf8< wchar_t > ) );
        std::wstringstream wss;
        wss << wif.rdbuf ();
        return wss.str ();
    }

    static std::string ConvertWStringToString ( const std::wstring& src )
    {
        typedef std::codecvt_utf8< wchar_t > convert_type;
        std::wstring_convert< convert_type, wchar_t > converter;
        return converter.to_bytes ( src );
    }

    static std::wstring ConvertStringToWString ( const std::string &src )
    {
        typedef std::codecvt_utf8< wchar_t > convert_type;
        std::wstring_convert< convert_type, wchar_t > converter;
        return converter.from_bytes ( src );
    }
    
}
#endif