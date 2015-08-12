#ifndef _CORE_INPUT_INPUTDEVICE_H_
#define _CORE_INPUT_INPUTDEVICE_H_

#include <array>
#include <map>
#include "External/boost/signals2.hpp"
#include "CorePrerequisites.h"
#include "Math/Vector.h"

namespace ZGE
{
    enum KeyboardAction
    {
        KA_ACTIONSTART = 0x00,

        KA_W           = 0x01,
        KA_S           = 0x02,
        KA_A           = 0x03,
        KA_D           = 0x04,

        KA_ACTIONEND,
    };

    #define KEYBOARDACTION_TO_INDEX(action) ( action - KA_ACTIONSTART - 1 )

    #define KEYBOARDACTION_NUM ( KA_ACTIONEND - KA_ACTIONSTART - 1 )

    enum MouseAction
    {
        MA_ACTIONSTART = 0x100,

        MA_X           = 0x101,
        MA_Y           = 0x102,
        MA_LBUTTON     = 0x103,
        MA_RBUTTON     = 0x104,
        MA_MBUTTON     = 0x105,

        MA_ACTIONEND,
    };

    #define MOUSEACTION_TO_INDEX(action) ( action - MA_ACTIONSTART - 1 )

    #define MOUSEACTION_NUM ( MA_ACTIONEND - MA_ACTIONSTART - 1 )

    struct InputDeviceStatus;

    typedef std::pair< U32, std::shared_ptr< const InputDeviceStatus > > InputAction;

    typedef boost::signals2::signal< void ( const InputAction & action ) > ActionSignal;

    typedef std::shared_ptr< ActionSignal > ActionSignalPtr;

    class InputDevice
    {
    public:
        enum InputDeviceType
        {
            IDT_KEYBOARD,
            IDT_MOUSE,
        };

        virtual ~InputDevice () {};

        virtual InputDeviceType DeviceType () const = 0;

        virtual void Update () = 0;

		// First type is Self-Defined Enum, Second type is Engine-Defined Enum.
        virtual void OnActionMap ( const std::map< U32, U32 > &actionMap, const ActionSignalPtr &signal ) = 0;

    protected:
        bool m_Index;

        std::shared_ptr< InputDeviceStatus > m_Status;

    };

    class InputMouse : public InputDevice
    {
    public:
        InputMouse ();

        virtual ~InputMouse () {};

        virtual InputDeviceType DeviceType () const
        {
            return InputDevice::IDT_MOUSE;
        }

		Vector2i Offset () const
		{
			return Vector2i ( m_Offset );
		}

		Vector2i ClientPos () const
		{
			return m_ClientPos;
		}

		Vector2i AbsPos () const
		{
			return m_AbsPos;
		}

        bool LeftButton () const
        {
            return m_Actions[ m_Index ][ MA_LBUTTON - MA_ACTIONSTART - 1 ];
        }

        bool RightButton () const
        {
            return m_Actions[ m_Index ][ MA_RBUTTON - MA_ACTIONSTART - 1 ];
        }

        bool MiddleButton () const
        {
            return m_Actions[ m_Index ][ MA_MBUTTON - MA_ACTIONSTART - 1 ];
        }

        bool IsButtonDown ( const MouseAction &action ) const
        {
            I32 i = action - MouseAction::MA_ACTIONSTART - 1;
            return m_Actions[ m_Index ][ i ];
        }

        bool IsButtonJustDown ( const MouseAction &action ) const
        {
            I32 i = action - MouseAction::MA_ACTIONSTART - 1;
            return ( m_Actions[ m_Index ][ i ] && !m_Actions[ !m_Index ][ i ] );
        }

        bool IsButtonUp ( const MouseAction &action ) const
        {
            I32 i = action - MouseAction::MA_ACTIONSTART - 1;
            return !m_Actions[ m_Index ][ i ];
        }

        bool IsButtonJustUp ( const MouseAction &action ) const
        {
            I32 i = action - MouseAction::MA_ACTIONSTART - 1;
            return ( !m_Actions[ m_Index ][ i ] && m_Actions[ !m_Index ][ i ] );
        }

    protected:
        Vector3i m_Offset;
		Vector2i m_ClientPos;
		Vector2i m_AbsPos;

        // L, R, M
        std::array< std::array< bool, MOUSEACTION_NUM >, 2 > m_Actions;
    };

    class InputKeyboard : public InputDevice
    {
    public:
        InputKeyboard ();

        virtual ~InputKeyboard () {};

        virtual InputDeviceType DeviceType () const
        {
            return InputDevice::IDT_KEYBOARD;
        }

        bool IsKeyDown ( const KeyboardAction &action ) const
        {
            I32 i = action - KeyboardAction::KA_ACTIONSTART - 1;
            return m_Actions[ m_Index ][ i ];
        }

        bool IsKeyJustDown ( const KeyboardAction &action ) const
        {
            I32 i = action - KeyboardAction::KA_ACTIONSTART - 1;
            return ( m_Actions[ m_Index ][ i ] && !m_Actions[ !m_Index ][ i ] );
        }

        bool IsKeyUp ( const KeyboardAction &action ) const
        {
            I32 i = action - KeyboardAction::KA_ACTIONSTART - 1;
            return !m_Actions[ m_Index ][ i ];
        }

        bool IsKeyJustUp ( const KeyboardAction &action ) const
        {
            I32 i = action - KeyboardAction::KA_ACTIONSTART - 1;
            return ( !m_Actions[ m_Index ][ i ] && m_Actions[ !m_Index ][ i ] );
        }

    protected:
        // W, S, A, D
        std::array< std::array< bool, KEYBOARDACTION_NUM >, 2 > m_Actions;

    };
}

#endif