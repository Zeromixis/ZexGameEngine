#pragma once

#include "ZGEDecl.h"

#include "Pattern/Singleton.h"
#include "Timer.h"

namespace ZGE
{
    class Clock : public Singleton< Clock >
    {
        friend class Singleton< Clock >;
    public:
        ~Clock ();
        F64 GetElapsedTimeFromStartMS ();

    private:
        Clock ();
        

        Timer m_Timer;
    };
}


