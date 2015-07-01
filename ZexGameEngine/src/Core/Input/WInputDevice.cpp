#include "Core/Input/WInputDevice.h"
#include <iostream>
#include <xutility>
#include "Core/Input/InputDeviceStatus.h"

namespace ZGE
{
    WInputMouse::WInputMouse ( HWND hwnd, HANDLE device ) :
        m_Hwnd ( hwnd ),
        m_Device ( device ),
        m_OffsetState ( 0, 0, 0 )
    {
        m_Action.fill ( false );
		m_ButtonState.fill ( false );
    }

    WInputMouse::~WInputMouse ()
    {

    }

    void WInputMouse::OnRawInput ( const RAWINPUT *rawInput )
    {
        if ( rawInput->header.hDevice == m_Device && m_Hwnd == GetForegroundWindow () )
        {
            // Make Sure the Mouse X, Y is true then the Event will be sent always.
            m_Action[ MA_X - MA_ACTIONSTART - 1 ] = true;
            m_Action[ MA_Y - MA_ACTIONSTART - 1 ] = true;

            for ( size_t i = 0; i < 3; ++i )
            {
                if ( rawInput->data.mouse.usButtonFlags & ( 1UL << ( i * 2 + 0 ) ) )
                {
					m_ButtonState[ i ] = true;
                }
                if ( rawInput->data.mouse.usButtonFlags & ( 1UL << ( i * 2 + 1 ) ) )
                {
					m_ButtonState[ i ] = false;
                }
            }

            if ( MOUSE_MOVE_RELATIVE == ( rawInput->data.mouse.usFlags & 1UL ) )
            {
                m_OffsetState.x () = rawInput->data.mouse.lLastX;
                m_OffsetState.y () = rawInput->data.mouse.lLastY;
            }
        }
    }


    void WInputMouse::OnActionMap ( const std::map< U32, U32 > &actionMap, const ActionSignalPtr &signal )
    {
		InputMouseStatus *status = static_cast< InputMouseStatus * > ( m_Status.get () );
		status->Action = m_Actions[ m_Index ];
		status->Offset = m_Offset;
		
        // Check ActionMap
        for ( auto element : actionMap )
        {
            // Check if it is the Mouse Action
            if ( element.second > MA_ACTIONSTART && element.second < MA_ACTIONEND )
            {
				if ( IsButtonDown ( static_cast< MouseAction >( element.second ) ) || IsButtonUp ( static_cast< MouseAction >( element.second ) ) )
                {
                    InputAction inputAction;
                    inputAction.first = element.first;
                    inputAction.second = m_Status;
                    ( *signal )( inputAction );
                }
            }
        }
    }

    void WInputMouse::Update ()
    {
        m_Status.reset ();
		std::copy ( m_ButtonState.begin (), m_ButtonState.end (), m_Action.begin() + 2 );
        m_Index = !m_Index;
        m_Actions[ m_Index ] = m_Action;
        m_Offset = m_OffsetState;
        m_OffsetState = Vector3i ( 0, 0, 0 );
    }

    WInputKeyboard::WInputKeyboard ( HWND hwnd, HANDLE device ) :
        m_Hwnd ( hwnd ),
        m_Device ( device )
    {
        m_Action.fill ( false );
    }

    void WInputKeyboard::OnRawInput ( const RAWINPUT *rawInput )
    {
        if ( rawInput->header.hDevice == m_Device && m_Hwnd == GetForegroundWindow () )
        {
            I32 wVKey = 0x57, sVKey = 0x53, aVKey = 0x41, dVKey = 0x44;
            I32 vKeys[ 4 ];
            vKeys[ 0 ] = wVKey;
            vKeys[ 1 ] = sVKey;
            vKeys[ 2 ] = aVKey;
            vKeys[ 3 ] = dVKey;

            for ( size_t i = 0; i < 4; ++i )
            {
                if ( vKeys[ i ] == rawInput->data.keyboard.VKey )
                {
                    m_Action[ i ] = ( RI_KEY_MAKE == ( rawInput->data.keyboard.Flags & 1UL ) );
                }
            }
        }
    }

    void WInputKeyboard::OnActionMap ( const std::map< U32, U32 > &actionMap, const ActionSignalPtr &signal )
    {
		InputKeyboardStatus *status = static_cast< InputKeyboardStatus * > ( m_Status.get () );
		status->Action = m_Actions[ m_Index ];

        // Check ActionMap
        for ( auto element : actionMap )
        {
            // Check if it is the Mouse Action
            if ( element.second > KA_ACTIONSTART && element.second < KA_ACTIONEND )
            {
				if ( IsKeyDown ( static_cast< KeyboardAction >( element.second ) ) || IsKeyUp ( static_cast< KeyboardAction >( element.second ) ) )
                {
                    InputAction inputAction;
                    inputAction.first = element.first;
                    inputAction.second = m_Status;
                    ( *signal )( inputAction );
                }
            }
        }
    }

    void WInputKeyboard::Update ()
    {
        m_Index = !m_Index;
        m_Actions[ m_Index ] = m_Action;
        m_Action.fill ( false );
    }
}