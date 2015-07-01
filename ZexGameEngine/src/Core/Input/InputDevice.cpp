#include "Core/Input/InputDevice.h"
#include "Core/Input/InputDeviceStatus.h"


namespace ZGE
{
    InputMouse::InputMouse ()
    {
        m_Status.reset ( new InputMouseStatus );
    }

    InputKeyboard::InputKeyboard ()
    {
        m_Status.reset ( new InputKeyboardStatus );
    }
}