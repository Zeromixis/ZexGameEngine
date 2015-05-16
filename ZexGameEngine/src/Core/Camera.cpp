#include "Core/Camera.h"
#include <cmath>

namespace ZGE
{
    Camera::Camera ()
    {

    }

    Camera::Camera ( const Camera &rhs )
    {
        
    }

    Camera::~Camera ()
    {

    }

    void Camera::SetView ( const Vector3f& eyePos, const Vector3f& lookAt )
    {
        SetView ( eyePos, lookAt, Vector3f ( 0.0f, 1.0f, 0.0f ) );
    }

    void Camera::SetView ( const Vector3f& eyePos, const Vector3f& lookAt, const Vector3f& upVector )
    {
        m_EyePos = eyePos;
        m_LookAt = lookAt;

        // Set View Matrix

        Vector3f vUp = upVector;
        Vector3f zAxis = Normalize ( m_LookAt - m_EyePos );
        Vector3f xAxis = Normalize ( Cross ( vUp, zAxis ) );
        Vector3f yAxis = Cross ( zAxis, xAxis );

        m_ViewMat = Float44
        (
            xAxis.x (), yAxis.x (), zAxis.x (), 0,
            xAxis.y (), yAxis.y (), zAxis.y (), 0,
            xAxis.z (), yAxis.z (), zAxis.z (), 0,
            -Dot ( xAxis, eyePos ), -Dot ( yAxis, eyePos ), -Dot ( zAxis, eyePos ), 1
        );

        //////////////////////////////////////////////////////////////////////////

        m_InvViewMat = Inverse ( m_ViewMat );
    }

    void Camera::SetProj ( F32 fov, F32 aspect, F32 nearPlane, F32 farPlane )
    {
        m_Fov = fov;
        m_Aspect = aspect;
        m_NearPlane = nearPlane;
        m_FarPlane = farPlane;

        // Set Projection Matrix

        typedef Float44::value_type valType;
        const valType h ( 1.0f / tan ( m_Fov / 2.0f ) );
        const valType w ( h / m_Aspect );
        const valType q ( m_FarPlane / ( m_FarPlane - m_NearPlane ) );
        m_ProjMat = Float44
        (
            w, 0, 0,                0,
            0, h, 0,                0,
            0, 0, q,                1,
            0, 0, -m_NearPlane * q, 0
        );   

        //////////////////////////////////////////////////////////////////////////
    }
}
