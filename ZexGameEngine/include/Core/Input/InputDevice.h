#ifndef _CORE_INPUT_INPUTDEVICE_H_
#define _CORE_INPUT_INPUTDEVICE_H_

#include <array>
#include <map>
#include "External/boost/signals2.hpp"
#include "DataDef.h"
#include "Math/Vector.h"
#include "Core/Input/InputDeviceStatus.h"

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

		// First type is Self-defined Enum, Second type is Engine-defined Enum.
        virtual void OnActionMap ( const std::map< U32, U32 > &actionMap, const ActionSignalPtr &signal ) = 0;

    protected:
        bool m_Index;

        std::shared_ptr< InputDeviceStatus > m_Status;

    };

    struct InputDeviceStatus
    {
        virtual ~InputDeviceStatus ()
        {

        }

        virtual InputDevice::InputDeviceType DeviceType () const = 0;
    };



    class InputMouse : public InputDevice
    {
    public:
        enum { ACTIONUMS = MouseAction::MA_ACTIONEND - MouseAction::MA_ACTIONSTART - 1 };

		InputMouse ()
		{
			m_Status.reset ( new InputMouseStatus );
		}

        virtual ~InputMouse () {};

        virtual InputDeviceType DeviceType () const
        {
            return InputDevice::IDT_MOUSE;
        }

        I32 X () const
        {
            return m_Offset.x ();
        }

        I32 Y () const
        {
            return m_Offset.y ();
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

        bool IsButtonDown ( const MouseAction& action ) const
        {
            I32 i = action - MouseAction::MA_ACTIONSTART - 1;
            return m_Actions[ m_Index ][ i ];
        }

        bool IsButtonUp ( const MouseAction& action ) const
        {
            I32 i = action - MouseAction::MA_ACTIONSTART - 1;
            return !m_Actions[ m_Index ][ i ];
        }

    protected:
        Vector3i m_Offset;

        // L, R, M
        std::array< std::array< bool, ACTIONUMS >, 2 > m_Actions;
    };

    class InputKeyboard : public InputDevice
    {
    public:
        enum { ACTIONUMS = KeyboardAction::KA_ACTIONEND - KeyboardAction::KA_ACTIONSTART - 1 };

		InputKeyboard ()
		{
			m_Status.reset ( new InputKeyboard );
		}

        virtual ~InputKeyboard () {};

        virtual InputDeviceType DeviceType () const
        {
            return InputDevice::IDT_KEYBOARD;
        }

        bool IsKeyDown ( const KeyboardAction& action ) const
        {
            I32 i = action - KeyboardAction::KA_ACTIONSTART - 1;
            return m_Actions[ m_Index ][ i ];
        }

        bool IsKeyUp ( const KeyboardAction& action ) const
        {
            I32 i = action - KeyboardAction::KA_ACTIONSTART - 1;
            return !m_Actions[ m_Index ][ i ];
        }

    protected:
        // W, S, A, D
        std::array< std::array< bool, ACTIONUMS >, 2 > m_Actions;

    };


    struct InputMouseStatus : public InputDeviceStatus
    {
        Vector3i Offset;
        std::array< bool, InputMouse::ACTIONUMS > Action;

        virtual InputDevice::InputDeviceType DeviceType ( ) const
        {
            return InputDevice::IDT_MOUSE;
        }
    };

    struct InputKeyboardStatus : public InputDeviceStatus
    {
        std::array< bool, InputKeyboard::ACTIONUMS > Action;

        virtual InputDevice::InputDeviceType DeviceType () const
        {
            return InputDevice::IDT_KEYBOARD;
        }
    };
}

#endif