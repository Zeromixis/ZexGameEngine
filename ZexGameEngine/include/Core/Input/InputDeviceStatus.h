#ifndef _CORE_INPUTDEVICESTATUS_H_
#define _CORE_INPUTDEVICESTATUS_H_

#include "InputDevice.h"

namespace ZGE
{

    struct InputDeviceStatus
    {
        virtual ~InputDeviceStatus ()
        {

        }

        virtual InputDevice::InputDeviceType DeviceType () const = 0;
    };


    struct InputMouseStatus : public InputDeviceStatus
    {
        Vector3i Offset;
        std::array< bool, MOUSEACTION_NUM > Action;

        virtual InputDevice::InputDeviceType DeviceType () const
        {
            return InputDevice::IDT_MOUSE;
        }
    };


    struct InputKeyboardStatus : public InputDeviceStatus
    {
        std::array< bool, KEYBOARDACTION_NUM > Action;

        virtual InputDevice::InputDeviceType DeviceType () const
        {
            return InputDevice::IDT_KEYBOARD;
        }
    };




}

#endif