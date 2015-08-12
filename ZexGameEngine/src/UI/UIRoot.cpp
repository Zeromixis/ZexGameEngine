#include "UI/UIRoot.h"
#include "App/Context.h"
#include "Core/Input/InputDeviceStatus.h"


namespace ZGE
{
    UIRoot::UIRoot ()
    {
        m_Node.Position () = Vector2f ( 0.0f, 0.0f );
		m_Width = Context::GetInstance ()->GetWindowPtr ()->Width ();
		m_Height = Context::GetInstance ()->GetWindowPtr ()->Height ();

		// All Input Enum need to be add, so we can detect.
		std::map < U32, U32 > actionMap;

		for ( auto i = MA_ACTIONSTART + 1; i < MA_ACTIONEND; ++i )
		{
			actionMap[ i ] = UIEVENT_MOUSE;
		}

		ActionSignalPtr signal = std::make_shared< ActionSignal > ();
		signal->connect ( boost::bind ( &UIRoot::OnInputEvent, this, _1 ) );

		InputManager::GetInstance ()->AddActionMap ( actionMap, signal, 0 );
    }

    UIRoot::~UIRoot ()
    {

    }

    void UIRoot::OnInputEvent ( const InputAction &inputAction )
    {
		switch ( inputAction.first )
		{
		case UIEVENT_MOUSE:
		{
			const InputMouseStatus *status = dynamic_cast< const InputMouseStatus * >( inputAction.second.get () );
			Vector2i pos = status->ClientPos;




			break;
		}
		case UIEVENT_KEYBOARD:
		{
			break;
		}
		default:
			break;
		}

    }

}