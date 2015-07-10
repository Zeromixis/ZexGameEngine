#ifndef _CORE_TIMEMANAGER_H_
#define _CORE_TIMEMANAGER_H_

#include "Pattern/Singleton.h"
#include "Timer.h"


namespace ZGE
{
	class TimeManager : public Singleton < TimeManager >
	{
		friend class Singleton< TimeManager >;
	public:
		~TimeManager ();

		const F32 & Fps () const
		{
			return m_Fps;
		}

		const F32 & FrameTimeMS () const
		{
			return m_FrameTime;
		}

		const F64 RunningTimeMS () const
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