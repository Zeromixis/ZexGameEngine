#include "Core/Camera.h"
#include "Math/MathFunc.h"

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
		Vector3f zAxis = MathFunc::Normalize ( m_LookAt - m_EyePos );
		Vector3f xAxis = MathFunc::Normalize ( MathFunc::Cross ( vUp, zAxis ) );
		Vector3f yAxis = MathFunc::Cross ( zAxis, xAxis );

		m_ViewMat = Float44
		(
			xAxis.x (), yAxis.x (), zAxis.x (), 0,
			xAxis.y (), yAxis.y (), zAxis.y (), 0,
			xAxis.z (), yAxis.z (), zAxis.z (), 0,
			-MathFunc::Dot ( xAxis, eyePos ), -MathFunc::Dot ( yAxis, eyePos ), -MathFunc::Dot ( zAxis, eyePos ), 1
		);

		//////////////////////////////////////////////////////////////////////////

		m_InvViewMat = MathFunc::MatrixInverse ( m_ViewMat );
	}

	void Camera::SetProj ( F32 fov, F32 aspect, F32 nearPlane, F32 farPlane )
	{
		m_Fov       = fov;
		m_Aspect    = aspect;
		m_NearPlane = nearPlane;
		m_FarPlane  = farPlane;

		// Set Projection Matrix
		m_ProjMat = MathFunc::PerspectiveFovLH ( m_Fov, m_Aspect, m_NearPlane, m_FarPlane );

		// OpenGL adjust
		m_ProjMat = m_ProjMat * Float44
			(
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 2, 0,
				0, 0, -1, 1
			);
	}
}
