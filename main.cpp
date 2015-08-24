#include <iostream>

#if _WIN32

#include <windows.h>
#include "CorePrerequisites.h"
#include <memory>
#include "App/WindowWin.h"
#include "App/Context.h"
#include "Math/Matrix.h"
#include "Core/TimeManager.h"


int WINAPI WinMain ( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdWindow )
{
    using namespace ZGE;
    WindowWin *window = new ZGE::WindowWin ( ZGE::String ( L"ZexGameEngine" ) );

    Context::GetInstance ()->SetWindowPtr ( window );
    Context::GetInstance ()->SetRenderEngine ( new RenderEngine () );

	// Init various Manager here.
    InputManager::GetInstance ();
	TimeManager::GetInstance ();

    MSG msg;
    ::PeekMessage ( &msg, window->Hwnd (), 0, 0, PM_NOREMOVE );
    
    while ( WM_QUIT != msg.message )
    {
        BOOL isGotMsg = ::PeekMessage ( &msg, window->Hwnd (), 0, 0, PM_REMOVE );
        if ( isGotMsg )
        {
            ::TranslateMessage ( &msg );
            ::DispatchMessage ( &msg );
        }
        else
        {
            TimeManager::GetInstance ()->Update ();
            InputManager::GetInstance ()->Update ();
            Context::GetInstance ()->GetRenderEngine ()->Refresh ();
        }
    }
    return msg.wParam;
}

#else

int main ( int argc, char* argv[] )
{
    return 0;
}
#endif

