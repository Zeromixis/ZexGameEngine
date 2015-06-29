#ifndef _UI_UIEVENT_H_
#define _UI_UIEVENT_H_

#include "DataDef.h"
#include "Math/Vector.h"

namespace ZGE
{
	struct UIEvent
	{
		UIEvent ();
		virtual ~UIEvent ();
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

#endif // _UI_UIEVENT_H_