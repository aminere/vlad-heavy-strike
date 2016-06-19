/* 

Amine Rehioui
Created: December 18th 2010

*/

#ifndef _PLANE_H_INCLUDED_
#define _PLANE_H_INCLUDED_

namespace shoot
{
	//! Plane class
	class Plane
	{
	public:

		//! classification of a point with respect to the plane
		enum E_Classification
		{
			C_Front,
			C_Back,
			C_Planar,
			C_Unknown
		};

		//! constructor
		Plane()
			: Normal(Vector3::Zero)
			, D(0.0f)
		{
		}

		//! constructor
		Plane(const Vector3& vNormal, f32 fDistFromOrigin)
			: Normal(vNormal)
			, D(fDistFromOrigin)
		{
		}
			
		//! constructor
		Plane(const Vector3& v1, const Vector3& v2, const Vector3& v3);

		//! returns the intersection of ray with the plane		
		bool IntersectWithRay(const Vector3& vRayStart, const Vector3& vRayDir, Vector3* pIntersection = 0, E_Classification* pClass = 0) const;

		//! classifies a point with respect to the plane
		E_Classification ClassifyPoint(const Vector3& vPoint, f32* fNominator = 0) const;

		Vector3 Normal;
		f32 D; // distance from origin
	};
}

#endif // _PLANE_H_INCLUDED_

