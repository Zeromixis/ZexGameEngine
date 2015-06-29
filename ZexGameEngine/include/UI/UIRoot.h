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
        UIRoot ();

        virtual ~UIRoot ();

    private:
        // I think UIEvent must be first to deal.
        void OnInputEvent ( const InputAction& inputAction );
	};
	

}


#endif