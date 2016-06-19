/* 

Amine Rehioui
Created: October 15th 2011

*/

#ifndef _COLLISION_UTILS_H_INCLUDED_
#define _COLLISION_UTILS_H_INCLUDED_

namespace shoot
{
	//! Collision utils
	class CollisionUtils
	{
	public:

		//! AABBox3D vs 3D ray intersection check
		static bool AABBox3DIntersectsWithRay(const AABBox3D& box, const Vector3& rayStart, const Vector3& rayEnd, Vector3* pIntersection = 0);

		//! AABBox3D vs AABBox3D intersection check
		static bool AABBox3DIntersectsWithAABBox3D(const AABBox3D& box1, const AABBox3D& box2);

		//! AABBox2D vs AABBox2D intersection check
		static bool AABBox2DIntersectsWithAABBox2D(const AABBox2D& box1, const AABBox2D& box2);

		//! AABBox3D vs 3D sphere intersection check
		static bool AABBox3DIntersectsWithSphere(const AABBox3D& box, const Vector3& sphereCenter, f32 sphereRadius);

		//! 3D sphere vs 3D sphere intersection check
		static bool SphereIntersectsWithSphere(const Vector3& sphere1Center, f32 sphere1Radius, const Vector3& sphere2Center, f32 sphere2Radius);

		//! 3D sphere vs ray intersection check
		static bool SphereIntersectsWithRay(const Vector3& vCenter, f32 fRadius, const Vector3& vRayStart, const Vector3& vRayDir, f32* fRayLength = 0, f32* fDistToIntersection = 0);

	private:

		//! Fast Ray intersection check (no intersection point calculated)
		static bool AABBox3DFastRayIntersectionCheck(const AABBox3D& box, const Vector3& rayStart, const Vector3& rayEnd);

#ifdef SHOOT_EDITOR
		//! Slower Ray intersection check, can return intersection point
		static bool AABBox3DSlowRayIntersectionCheck(const AABBox3D& box, const Vector3& rayStart, const Vector3& rayEnd, Vector3* pIntersection);
#endif // SHOOT_EDITOR
	};
}

#endif // _COLLISION_UTILS_H_INCLUDED_


