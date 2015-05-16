#include "Timer.h"
#include <windows.h>

namespace ZGE
{
    Timer::Timer ()
    {
        Start ();
    }

    Timer::Timer ( bool isAutoStart )
    {
        m_IsStart = isAutoStart;
        if ( m_IsStart )
        {
            Start ();
        }
    }

    Timer::~Timer ()
    {

    }

    ZGE::F64 Timer::GetElapsedTimeMS ()
    {
        if ( m_IsStart )
        {
            LARGE_INTEGER freq, nowTick;
            QueryPerformanceFrequency ( &freq );
            QueryPerformanceCounter ( &nowTick );
            return ( F64 )( nowTick.QuadPart - m_StartTick ) / freq.QuadPart * 1000;
        }
        else
        {
            return 0;
        }
    }

    bool Timer::IsStopped ()
    {
        return !m_IsStart;
    }

    void Timer::Start ()
    {
        m_IsStart = true;
        Reset ();
    }

    void Timer::Stop ()
    {
        m_IsStart = false;
    }

    void Timer::Reset ()
    {
        LARGE_INTEGER li;
        QueryPerformanceCounter ( &li );
        m_StartTick = li.QuadPart;
    }

}
