#ifndef _TIMER_H_
#define _TIMER_H_

#include "DataDef.h"

namespace ZGE
{
    class Timer
    {
    public:
        Timer ();
        Timer ( bool isAutoStart );
        ~Timer ();

        bool IsStopped ();
        void Start ();
        void Stop ();
        void Reset ();

        F64 GetElapsedTimeMS ();

    private:
        U64 m_StartTick;
        bool m_IsStart;
    };
}

#endif

