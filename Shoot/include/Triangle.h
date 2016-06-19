/* 

Amine Rehioui
Created: December 18th 2010

*/

#ifndef _TRIANGLE_H_INCLUDED_
#define _TRIANGLE_H_INCLUDED_

namespace shoot
{
	class Triangle
	{
	public:

		//! constructor
		Triangle(const Vector3& a, const Vector3& b, const Vector3& c)
			: A(a)
			, B(b)
			, C(c)
		{
		}

		//! returns true if a co-planar point is inside the triangle
		bool IsPointInside(const Vector3& vPoint, f32 fMargin = 0.0f) const;

		//! returns the closest point on the triangle
		Vector3 GetClosestPoint(const Vector3& vPoint) const;

		Vector3 A;
		Vector3 B;
		Vector3 C;
	};
}

#endif // _TRIANGLE_H_INCLUDED_

