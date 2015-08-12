#ifndef _CORE_INPUT_INPUTMANAGER_H_
#define _CORE_INPUT_INPUTMANAGER_H_

#include <vector>
#include <memory>
#include <map>
#include <tuple>
#include "Pattern/Singleton.h"
#include "Core/Input/InputDevice.h"
#include "App/WindowWin.h"

namespace ZGE
{
    class InputManager : public Singleton< InputManager >
    {
        friend class Singleton< InputManager >;
    public:
        enum UsualPriority
        {
            High    = 0,
            Middle  = 3,
            Low     = 6,
            Last    = 9,
        };

        ~InputManager () {};

        //void AddInputDevice ( std::shared_ptr< InputDevice > inputDevice );

        //void RemoveInputDevice ();

        size_t DevicesNum ();

        void Update ();

        void OnRawInput ( const Window &window, HRAWINPUT rawInput );

		// ActionMap first type is Engine-Defined Key Enum, second type is Self-Defined Enum.
        void AddActionMap ( std::map< U32, U32 > actionMap, ActionSignalPtr actionSignal, int priority );

        void BlockPropagate () { m_IsBlocked = true; }

    protected:
        InputManager ();

        std::vector< std::shared_ptr< InputDevice > > m_Devices;

        typedef std::tuple< std::map< U32, U32 >, ActionSignalPtr, int > ActionTuple;

        std::vector< ActionTuple > m_ActionMaps;

        bool m_IsBlocked;

    };
}

#endif