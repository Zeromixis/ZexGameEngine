#pragma once

#include "ZGEDecl.h"

#include "Math/Vector.h"

namespace ZGE
{
	struct UIEvent
	{
        UIEvent ()
            : m_IsCatched ( true )
        {
            
        }

		virtual ~UIEvent () {};

        void Catch () 
		{ 
			m_IsCatched = true; 
		}

        bool IsCatched () const
		{ 
			return m_IsCatched; 
		}

    private:
        mutable bool m_IsCatched;
	};

	struct UIMouseEvent : public UIEvent
	{
		enum EventType
		{
			MOUSE_ENTER,
			MOUSE_EXIT,
			MOUSE_PRESS,
			MOUSE_RELEASE,
			MOUSE_MOVE,
		};

		Vector2f Position;
		EventType EType;
	};
}
