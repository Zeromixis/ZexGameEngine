#include "App/Game.h"

#include "App/WindowWin.h"
#include "Math/Quaternion.h"
#include "App/Context.h"
#include "Math/Matrix.h"
#include "Core/TimeManager.h"

namespace ZGE
{

    Game::Game ()
    {

    }

    Game::~Game ()
    {

    }

    void Game::Init ()
    {
        WindowWin *window = new ZGE::WindowWin (std::wstring (L"ZexGameEngine"));

        Context::GetInstance ()->SetWindowPtr (window);
        Context::GetInstance ()->SetRenderEngine (new RenderEngine ());

        // Init various Manager here.
        InputManager::GetInstance ();
        TimeManager::GetInstance ();
    }

    void Game::Loop ()
    {
        WindowWin *window = (WindowWin *)(Context::GetInstance ()->GetWindowPtr ().get ());

        MSG msg;
        ::PeekMessage (&msg, window->Hwnd (), 0, 0, PM_NOREMOVE);

        while (WM_QUIT != msg.message)
        {
            BOOL isGotMsg = ::PeekMessage (&msg, window->Hwnd (), 0, 0, PM_REMOVE);
            if (isGotMsg)
            {
                ::TranslateMessage (&msg);
                ::DispatchMessage (&msg);
            }
            else
            {
                TimeManager::GetInstance ()->Update ();
                InputManager::GetInstance ()->Update ();
                Context::GetInstance ()->GetRenderEngine ()->Refresh ();
            }
        }
    }

    void Game::Finalize ()
    {

    }

}