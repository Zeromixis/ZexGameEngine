#ifndef _UI_UIROOT_H_
#define _UI_UIROOT_H_

#include "UIComponent.h"
#include "Pattern/Singleton.h"
#include "Core/Input/InputDevice.h"

namespace ZGE
{
	class UIRoot
		: public UIComponent
		, public Singleton < UIRoot >
	{
		friend class Singleton < UIRoot >;
    public:
		enum
		{
			UIEVENT_MOUSE,
			UIEVENT_KEYBOARD,
		};


        UIRoot ();

        virtual ~UIRoot ();

    private:
        void OnInputEvent ( const InputAction &inputAction );
	};
	

}


#endif