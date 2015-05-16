#include "Clock.h"


namespace ZGE
{
    Clock::Clock ( ) : m_Timer ( true )
    {

    }

    Clock::~Clock ()
    {
    }

    ZGE::F64 Clock::GetElapsedTimeFromStartMS ()
    {
        return m_Timer.GetElapsedTimeMS ();
    }

}
