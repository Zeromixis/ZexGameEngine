#ifndef _CLOCK_H_
#define _CLOCK_H_

#include "Singleton.h"
#include "DataDef.h"
#include "Timer.h"

namespace ZGE
{
    class Clock : public Singleton< Clock, true >
    {
        friend class Singleton< Clock, true >;
    public:
        ~Clock ();
        F64 GetElapsedTimeFromStartMS ();

    private:
        Clock ();
        

        Timer m_Timer;
    };
}

#endif

