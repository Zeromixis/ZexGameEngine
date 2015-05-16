#include "Core/Input/InputManager.h"
#include "Core/Input/WInputDevice.h"
#include "App/Context.h"
#include <hidusage.h>

namespace ZGE
{
    InputManager::InputManager ()
    {
        // Init and Register RawInputDevice in Windows.

        Window *window = Context::GetInstance ()->GetWindowPtr ().get ();
        WindowWin *windowWin = static_cast< WindowWin * >( window );
        HWND hwnd = windowWin->Hwnd ();

        UINT numDevices;

        GetRawInputDeviceList ( nullptr, &numDevices, sizeof( RAWINPUTDEVICELIST ) );

        PRAWINPUTDEVICELIST rawInputDeviceList = new RAWINPUTDEVICELIST[ numDevices ];

        GetRawInputDeviceList ( rawInputDeviceList, &numDevices, sizeof( RAWINPUTDEVICELIST ) );

        RAWINPUTDEVICE rawInputDevice;
        rawInputDevice.usUsagePage = HID_USAGE_PAGE_GENERIC;
        rawInputDevice.hwndTarget = hwnd;

        std::vector< RAWINPUTDEVICE > rawInputDevices;

        for ( size_t i = 0; i < numDevices; ++i )
        {
            PRAWINPUTDEVICELIST deviceList = rawInputDeviceList + i;
            std::shared_ptr< InputDevice > device = nullptr;
            switch ( deviceList->dwType )
            {
            case RIM_TYPEMOUSE:
                device = std::make_shared< WInputMouse > ( hwnd, deviceList->hDevice );
                rawInputDevice.usUsage = HID_USAGE_GENERIC_MOUSE;
                rawInputDevice.dwFlags = 0;
                rawInputDevice.hwndTarget = hwnd;
                rawInputDevices.push_back ( rawInputDevice );
                break;
            case RIM_TYPEKEYBOARD:
                device = std::make_shared< WInputKeyboard > ( hwnd, deviceList->hDevice );
                rawInputDevice.usUsage = HID_USAGE_GENERIC_KEYBOARD;
                rawInputDevice.dwFlags = 0;
                rawInputDevice.hwndTarget = hwnd;
                rawInputDevices.push_back ( rawInputDevice );
                break;
            case RIM_TYPEHID:
            {
                // TODO: Add Other Type Device.
                int i = 0;
                break;
            }
            default:
                break;
            }
			if ( nullptr != device )
			{
				m_Devices.push_back ( device );
			}
        }

        delete[] rawInputDeviceList;

        
        if ( RegisterRawInputDevices ( &rawInputDevices[ 0 ], rawInputDevices.size (), sizeof ( rawInputDevices[ 0 ] ) ) )
        {
            //////////////////////////////////////////////////////////////////////////

            // Connect Signal to Slot

            windowWin->OnRawInput ().connect ( boost::bind ( &InputManager::OnRawInput, this, _1, _2 ) );

            //////////////////////////////////////////////////////////////////////////
        }

    }

    void InputManager::OnRawInput ( const Window& window, HRAWINPUT rawInput )
    {
        const WindowWin& windowWin = static_cast< const WindowWin& >( window );

        UINT dwSize;

        GetRawInputData ( rawInput, RID_INPUT, nullptr, &dwSize, sizeof( RAWINPUTHEADER ) );

        LPBYTE lpb = new BYTE[ dwSize ];

        GetRawInputData ( rawInput, RID_INPUT, lpb, &dwSize, sizeof( RAWINPUTHEADER ) );

        RAWINPUT *raw = reinterpret_cast< RAWINPUT * >( lpb );
        
        switch ( raw->header.dwType )
        {
        case RIM_TYPEMOUSE:
            for ( size_t i = 0; i < m_Devices.size (); ++i )
            {
                if ( InputDevice::IDT_MOUSE == m_Devices[ i ]->DeviceType () )
                {
                #ifdef _WIN32
                    WInputMouse *wInputMouse = static_cast< WInputMouse * >( m_Devices[ i ].get () );
                    wInputMouse->OnRawInput ( raw );
                #endif
                }
            }
            break;
        case RIM_TYPEKEYBOARD:
            for ( size_t i = 0; i < m_Devices.size (); ++i )
            {
                if ( InputDevice::IDT_KEYBOARD == m_Devices[ i ]->DeviceType () )
                {
                #ifdef _WIN32
                    WInputKeyboard *wInputKeyboard = static_cast< WInputKeyboard * >( m_Devices[ i ].get () );
                    wInputKeyboard->OnRawInput ( raw );
                #endif
                }
            }
            break;
        case RIM_TYPEHID:
            // TODO: Add Other Type Device.

            break;
        default:
            break;
        }

        delete[] lpb;
    }

    size_t InputManager::DevicesNum ()
    {
        return m_Devices.size ();
    }

    void InputManager::Update ()
    {      
        for ( auto device : m_Devices )
        {
            device->Update ();
        }

        for ( auto &action : m_ActionMaps )
        {
            const std::map< U32, U32 > &actionMap = action.first;
            for ( auto &element : actionMap )
            {
                if ( element.second > KA_ACTIONSTART && element.second < KA_ACTIONEND )
                {
                    for ( auto device : m_Devices )
                    {
                        if ( device->DeviceType () == InputDevice::IDT_KEYBOARD )
                        {
                            device->OnActionMap ( actionMap, action.second );
                        }
                    }
                }
                else if ( element.second > MA_ACTIONSTART && element.second < MA_ACTIONEND )
                {
                    for ( auto device : m_Devices )
                    {
                        if ( device->DeviceType () == InputDevice::IDT_MOUSE )
                        {
                            device->OnActionMap ( actionMap, action.second );
                        }
                    }
                }
            }
        } 
    }

    void InputManager::AddActionMap ( std::map< U32, U32 > actionMap, ActionSignalPtr actionSignal )
    {
        m_ActionMaps.push_back ( std::make_pair ( actionMap, actionSignal ) );
    }
}
