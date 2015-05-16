#ifndef _CORE_TIMEMANAGER_H_
#define _CORE_TIMEMANAGER_H_

#include "Singleton.h"
#include "Timer.h"


namespace ZGE
{
	class TimeManager : public Singleton < TimeManager, true >
	{
		friend class Singleton< TimeManager, true >;
	public:
		~TimeManager ();

		F32 Fps ()
		{
			return m_Fps;
		}

		F32 FrameTimeMS ()
		{
			return m_FrameTime;
		}

		F64 RunningTimeMS ()
		{
			m_RunningTimer.GetElapsedTimeMS ();
		}

		void Update ();

	private:
		TimeManager ();

		Timer m_RunningTimer;
		F32 m_Fps;
		F32 m_FrameTime;
		F64 m_LastUpdateTime;
	};
}


#endif