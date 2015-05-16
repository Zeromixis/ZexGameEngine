#include "Core/TimeManager.h"
#include <iostream>

namespace ZGE
{
	TimeManager::TimeManager ()
		: m_RunningTimer ( true )
		, m_Fps ( 0.0f )
		, m_FrameTime ( 0.0f )
	{
		m_LastUpdateTime = m_RunningTimer.GetElapsedTimeMS ();
	}

	TimeManager::~TimeManager ()
	{

	}

	void TimeManager::Update ()
	{
		F64 nowUpdateTime = m_RunningTimer.GetElapsedTimeMS ();
		m_FrameTime = nowUpdateTime - m_LastUpdateTime;
		m_Fps = 1000.0f / m_FrameTime;
		m_LastUpdateTime = nowUpdateTime;

		// Debug Code
		std::cout << "Fps: " << Fps() << std::endl;
		std::cout << "FrameTimeMS: " << FrameTimeMS () << std::endl;
	}
}