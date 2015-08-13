#include "Core/CameraController.h"

namespace ZGE
{
    CameraController::CameraController ()
    {
        CameraController ( nullptr );
    }

    CameraController::CameraController ( Camera *camera )
    {
        m_Camera = std::shared_ptr< Camera >( camera );

        // Add ActionMap
        ActionSignalPtr signal = std::make_shared< ActionSignal > ();
        signal->connect ( boost::bind ( &CameraController::OnInput, this, _1 ) );

        std::map< U32, U32 > actionMap;
        actionMap[ RotateLeftRight ] = MA_X;
        actionMap[ RotateUpDown ]    = MA_Y;
        actionMap[ Forward ]         = KA_W;
        actionMap[ Back ]            = KA_S;
        actionMap[ Left ]            = KA_A;
        actionMap[ Right ]           = KA_D;

		// Register ActionMap
        //InputManager::GetInstance ()->AddActionMap ( actionMap, signal );
    }

    CameraController::CameraController ( const CameraController& rhs )
    {
        m_Camera = rhs.m_Camera;
    }

    CameraController::~CameraController ()
    {

    }

    void CameraController::Move ( F32 x, F32 y, F32 z )
    {
        if ( m_Camera )
        {
            auto a = m_Camera->RightVector ();
            Vector3f xUnit = Normalize ( m_Camera->RightVector () );
            Vector3f yUnit = Normalize ( m_Camera->UpVector () );
            Vector3f zUnit = Normalize ( m_Camera->ForwardVector () );

            Vector3f movement = x * xUnit + y * yUnit + z * zUnit;

            const F32 moveScaler = 1.0f;

            movement *= moveScaler;

            Vector3f newEyePos = m_Camera->EyePos () + movement;

            F32 lookAtDistance = Length ( m_Camera->LookAt () - m_Camera->EyePos () );

            m_Camera->SetView ( newEyePos, m_Camera->ForwardVector () * lookAtDistance, m_Camera->UpVector () );
        }
    }

    void CameraController::Rotate ( F32 x, F32 y, F32 z, F32 angle )
    {
		if ( m_Camera && angle != 0.0f )
        {
			F32 c = std::cosf ( angle );
			F32 s = std::sinf ( angle );
			F32 t = 1.0f - c;
			Float44 rotateMat = Float44
			(
				t * x * x + c,     t * x * y - s * z, t * x * z + s * y, 0.0f,
				t * x * y + s * z, t * y * y + c,     t * y * z - s * x, 0.0f,
				t * x * z - s * y, t * y * z + s * x, t * z * z + c,     0.0f,
				0.0f,              0.0f,		      0.0f,				 1
			);

			Vector4f vec = Vector4f ( 0.0f, 0.0f, 0.0f, 0.0f );
			Vector3f lookAtVec = m_Camera->LookAt () - m_Camera->EyePos ();

			vec.x () = lookAtVec.x ();
			vec.y () = lookAtVec.y ();
			vec.z () = lookAtVec.z ();

            //Vector3f newLookAt = static_cast< Vector4f >( m_Camera->LookAt () ) * rotateMat;
			Vector3f zzz = Vector3f ( vec * rotateMat );
			Vector3f newLookAt = m_Camera->EyePos () + zzz;

			m_Camera->SetView ( m_Camera->EyePos (), newLookAt, m_Camera->UpVector() );
        }
    }

    void CameraController::OnInput ( const InputAction& action )
    {
        const F32 length = 0.1f;
        const InputMouse *mouse = dynamic_cast< const InputMouse * >( action.second );
   
        switch ( action.first )
        {
        case RotateLeftRight :
            // If Right Mouse Button is pressed
            if ( mouse->IsButtonDown( MA_RBUTTON ) )
            {
                Rotate ( 0.0f, 1.0f, 0.0f, length / 5.0f * mouse->Offset ().x () );
            }

            break;
        case RotateUpDown :
            // If Right Mouse Button is pressed
            if ( mouse->IsButtonDown ( MA_RBUTTON ) )
            {
                Rotate ( 1.0f, 0.0f, 0.0f, length / 5.0f * mouse->Offset ().y () );
            }

            break;
        case Forward :
            MoveForward ( length );
            break;
        case Back :
            MoveBack ( length );
            break;
        case Left :
            MoveLeft ( length );
            break;
        case Right :
            MoveRight ( length );
            break;
        default:
            break;
        }
    }
}