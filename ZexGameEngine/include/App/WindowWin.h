#ifndef _WINDOW_WIN_H
#define _WINDOW_WIN_H

#ifdef _WIN32

#include <windows.h>
#include "Window.h"
#include "boost/signals2.hpp"

namespace ZGE
{
    class WindowWin : public Window
    {
    public:
        WindowWin ( const String& name );
        virtual ~WindowWin () {};
        
        LRESULT MsgProc ( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );

        void OpenConsole ();

        virtual void OnSize ( U32 width, U32 height );

        virtual void SwapBuffer ();

        HWND Hwnd ()
        {
            return m_Hwnd;
        }

        HDC Hdc ()
        {
            return m_Hdc;
        }

        typedef boost::signals2::signal< void ( const Window & wnd, HRAWINPUT ri )  > RawInputEvent;

        RawInputEvent& OnRawInput ()
        {
            return m_RawInputEvent;
        }

    protected:
        HINSTANCE m_HInstance;
        HWND m_Hwnd;
        HDC m_Hdc;
        RawInputEvent m_RawInputEvent;
    };
}

#endif

#endif