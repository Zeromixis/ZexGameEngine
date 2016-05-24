#include "Core/CameraController.h"

namespace ZGE
{
    CameraController::CameraController ()
    {
        CameraController (nullptr);
    }

    CameraController::CameraController (Camera *camera)
    {
        m_Camera = std::shared_ptr< Camera > (camera);

        // Add ActionMap
        m_Signal = std::make_shared< ActionSignal > ();
        m_Signal->connect (boost::bind (&CameraController::OnInput, this, _1));

        std::map< U32, U32 > actionMap;
        actionMap [RotateLeftRight] = MA_X;
        actionMap [RotateUpDown] = MA_Y;
        actionMap [Forward] = KA_W;
        actionMap [Back] = KA_S;
        actionMap [Left] = KA_A;
        actionMap [Right] = KA_D;

        // Register ActionMap
        InputManager::GetInstance ()->AddActionMap (actionMap, m_Signal, 0);
    }

    CameraController::CameraController (const CameraController& rhs)
    {
        m_Camera = rhs.m_Camera;
    }

    CameraController::~CameraController ()
    {
        InputManager::GetInstance ()->RemoveActionMap (m_Signal);
    }

    void CameraController::Move (const F32 & x, const F32 & y, const F32 &z)
    {
        if (m_Camera)
        {
            auto a = m_Camera->RightVector ();
            Vector3f xUnit = MathFunc::Normalize (m_Camera->RightVector ());
            Vector3f yUnit = MathFunc::Normalize (m_Camera->UpVector ());
            Vector3f zUnit = MathFunc::Normalize (m_Camera->ForwardVector ());

            Vector3f movement = x * xUnit + y * yUnit + z * zUnit;

            const F32 moveScaler = 0.3f;

            movement *= moveScaler;

            Vector3f newEyePos = m_Camera->EyePos () + movement;

            F32 lookAtDistance = (m_Camera->LookAt () - m_Camera->EyePos ()).Length ();

            m_Camera->SetView (newEyePos, m_Camera->ForwardVector () * lookAtDistance, m_Camera->UpVector ());
        }
    }

    void CameraController::Rotate (const F32 &x, const F32 &y, const F32 &z, const F32 &angle)
    {
        if (m_Camera && angle != 0.0f)
        {
            F32 c = std::cosf (angle);
            F32 s = std::sinf (angle);
            F32 t = 1.0f - c;
            Float44 rotateMat = Float44
            (
                t * x * x + c, t * x * y - s * z, t * x * z + s * y, 0.0f,
                t * x * y + s * z, t * y * y + c, t * y * z - s * x, 0.0f,
                t * x * z - s * y, t * y * z + s * x, t * z * z + c, 0.0f,
                0.0f, 0.0f, 0.0f, 1
            );

            Vector4f vec = Vector4f (0.0f, 0.0f, 0.0f, 0.0f);
            Vector3f lookAtVec = m_Camera->LookAt () - m_Camera->EyePos ();

            vec.x () = lookAtVec.x ();
            vec.y () = lookAtVec.y ();
            vec.z () = lookAtVec.z ();

            Vector3f zzz = Vector3f (vec * rotateMat);
            Vector3f newLookAt = m_Camera->EyePos () + zzz;

            m_Camera->SetView (m_Camera->EyePos (), newLookAt, m_Camera->UpVector ());
        }
    }

    void CameraController::OnInput (const InputAction &action)
    {
        const F32 length = 0.1f;
        const InputMouse *mouse = dynamic_cast<const InputMouse *>(action.second);

        switch (action.first)
        {
        case RotateLeftRight:
            // If Right Mouse Button is pressed
            if (mouse->IsButtonDown (MA_RBUTTON))
            {
                auto cameraUpVec = m_Camera->UpVector ();
                MathFunc::Normalize (cameraUpVec);
                Rotate (cameraUpVec.x (), cameraUpVec.y (), cameraUpVec.z (), length / 5.0f * mouse->Offset ().x () / 20.0f);
            }

            break;
        case RotateUpDown:
            // If Right Mouse Button is pressed
            if (mouse->IsButtonDown (MA_RBUTTON))
            {
                auto cameraRightVec = m_Camera->RightVector ();
                MathFunc::Normalize (cameraRightVec);
                Rotate (cameraRightVec.x (), cameraRightVec.y (), cameraRightVec.z (), length / 5.0f * mouse->Offset ().y () / 20.0f);
            }

            break;
        case Forward:
            MoveForward (length);
            break;
        case Back:
            MoveBack (length);
            break;
        case Left:
            MoveLeft (length);
            break;
        case Right:
            MoveRight (length);
            break;
        default:
            break;
        }
    }
}