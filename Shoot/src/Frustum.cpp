/* 

Amine Rehioui
Created: April 1st 2013

*/

#include "Shoot.h"

#include "Frustum.h"

namespace shoot
{
	//! constructor
	Frustum::Frustum(f32 fFov, f32 fRatio, f32 fNear, f32 fFar, const Vector3& vPosition, const Vector3& vLookAt, const Vector3& vUp)
	{
		f32 Hnear = Math::Tan(fFov * Math::DegToRadFactor / 2) * fNear;
		f32 Wnear = Hnear * fRatio;
		f32 Hfar = Math::Tan(fFov * Math::DegToRadFactor / 2) * fFar;
		f32 Wfar = Hfar * fRatio;
		Vector3 vDirection = (vLookAt-vPosition).Normalize();
		Vector3 vRight = vDirection.CrossProduct(vUp).Normalize();
		Vector3 vLocalUp = vRight.CrossProduct(vDirection).Normalize();

		Vector3 FCenter = vPosition + vDirection * fFar;
		Vector3 FTopLeft = FCenter + (vLocalUp * Hfar) - (vRight * Wfar);
		Vector3 FTopRight = FCenter + (vLocalUp * Hfar) + (vRight * Wfar);
		Vector3 FBottomLeft = FCenter - (vLocalUp * Hfar) - (vRight * Wfar);
		Vector3 FBottomRight = FCenter - (vLocalUp * Hfar) + (vRight * Wfar);
		Vector3 NCenter = vPosition + vDirection * fNear;
		Vector3 NTopLeft = NCenter + (vLocalUp * Hnear) - (vRight * Wnear);
		Vector3 NTopRight = NCenter + (vLocalUp * Hnear) + (vRight * Wnear);
		Vector3 NBottomLeft = NCenter- (vLocalUp * Hnear) - (vRight * Wnear);
		Vector3 NBottomRight = NCenter - (vLocalUp * Hnear) + (vRight * Wnear);

		m_Planes[P_Top] = Plane(NTopLeft, FTopLeft, FTopRight);
		m_Planes[P_Bottom] = Plane(NBottomRight, FBottomRight, FBottomLeft);
		m_Planes[P_Left] = Plane(FBottomLeft, FTopLeft, NTopLeft);
		m_Planes[P_Right] = Plane(NBottomRight, NTopRight, FTopRight);
		m_Planes[P_Near] = Plane(NBottomLeft, NTopLeft, NTopRight);
		m_Planes[P_Far] = Plane(FBottomRight, FTopRight, FTopLeft);

#ifdef SHOOT_EDITOR
		m_FTopLeft = FTopLeft;
		m_FTopRight = FTopRight;
		m_FBottomLeft = FBottomLeft;
		m_FBottomRight = FBottomRight;
		m_NTopLeft = NTopLeft;
		m_NTopRight = NTopRight;
		m_NBottomLeft = NBottomLeft;
		m_NBottomRight = NBottomRight;
#endif // SHOOT_EDITOR
	}

	//! test a box against the frustum
	Frustum::E_TestResult Frustum::Test(const AABBox3D& bbox) const
	{
		Vector3 corners[8] =
		{
			Vector3::Create(bbox.Min().X, bbox.Min().Y, bbox.Min().Z),
			Vector3::Create(bbox.Min().X, bbox.Min().Y, bbox.Max().Z),
			Vector3::Create(bbox.Min().X, bbox.Max().Y, bbox.Min().Z),
			Vector3::Create(bbox.Min().X, bbox.Max().Y, bbox.Max().Z),
			Vector3::Create(bbox.Max().X, bbox.Min().Y, bbox.Min().Z),
			Vector3::Create(bbox.Max().X, bbox.Min().Y, bbox.Max().Z),
			Vector3::Create(bbox.Max().X, bbox.Max().Y, bbox.Min().Z),
			Vector3::Create(bbox.Max().X, bbox.Max().Y, bbox.Max().Z)
		};

		u32 planesIn = 0;
		for(u32 i=0; i<P_Count; ++i)
		{
			u32 pointsIn = 8;
			u32 planeIn = 1;

			for(u32 j=0; j<8; ++j)
			{
				if(m_Planes[i].ClassifyPoint(corners[j]) == Plane::C_Front)
				{
					planeIn = 0;
					--pointsIn;
				}
			}
			
			if(pointsIn == 0)
			{
				return TR_Out;
			}

			planesIn += planeIn;
		}
		
		if(planesIn == P_Count)
		{
			return TR_In;
		}
		
		return TR_Intersect;
	}

	//! test a sphere against the frustum
	Frustum::E_TestResult Frustum::Test(const Vector3& vCenter, f32 fRadius) const
	{
		for(u32 i=0; i<P_Count; ++i)
		{
			f32 fDist = vCenter.DotProduct(m_Planes[i].Normal) - m_Planes[i].D;
			if(fDist > fRadius)
			{
				return TR_Out;
			}

			if(Math::FAbs(fDist) < fRadius)
			{
				return TR_Intersect;
			}
		}

		return TR_In;
	}

	//! checks if a point is inside the frustum
	bool Frustum::Contains(const Vector3& vPoint) const
	{
		for(u32 i=0; i<P_Count; ++i)
		{
			if(m_Planes[i].ClassifyPoint(vPoint) == Plane::C_Front)
			{
				return false;
			}
		}
		return true;
	}
}

