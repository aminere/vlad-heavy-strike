/* 

Amine Rehioui
Created: May 8th 2010

*/

#ifndef _CAMERA_H_INCLUDED_
#define _CAMERA_H_INCLUDED_

#include "Frustum.h"

namespace shoot
{
	//! Camera entity
	/** Note: the center of the entity is considered as the look at point, and the Position as the eye point. */
	class Camera : public Entity3D				    			   
	{
	public:

		//! Macro to provide type information
		DECLARE_OBJECT(Camera, Entity3D);

		//! constructor
		Camera();

		//! serializes the entity to/from a PropertyStream
		virtual void Serialize(PropertyStream& stream);

		//! registers the entity and its children for rendering
		virtual void RegisterForRendering();

		//! returns the projection matrix
		inline const Matrix44& GetProjectionMatrix() { return m_bProjectionMatrixDirty ? UpdateProjectionMatrix() : m_ProjectionMatrix; }

		//! returns the view matrix
		inline const Matrix44& GetViewMatrix() { return m_bViewMatrixDirty ? UpdateViewMatrix() : m_ViewMatrix; }		

		//! returns the view matrix inverse
		inline const Matrix44& GetViewMatrixInverse() { return m_bViewMatrixDirty ? UpdateViewMatrix(true) : m_ViewMatrixInverse; }
		
		//! returns the frustum
		inline const Frustum& GetFrustum() { return m_bFrustumDirty ? UpdateFrustum() : m_Frustum; }

		//! Sets the position relative to the parent's position
		virtual void SetPosition(const Vector3& vPos)
		{ 
			super::SetPosition(vPos);
			m_bViewMatrixDirty = true;
			m_bFrustumDirty = true;
		}	

		//! Translates the entity
		virtual void Translate(const Vector3& amount)
		{
			super::Translate(amount);
			m_bViewMatrixDirty = true;
			m_bFrustumDirty = true;
		}

		//! sets the scaling vector
		virtual void SetScale(const Vector3& vScale) 
		{ 
			super::SetScale(vScale);
			m_bViewMatrixDirty = true;
			m_bFrustumDirty = true;
		}

		//! returns the Up vector
		inline const Vector3& GetUpVector() const { return m_vUpVector; }

		//! sets the Up vector
		inline void SetUpVector(const Vector3& vUp)
		{ 
			m_vUpVector = vUp;
			m_bViewMatrixDirty = true;
			m_bFrustumDirty = true;
		}

		//! returns the look at point
		inline const Vector3& GetLookAt() const { return m_vLookAt; }

		//! sets the look at
		inline void SetLookAt(const Vector3& vLookAt)
		{ 
			m_vLookAt = vLookAt;
			m_bViewMatrixDirty = true;
			m_bFrustumDirty = true;
		}

		//! returns the camera FOV
		inline f32 GetFOV() const { return m_FOV; }

		//! camera types
		enum E_Type
		{
			Type_Perspective,
			Type_Ortho3D,
			Type_Ortho2D
		};		

		//! returns the camera type
		E_Type GetType() const { return m_eType; }

		//! set the camera type
		void SetType(E_Type eType)
		{
			m_eType = eType;
			m_bProjectionMatrixDirty = true;
			m_bViewMatrixDirty = true;
			m_bFrustumDirty = true;
		}

		//! sets the projection matrix dirty flag
		inline void SetProjectionMatrixDirty() { m_bProjectionMatrixDirty = true; }

		//! sets the view matrix dirty flag
		inline void SetViewMatrixDirty() 
		{ 
			m_bViewMatrixDirty = true;
			m_bFrustumDirty = true;
		}

		//! calculates a 3D ray from screen coordinates
		/** the ray is from the camera position to a far plane position pointed by the screen coordinates */
		void Get3DRayFromScreenCoords(Vector3& rayStart, Vector3& rayDir, const Point& coords, const Vector2& vScreenSize);

		//! returns the 2D offset
		inline const Vector2& Get2DOffset() const { return m_v2DOffset; }

		//! sets the dist to the near plane
		inline void SetZNear(f32 fZnear) { m_ZNear = fZnear; m_bProjectionMatrixDirty = true; m_bFrustumDirty = true; }

		//! sets the dist to the far plane
		inline void SetZFar(f32 fZFar) { m_ZFar = fZFar; m_bProjectionMatrixDirty = true; m_bFrustumDirty = true; }

		//! returns the target resolution
		inline const Size& GetTargetResolution() const { return m_TargetResolution; }

		//! sets this camera for rendering
		inline void Activate() { m_bSet = false; }

	protected:

		Matrix44 m_ProjectionMatrix;
		Matrix44 m_ViewMatrix;
		Matrix44 m_ViewMatrixInverse;
		Frustum m_Frustum;

		bool m_bProjectionMatrixDirty;
		bool m_bViewMatrixDirty;
		bool m_bFrustumDirty;

		Vector2 m_v2DOffset;
		bool m_bSet;

		//! update the projection matrix
		const Matrix44& UpdateProjectionMatrix();

		//! update the view matrix
		const Matrix44& UpdateViewMatrix(bool bReturnInverse = false);

		//! updates the view frustum
		const Frustum& UpdateFrustum();

		// properties
		f32 m_FOV;
		f32 m_ZNear;
		f32 m_ZFar;
		E_Type m_eType;
		Vector3 m_vUpVector;
		Vector3 m_vLookAt;
		Size m_TargetResolution;
	};
}

#endif // _CAMERA_3D_H_INCLUDED_

