#ifndef _TIMER_H_
#define _TIMER_H_

#include "ZGEDecl.h"

namespace ZGE
{
    class Timer
    {
    public:
        Timer ();
        Timer ( bool isAutoStart );
        ~Timer ();

        bool IsStopped () const;
        void Start ();
        void Stop ();
        void Reset ();

        const F64 GetElapsedTimeMS () const ;

    private:
        U64 m_StartTick;
        bool m_IsStart;
    };
}

#endif

