#ifndef _CORE_INPUT_WINPUTDEVICE_H_
#define _CORE_INPUT_WINPUTDEVICE_H_

#include "Core/Input/InputDevice.h"
#include <wtypes.h>

namespace ZGE
{
    class WInputMouse : public InputMouse
    {
    public:
        WInputMouse ( HWND hwnd, HANDLE device );

        virtual ~WInputMouse ();

        virtual void OnRawInput ( const RAWINPUT *rawInput );

        virtual void OnActionMap ( const std::map< U32, U32 > &actionMap, const ActionSignalPtr &signal );

        virtual void Update ();

    protected:
        HWND m_Hwnd;

        HANDLE m_Device;

//      Vector3i m_OffsetState;

		std::array< bool, 3 > m_ButtonState;

        std::array< bool, MOUSEACTION_NUM > m_Action;
    };

    class WInputKeyboard : public InputKeyboard
    {
    public:
        WInputKeyboard ( HWND hwnd, HANDLE device );

		virtual ~WInputKeyboard ();

        virtual void OnRawInput ( const RAWINPUT *rawInput );

        virtual void OnActionMap ( const std::map< U32, U32 > &actionMap, const ActionSignalPtr &signal );

        virtual void Update ();

    protected:
        HWND m_Hwnd;

        HANDLE m_Device;

        std::array< bool, KEYBOARDACTION_NUM > m_Action;
    };
}

#endif