/* 

Amine Rehioui
Created: May 9th 2010

*/

#include "Shoot.h"

#include "Camera.h"

// type literals
const char* const g_strCamera3DTypes[] =
{
	"Perspective",
	"Orthogonal3D",
	"Orthogonal2D",
	0
};

namespace shoot
{
	DEFINE_OBJECT(Camera);

	//! constructor
	Camera::Camera()
	: m_bSet(false)
	// properties
	, m_FOV(45.0f)
	, m_ZNear(0.5f)
	, m_ZFar(300.0f)
	, m_eType(Type_Perspective)
	, m_vUpVector()
	, m_bProjectionMatrixDirty(true)
	, m_bViewMatrixDirty(true)
	, m_bFrustumDirty(true)
	, m_TargetResolution(480, 640)
	{
		m_vUpVector.Set(0.0f, 0.0f, 1.0f);
		m_vPosition = Vector3::Zero;
		m_vLookAt = Vector3::Zero;
	}

	//! serializes the entity to/from a PropertyStream
	void Camera::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);

		stream.Serialize(PT_Float, "FOV", &m_FOV);
		stream.Serialize(PT_Float, "ZNear", &m_ZNear);
		stream.Serialize(PT_Float, "ZFar", &m_ZFar);
		stream.Serialize(PT_Enum, "Projection", &m_eType, ENUM_PARAMS1(g_strCamera3DTypes));
		stream.Serialize(PT_Vec3, "UpVector", &m_vUpVector);
		stream.Serialize(PT_Vec3, "LookAt", &m_vLookAt);
		stream.Serialize(PT_Size, "TargetResolution", &m_TargetResolution);		

		m_bProjectionMatrixDirty = true;
		m_bViewMatrixDirty = true;
		m_bFrustumDirty = true;
	}

	//! registers the entity and its children for rendering
	void Camera::RegisterForRendering()
	{
		if(!m_bSet)
		{
			switch(m_eType)
			{
			case Type_Perspective:
			case Type_Ortho3D:
				EntityRenderer::Instance()->Set3DCamera(this);
				break;

			case Type_Ortho2D:
				EntityRenderer::Instance()->Set2DCamera(this);
				break;
			}
			m_bSet = true;
		}
	}

	//! update the projection matrix
	const Matrix44& Camera::UpdateProjectionMatrix()
	{
		const Vector2& vScreenSize = GraphicsDriver::Instance()->GetViewPort().Size();
		switch(m_eType)
		{
		case Type_Perspective:
			m_ProjectionMatrix.BuildPerspectiveProjection(m_FOV, vScreenSize.X/vScreenSize.Y, m_ZNear, m_ZFar);
			break;

		case Type_Ortho3D:
			// TODO			
			break;

		case Type_Ortho2D:
			m_ProjectionMatrix.Build2DOrthographicProjection(vScreenSize.X, vScreenSize.Y);
			break;
		}
		m_bProjectionMatrixDirty = false;
		return m_ProjectionMatrix;
	}

	//! update the view matrix
	const Matrix44& Camera::UpdateViewMatrix(bool bReturnInverse /*= false*/)
	{
		switch(m_eType)
		{
		case Type_Perspective:
			m_ViewMatrix.BuildLookAtLH(m_vPosition, m_vLookAt, m_vUpVector);
			break;

		case Type_Ortho3D:
			// TODO
			break;

		case Type_Ortho2D:
			// stretch to preserve m_TargetResolution Ratio
			Vector2 vScreenSize = GraphicsDriver::Instance()->GetViewPort().Size();
			f32 screenRatio = vScreenSize.X/vScreenSize.Y;
			f32 targetRatio = f32(m_TargetResolution.Width)/m_TargetResolution.Height;
			Vector2 targetSize = vScreenSize;
			m_v2DOffset = Vector2();
			if(screenRatio > targetRatio)
			{
				targetSize.X = targetSize.Y * targetRatio;
				m_v2DOffset.X = (vScreenSize.X-targetSize.X)/2.0f;
			}
			else
			{
				targetSize.Y = targetSize.X / targetRatio;
				m_v2DOffset.Y = (vScreenSize.Y-targetSize.Y)/2.0f;
			}
			m_ViewMatrix = Matrix44::Identity;
			m_ViewMatrix.Scale(Vector3::Create(targetSize.X/m_TargetResolution.Width*m_vScale.X, targetSize.Y/m_TargetResolution.Height*m_vScale.Y, 1.0f));
			m_ViewMatrix.Translate(Vector3::Create(m_v2DOffset.X+m_vPosition.X, m_v2DOffset.Y+m_vPosition.Y, 0.0f));
			break;
		}
		m_ViewMatrix.GetInverse(m_ViewMatrixInverse);
		m_bViewMatrixDirty = false;
		return bReturnInverse ? m_ViewMatrixInverse : m_ViewMatrix;
	}

	//! updates the view frustum
	const Frustum& Camera::UpdateFrustum()
	{
		const Vector2& vScreenSize = GraphicsDriver::Instance()->GetViewPort().Size();
		m_Frustum = Frustum(m_FOV, vScreenSize.X/vScreenSize.Y, m_ZNear, m_ZFar, m_vPosition, m_vLookAt, m_vUpVector);
		m_bFrustumDirty = false;
		return m_Frustum;
	}

	//! calculates a 3D ray from screen coordinates
	/** the ray is from the camera position to a far plane position pointed by the screen coordinates */
	void Camera::Get3DRayFromScreenCoords(Vector3& rayStart, Vector3& rayDir, const Point& coords, const Vector2& vScreenSize)
	{
		// uses ToyMaker's method - http://www.toymaker.info/Games/html/picking.html
		const Matrix44& projMatrix = GetProjectionMatrix();

		Vector3 v;
		v.X =  ( ( ( 2.0f * coords.X ) / vScreenSize.X ) - 1.0f ) / projMatrix(0, 0);
		v.Y = -( ( ( 2.0f * coords.Y ) / vScreenSize.Y ) - 1.0f ) / projMatrix(1, 1);
		v.Z = -1.0f;

		const Matrix44& m = m_ViewMatrixInverse;
		// Transform the screen space pick ray into 3D space
		rayDir.X  = v.X*m(0, 0) + v.Y*m(1, 0) + v.Z*m(2, 0);
		rayDir.Y  = v.X*m(0, 1) + v.Y*m(1, 1) + v.Z*m(2, 1);
		rayDir.Z  = v.X*m(0, 2) + v.Y*m(1, 2) + v.Z*m(2, 2);
		rayStart  = m_vPosition;
	}	
}

