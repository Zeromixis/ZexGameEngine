#ifdef _WIN32
#include "glloader/glloader.h"
#include "App/WindowWin.h"
#include "App/Context.h"
#include <assert.h>
#include <io.h>
#include <fcntl.h>
#include <iostream>
#include "Clock.h"

namespace ZGE
{
    LRESULT CALLBACK WndProc ( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
    {
        Window *window = Context::GetInstance ()->GetWindowPtr ().get ();
        WindowWin *windowWin = reinterpret_cast< WindowWin* >( window );

        LRESULT result = 0L;
        if ( nullptr != windowWin )
        {
            result = windowWin->MsgProc ( hwnd, msg, wParam, lParam );
        }
        else
        {
            result = ::DefWindowProc ( hwnd, msg, wParam, lParam );
        }
        return result;
    }

    WindowWin::WindowWin ( const String& name ) : Window ( name )
    {
        // Open the console for debug info output
        OpenConsole ();

        m_HInstance = GetModuleHandle ( nullptr );
        LPCWSTR szClassName = L"ZexGameEngine";
        LPCWSTR windowName = name.c_str ();
        HWND hwnd;

        WNDCLASSEXW wc;
        wc.cbSize           = sizeof( wc );
        wc.style            = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc      = WndProc;
        wc.cbClsExtra       = 0;
        wc.cbWndExtra       = sizeof ( this );
        wc.hInstance        = m_HInstance;
        wc.hIcon            = nullptr;
        wc.hCursor          = LoadCursor ( nullptr, IDC_ARROW );
        wc.hbrBackground    = static_cast< HBRUSH >( ::GetStockObject ( BLACK_BRUSH ) );
        wc.lpszMenuName     = nullptr;
        wc.lpszClassName    = szClassName;
        wc.hIconSm          = nullptr;

        RegisterClassExW ( &wc );

        hwnd = CreateWindowW ( L"ZexGameEngine", name.c_str(), 
            WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
            CW_USEDEFAULT,
            nullptr,
            nullptr,
            m_HInstance,
            nullptr
            );

        this->m_Hwnd = hwnd;

        // Create temp window first
        static PIXELFORMATDESCRIPTOR pfd;

        HDC hDC = ::GetDC ( m_Hwnd );

        int pixelFormat = ::ChoosePixelFormat ( hDC, &pfd );
        ::SetPixelFormat ( hDC, 1, &pfd );
        HGLRC hRC = ::wglCreateContext ( hDC );
        ::wglMakeCurrent ( hDC, hRC );

        // Before function glloader_init be called, the opengl context must be created before.
        glloader_init ();

        
        int nPixCount = 0;
        int searchResult[ 200 ] = { 0 };
        int pixAttribs[] =
        {
            WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
            WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
            WGL_COLOR_BITS_ARB,     32,
            WGL_DEPTH_BITS_ARB,     24,
            WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
            WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
            WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
            WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
            WGL_SAMPLES_ARB,        4,
            0
        };

        ::wglChoosePixelFormatARB ( hDC, pixAttribs, nullptr, 200, searchResult, ( UINT * )( &nPixCount ) );
        int majorVer = 0, minorVer = 0;
        glGetIntegerv ( GL_MAJOR_VERSION, &majorVer );
        glGetIntegerv ( GL_MINOR_VERSION, &minorVer );

        //Recreate the window

        wglMakeCurrent ( hDC, nullptr );
        wglDeleteContext ( hRC );
        ::ReleaseDC ( m_Hwnd, hDC );
        ::DestroyWindow ( hwnd );

        hwnd = CreateWindowW 
        ( 
            L"ZexGameEngine", name.c_str (),
            WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
            CW_USEDEFAULT,
            nullptr,
            nullptr,
            m_HInstance,
            nullptr
        );

        this->m_Hwnd = hwnd;

        ::ShowWindow ( hwnd, 1 );
        ::UpdateWindow ( hwnd );
        ::SetFocus ( hwnd );

        hDC = ::GetDC ( this->m_Hwnd );
        this->m_Hdc = hDC;

        ::SetPixelFormat ( hDC, searchResult[ 0 ], &pfd );
        const int attr[] = 
        {
            WGL_CONTEXT_MAJOR_VERSION_ARB, majorVer,
            WGL_CONTEXT_MINOR_VERSION_ARB, minorVer,
            WGL_CONTEXT_FLAGS_ARB,         WGL_CONTEXT_DEBUG_BIT_ARB,
            WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
            0
        };
        
        hRC = wglCreateContextAttribsARB ( hDC, nullptr, attr );

        wglMakeCurrent ( hDC, hRC );

        // Re-Init the glloader
        glloader_init ();

		// Disable vsync
		wglSwapIntervalEXT ( 0 );

        // Set Window Property
        ::RECT windowRect;
        ::RECT clientRect;
        ::GetWindowRect ( hwnd, &windowRect );
        ::GetClientRect ( hwnd, &clientRect );

        m_Left = windowRect.left;
        m_Top = windowRect.top;
        m_Width = clientRect.right;
        m_Height = clientRect.bottom;
    }

    LRESULT WindowWin::MsgProc ( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
    {
        switch ( msg )
        {
        case WM_PAINT:
            DefWindowProc ( hwnd, msg, wParam, lParam );
            break;
        case WM_CLOSE:
            DestroyWindow ( hwnd );
            break;
        case WM_DESTROY:
            PostQuitMessage ( 0 );
            break;
        case WM_SIZE:
            if ( SIZE_MAXHIDE != wParam && SIZE_MINIMIZED != wParam )
            {
                this->m_IsActive = true;
                this->OnSize ( LOWORD ( lParam ), HIWORD ( lParam ) );
            }
            else
            {
                // Prevent Rendering when resizing.
                this->m_IsActive = false;
            }
            break;
        case WM_INPUT:
            this->OnRawInput ()( *this, reinterpret_cast< HRAWINPUT >( lParam ) );
            break;
        default:
            return DefWindowProc ( hwnd, msg, wParam, lParam );
        }
        return 0;
    }

    void WindowWin::OpenConsole ()
    {
        const WORD MAX_CONSOLE_LINES = 500;
        int hConHandle;
        long lStdHandle;
        CONSOLE_SCREEN_BUFFER_INFO conInfo;
        FILE *fp;

        // Allocate a console for this process ( Only one console for each process )
        // After this function call, you can use GetStdHandle to get the HANDLE of console.
        AllocConsole ();

        // Set the screen buffer to be big enough to let us scroll text
        GetConsoleScreenBufferInfo ( GetStdHandle ( STD_OUTPUT_HANDLE ), &conInfo );
        conInfo.dwSize.Y = MAX_CONSOLE_LINES;
        SetConsoleScreenBufferSize ( GetStdHandle ( STD_OUTPUT_HANDLE ), conInfo.dwSize );

        // Redirect unbuffered STDOUT to the console
        lStdHandle = ( long )GetStdHandle ( STD_OUTPUT_HANDLE );

        // Convert the HANDLE to C FILE pointer.
        hConHandle = _open_osfhandle ( lStdHandle, _O_TEXT );
        fp = _fdopen ( hConHandle, "w" );

        // Redirect
        *stdout = *fp;

        // Do not use the buffer
        setvbuf ( stdout, nullptr, _IONBF, 0 );

        // Redirect unbuffered STDIN to the console
        lStdHandle = ( long )GetStdHandle ( STD_INPUT_HANDLE );
        hConHandle = _open_osfhandle ( lStdHandle, _O_TEXT );
        fp = _fdopen ( hConHandle, "r" );
        *stdin = *fp;
        setvbuf ( stdin, nullptr, _IONBF, 0 );

        // Redirect unbuffered STDERR to the console
        lStdHandle = ( long )GetStdHandle ( STD_ERROR_HANDLE );
        hConHandle = _open_osfhandle ( lStdHandle, _O_TEXT );
        fp = _fdopen ( hConHandle, "w" );
        *stderr = *fp;
        setvbuf ( stderr , nullptr, _IONBF, 0 );

        // Make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
        // point to console as well
        std::ios::sync_with_stdio ();

        m_IsShowCmd = true;
    }

    void WindowWin::OnSize ( U32 width, U32 height )
    {
        glViewport ( 0, 0, width, height );
    }

    void WindowWin::SwapBuffer ()
    {
        SwapBuffers ( m_Hdc );
    }

}

#endif // _WIN32
