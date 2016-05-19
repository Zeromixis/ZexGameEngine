#ifndef _CORE_CAMERACONTROLLER_H_
#define _CORE_CAMERACONTROLLER_H_

#include "ZGEDecl.h"

#include "Core/Camera.h"
#include "Input/InputDevice.h"
#include "Core/Input/InputManager.h"

namespace ZGE
{
    class CameraController
        : public Object
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

        CameraController (Camera *camera);

        CameraController (const CameraController& rhs);

        ~CameraController ();

        void Update ();

        const std::shared_ptr< Camera >& GetCamera () const
        {
            return m_Camera;
        }

        void MoveForward (const F32 &length)
        {
            Move (0.0f, 0.0f, length);
        }

        void MoveBack (const F32 &length)
        {
            Move (0.0f, 0.0f, -length);
        }

        void MoveLeft (const F32 &length)
        {
            Move (-length, 0.0f, 0.0f);
        }

        void MoveRight (const F32 &length)
        {
            Move (length, 0.0f, 0.0f);
        }

        void Move (const F32 &x, const F32 &y, const F32 &z);

        void Rotate (const F32 &x, const F32 &y, const F32 &z, const F32 &angle);

    protected:
        std::shared_ptr< Camera > m_Camera;

    private:
        void OnInput (const InputAction& action);

        ActionSignalPtr m_Signal;
    };
}

#endif // !_CORE_CAMERACONTROLLER_H_
