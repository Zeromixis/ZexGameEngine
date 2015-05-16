#ifndef _CORE_CAMERACONTROLLER_H_
#define _CORE_CAMERACONTROLLER_H_

#include "Core/Camera.h"
#include <memory>
#include "Input/InputDevice.h"
#include "Core/Input/InputManager.h"

namespace ZGE
{
    class CameraController
    {
    public:
        enum
        {
            RotateLeftRight,
            RotateUpDown,
            Forward,
            Back,
            Left,
            Right,
        };

        CameraController ();

        CameraController ( Camera *camera );

        CameraController ( const CameraController& rhs );

        ~CameraController ();

        void Update ();

        const std::shared_ptr< Camera >& GetCamera () const
        {
            return m_Camera;
        }

        void MoveForward ( F32 length )
        {
            Move ( 0.0f, 0.0f, length );
        }

        void MoveBack ( F32 length )
        {
            Move ( 0.0f, 0.0f, -length );
        }
       
        void MoveLeft ( F32 length )
        {
            Move ( -length, 0.0f, 0.0f );
        }

        void MoveRight ( F32 length )
        {
            Move ( length, 0.0f, 0.0f );
        }

        void Move ( F32 x, F32 y, F32 z );

        void Rotate ( F32 x, F32 y, F32 z, F32 angle );

    protected:
        std::shared_ptr< Camera > m_Camera;

    private:
        void OnInput ( const InputAction& action );
    };
}

#endif // !_CORE_CAMERACONTROLLER_H_
