/* 

Amine Rehioui
Created: December 18th 2010

*/

#include "Shoot.h"

#include "Triangle.h"

namespace shoot
{
	//! returns true if a co-planar point is inside the triangle
	bool Triangle::IsPointInside(const Vector3& vPoint, f32 fMargin /*= 0.0f*/) const
	{		
		// using barycentric method - this is supposedly the fastest method there is for this.
		// from http://www.blackpawn.com/texts/pointinpoly/default.html
		// Compute vectors
		Vector3 v0 = C - A;
		Vector3 v1 = B - A;
		Vector3 v2 = vPoint - A;

		// Compute dot products
		f32 dot00 = v0.DotProduct(v0);
		f32 dot01 = v0.DotProduct(v1);
		f32 dot02 = v0.DotProduct(v2);
		f32 dot11 = v1.DotProduct(v1);
		f32 dot12 = v1.DotProduct(v2);

		// Compute barycentric coordinates
		f32 invDenom = 1.0f / ((dot00 * dot11) - (dot01 * dot01));
		f32 u = ((dot11 * dot02) - (dot01 * dot12)) * invDenom;
		f32 v = ((dot00 * dot12) - (dot01 * dot02)) * invDenom;

		// Check if point is in triangle
		return (u >= 0.0f-fMargin) && (v >= 0.0f-fMargin) && (u + v <= 1.0f+fMargin);
	}

	//! returns the closest point on the triangle
	Vector3 Triangle::GetClosestPoint(const Vector3& vPoint) const
	{
		Vector3 vAB = Math::GetClosestPointOnLine(vPoint, A, B);
		Vector3 vBC = Math::GetClosestPointOnLine(vPoint, B, C);
		Vector3 vCA = Math::GetClosestPointOnLine(vPoint, C, A);

		f32 fAB = (vPoint-vAB).GetLength();
		f32 fBC = (vPoint-vBC).GetLength();
		f32 fCA = (vPoint-vCA).GetLength();

		f32 fMin = fAB;
		Vector3 vResult = vAB;

		if(fBC < fMin)
		{
			fMin = fBC;
			vResult = vBC;
		}

		if(fCA < fMin)
		{
			vResult = vCA;
		}

		return vResult;	
	}
}

