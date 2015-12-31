#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "ZGEDecl.h"

namespace ZGE
{
    class Window
    {
    public:
        virtual ~Window () {};

        virtual void SwapBuffer () = 0;

        I32 Left ()
        {
            return m_Left;
        }

        I32 Top ()
        {
            return m_Top;
        }

        U32 Width ()
        {
            return m_Width;
        }

        U32 Height ()
        {
            return m_Height;
        }

        bool IsShowCmd ()
        {
            return m_IsShowCmd;
        }

        bool IsActive ()
        {
            return m_IsActive;
        }

    protected:
        Window ( const std::wstring& name ) : m_Name ( name )
        {
            m_Left      = 0;
            m_Top       = 0;
            m_Width     = 0;
            m_Height    = 0;
            m_IsActive  = false;
            m_IsShowCmd = false;
        }

        virtual void OnSize ( U32 width, U32 height ) = 0;

        std::wstring m_Name;
        I32 m_Left;
        I32 m_Top;
        U32 m_Width;
        U32 m_Height;
        bool m_IsShowCmd;
        bool m_IsActive;
    };
}


#endif