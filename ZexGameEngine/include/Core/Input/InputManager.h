#ifndef _CORE_INPUT_INPUTMANAGER_H_
#define _CORE_INPUT_INPUTMANAGER_H_

#include <vector>
#include <memory>
#include <map>
#include "Pattern/Singleton.h"
#include "Core/Input/InputDevice.h"
#include "App/WindowWin.h"

namespace ZGE
{
    class InputManager : public Singleton< InputManager >
    {
        friend class Singleton< InputManager >;
    public:
        ~InputManager () {};

        //void AddInputDevice ( std::shared_ptr< InputDevice > inputDevice );

        //void RemoveInputDevice ();

        size_t DevicesNum ();

        void Update ();

        void OnRawInput ( const Window &window, HRAWINPUT rawInput );

        void AddActionMap ( std::map< U32, U32 > actionMap, ActionSignalPtr actionSignal );

    protected:
        InputManager ();

        std::vector< std::shared_ptr< InputDevice > > m_Devices;

        std::vector< std::pair< std::map< U32, U32 >, ActionSignalPtr > > m_ActionMaps;

    };
}

#endif