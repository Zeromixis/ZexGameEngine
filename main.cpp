#include "ZGEDecl.h"

#include "App/Game.h"

int WINAPI WinMain ( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdWindow )
{
    ZGE::Game ZexGame;
    ZexGame.Init ();
    ZexGame.Loop ();
    ZexGame.Finalize ();
}

