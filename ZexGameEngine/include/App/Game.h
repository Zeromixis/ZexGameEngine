#pragma once

#include <windows.h>
#include "ZGEDecl.h"

#include "Pattern/Singleton.h"

namespace ZGE
{
    class Game
        : public Singleton<Game>
    {
    public:
        Game ();

        ~Game ();

        void Init ();

        void Loop ();

        void Finalize ();
    };
}