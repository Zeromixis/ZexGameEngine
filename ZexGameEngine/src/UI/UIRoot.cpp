#include "UI/UIRoot.h"
#include "App/Context.h"


namespace ZGE
{
    UIRoot::UIRoot ()
		: m_NowComp ( nullptr )
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

		// InputManager::GetInstance ()->AddActionMap ( actionMap, signal, 0 );
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
			const InputMouse *mouse = dynamic_cast< const InputMouse * > ( inputAction.second );
			Vector2f localPos = mouse->ClientPos ();
			UIComponent *parent = this;
			UIComponent *child = nullptr;

			// Search destination component.
			while ( true )
			{
				child = parent->ComponentAt ( localPos );
				if ( nullptr != child && child->Enabled () )
				{
					parent = child;
					child = nullptr;
					localPos = localPos - child->Position ();
				}
				else
				{
					break;
				}
			}
			if ( nullptr != parent )
			{
				UIComponent *dstComp = parent;

				// Deal with enter and exit
				if ( dstComp != m_NowComp )
				{
					if ( nullptr != m_NowComp )
					{
						UIMouseEvent mouseExitEvent;
						mouseExitEvent.EType = UIMouseEvent::MOUSE_EXIT;
						m_NowComp->OnUIEvent ( &mouseExitEvent );

						UIMouseEvent mouseEnterEvent;
						mouseEnterEvent.EType = UIMouseEvent::MOUSE_ENTER;
						dstComp->OnUIEvent ( &mouseEnterEvent );
					}
				}

				// Deal with press and release
				for ( MouseAction action = MA_LBUTTON; action < MA_ACTIONEND; )
				{
					bool breakFlag = false;
					if ( mouse->IsButtonDown ( action ) )
					{
						UIMouseEvent mousePressEvent;
						mousePressEvent.EType = UIMouseEvent::MOUSE_PRESS;
						dstComp->OnUIEvent ( &mousePressEvent );
						breakFlag = true;
					}
					if ( mouse->IsButtonJustUp ( action ) )
					{
						UIMouseEvent mouseReleaseEvent;
						mouseReleaseEvent.EType = UIMouseEvent::MOUSE_RELEASE;
						dstComp->OnUIEvent ( &mouseReleaseEvent );
						breakFlag = true;
					}
					if ( breakFlag )
					{
						break;
					}
					else
					{
						int t = ( int )action;
						t += 1;
						action = ( MouseAction )t;
					}
				}

				// Deal with Move
				if ( mouse->Offset () != Vector2i ( 0, 0 ) )
				{
					UIMouseEvent mouseMoveEvent;
					mouseMoveEvent.Position = mouseMoveEvent.Position;
					mouseMoveEvent.EType = UIMouseEvent::MOUSE_MOVE;
					dstComp->OnUIEvent ( &mouseMoveEvent );
				}
				m_NowComp = dstComp;

			}
			else
			{

			}


			
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