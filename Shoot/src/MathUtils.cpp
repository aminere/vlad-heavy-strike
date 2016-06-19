/* 

Amine Rehioui
Created: March 20th 2010

*/

#include "Shoot.h"

#include <math.h> // cos

namespace shoot
{
	const f32 Math::PI = 3.141592f;

	const f32 Math::Epsilon = 0.00001f;

	const f32 Math::Maxf32 = 3.402823466E+38f;

	//! factor to convert from degrees to radians
	const f32 Math::DegToRadFactor = 0.017453288f; // PI / 180.0f;
	const f32 Math::RadToDegFactor = 57.29579143f; // 180.0f / PI;

	//! returns true if a point is on a the line segment [A B]
	bool Math::IsPointOnLineSegment(const Vector3& vPoint, const Vector3& A, const Vector3& B)
	{		
		Vector3 delta = B - A;		
		bool bTIsSet = false;
		f32 t = 0.0f;
		
		// check X
		if(!FIsZero(delta.X))
		{
			t = (vPoint.X - A.X) / delta.X;
			bTIsSet = true;
		}
		else if(!FEqual(vPoint.X, A.X))
		{
			return false;
		}		

		// check Y
		if(!FIsZero(delta.Y))
		{
			f32 newT = (vPoint.Y - A.Y) / delta.Y;
			t = bTIsSet ? t : newT;
			bTIsSet = true;
			if(!FEqual(t, newT))
			{
				return false;
			}
		}
		else if(!FEqual(vPoint.Y, A.Y))
		{
			return false;
		}	

		// check Z
		if(!FIsZero(delta.Z))
		{
			f32 newT = (vPoint.Z - A.Z) / delta.Z;
			t = bTIsSet ? t : newT;
			bTIsSet = true;
			if(!FEqual(t, newT))
			{
				return false;
			}
		}
		else if(!FEqual(vPoint.Z, A.Z))
		{
			return false;
		}

		return bTIsSet ? (t >= 0.0f && t <= 1.0f) : true;		
	}

	//! returns the closest point on a line segment [A B]
	Vector3 Math::GetClosestPointOnLine(const Vector3& vPoint, const Vector3& vA, const Vector3& vB)
	{		
		Vector3 vC = vPoint-vA;
		Vector3 vD = vB-vA; 
		f32 fLength = vD.GetLength();

		vD = vD / fLength;
		f32 t = vD.DotProduct(vC);

		if(t < 0.0f) 
		{
			return vA;
		}
		else if(t > fLength) 
		{
			return vB;
		}

		return vA + vD*t;
	}

	//! return the cosine of an angle
	/** \param fAngle: angle in radians */
	f32 Math::Cos(f32 fAngle)
	{
		return cos(fAngle);
	}

	//! return the sine of an angle
	/** \param fAngle: angle in radians */
	f32 Math::Sin(f32 fAngle)
	{
		return sin(fAngle);
	}

	//! returns the tangent of an angle
	f32 Math::Tan(f32 fAngle)
	{
		return tan(fAngle);
	}

	//! return the arc cosine
	f32 Math::ACos(f32 f)
	{
		return acos(f);
	}

	//! return the arc sine
	f32 Math::ASin(f32 f)
	{
		return asin(f);
	}

	//! return the arc tangent
	f32 Math::ATan(f32 f)
	{
		return atan(f);
	}

	//! return the arc tangent
	f32 Math::ATan2(f32 y, f32 x)
	{
		return atan2(y, x);
	}

	//! returns the floor of a number
	f32 Math::Floor(f32 f)
	{
		return floorf(f);
	}

	//! returns the square root of a number
	f32 Math::SquareRoot(f32 f)
	{
		return sqrtf(f);
	}

	//! converts from spherical coords to cartesian coords
	/** \param fRadius: sphere radius
		\param fAzimuth: azimuth angle [0 - 2Pi] 
		\parem fInclination: inclination angle [0 - Pi] */
	Vector3 Math::SphericalToCartesian(f32 fRadius, f32 fAzimuth, f32 fInclination)
	{
		Vector3 v;
		v.X = fRadius*Cos(fAzimuth)*Sin(fInclination);
		v.Y = fRadius*Sin(fAzimuth)*Sin(fInclination);
		v.Z = fRadius*Cos(fInclination);
		return v;
	}

	//! returns the next power of 2
	u32 Math::GetNextPow2(u32 i)
	{
		i--;
		i |= i >> 1;
		i |= i >> 2;
		i |= i >> 4;
		i |= i >> 8;
		i |= i >> 16;
		i++;
		return i;
	}
}

