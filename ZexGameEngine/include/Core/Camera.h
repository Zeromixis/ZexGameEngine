#ifndef _CORE_CAMERA_H_
#define _CORE_CAMERA_H_

#include "ZGEDecl.h"
#include "ZGEDef.h"

#include "Object.h"

#include "Math/Vector.h"
#include "Math/Matrix.h"

namespace ZGE
{
    class Camera
        : public Object
    {
    public:
        Camera ();

        void SetView ( const Vector3f& eyePos, const Vector3f& lookAt );

        void SetView ( const Vector3f& eyePos, const Vector3f& lookAt, const Vector3f& upVector );

        void SetProj ( F32 fov, F32 aspect, F32 nearPlane, F32 farPlane );

        Camera ( const Camera &rhs );
             
        virtual ~Camera ();

        const Vector3f & RightVector () const
        {
            return reinterpret_cast< const Vector3f & > ( m_InvViewMat.RowVector ( 0 ) );
        }

        const Vector3f & UpVector () const
        {
            return reinterpret_cast< const Vector3f & >( m_InvViewMat.RowVector ( 1 ) );
        }

        const Vector3f & ForwardVector () const
        {
            return reinterpret_cast< const Vector3f & >( m_InvViewMat.RowVector ( 2 ) );
        }

        const Float44 & ViewMatrix () const
        {
            return m_ViewMat;
        }

        const Float44 & ProjMatrix () const
        {
            return m_ProjMat;
        }

        const Vector3f & EyePos () const
        {
            return m_EyePos;
        }

        const Vector3f & LookAt () const
        {
            return m_LookAt;
        }

        F32 Fov () const
        {
            return m_Fov;
        }

        F32 Aspect () const
        {
            return m_Aspect;
        }

        F32 NearPlane () const
        {
            return m_NearPlane;
        }

        F32 FarPlane () const
        {
            return m_FarPlane;
        }

    protected:
        Vector3f m_EyePos;
        Vector3f m_LookAt;
        Float44 m_ViewMat;
        Float44 m_ProjMat;
        Float44 m_InvViewMat;
        F32 m_Fov;
        F32 m_Aspect;
        F32 m_NearPlane;
        F32 m_FarPlane;
    };
}


#endif  //_CORE_CAMERA_H_