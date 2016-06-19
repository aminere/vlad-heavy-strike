/* 

Amine Rehioui
Created: March 20th 2010

*/

#ifndef _MATH_UTILS_H_INCLUDED_
#define _MATH_UTILS_H_INCLUDED_

namespace shoot
{
	// forwards
	class Vector3;

	//! Math utilities
	class Math
	{
	public:
			
		static const f32 PI;

		static const f32 Epsilon;

		static const f32 Maxf32;

		//! returns true if a point is on a the line segment [A B]
		static bool IsPointOnLineSegment(const Vector3& vPoint, const Vector3& A, const Vector3& B);

		//! returns the closest point on a line segment [A B]
		static Vector3 GetClosestPointOnLine(const Vector3& vPoint, const Vector3& vA, const Vector3& vB);

		//! clamp a value between min and max
		template <class T>
		static T Clamp(T value, T min, T max)
		{
			if(value < min) return min;
			else if(value > max) return max;
			else return value;
		}

		//! swaps two values
		template <class T>
		static void Swap(T& value1, T& value2)
		{
			T temp = value1;
			value1 = value2;
			value2 = temp;
		}

		//! returns the absolute value
		inline static f32 FAbs(f32 fValue)
		{
			return ((fValue > 0.0f) ? fValue : -fValue);
		}

		//! compare two floating point values
		static bool FEqual(f32 fValue1, f32 fValue2, f32 fEpsilon = Epsilon)
		{
			return FAbs(fValue1 - fValue2) < Epsilon;
		}

		//! compare a floating point with zero
		static bool FIsZero(f32 fValue)
		{
			return FAbs(fValue) < Epsilon;
		}

		//! returns the sign
		static inline f32 FSign(f32 f)
		{
			return (f < 0.0f) ? -1.0f : 1.0f;
		}

		//! returns the minimum of two values
		template <class T>
		static T Min(T a, T b) 
		{
			return (a < b) ? a : b;
		}

		//! returns the maximum of two values
		template <class T>
		static T Max(T a, T b) 
		{
			return (a > b) ? a : b;
		}

		//! returns the maximum of three values
		template <class T>
		static T Max(T a, T b, T c) 
		{
			return (a > b) ? (a > c ? a : c) : (b > c ? b : c);
		}

		//! return the cosine of an angle
		/** \param fAngle: angle in radians */
		static f32 Cos(f32 fAngle);

		//! return the sine of an angle
		/** \param fAngle: angle in radians */
		static f32 Sin(f32 fAngle);

		//! returns the tangent of an angle
		static f32 Tan(f32 fAngle);

		//! return the arc cosine
		static f32 ACos(f32 f);

		//! return the arc sine
		static f32 ASin(f32 f);

		//! return the arc tangent
		static f32 ATan(f32 f);

		//! return the arc tangent
		static f32 ATan2(f32 y, f32 x);

		//! linear interpolation
		template <class T, class U>
		static T Lerp(const T a, const T b, U k)
		{
			return (b - a)*k + a;
		}

		//! cubic interpolation
		template <class T, class U>
		static T CubicInterpolate(T p1, T p2, T p0, T p3, U k)
		{
			U k2 = k*k;
			T a0, a1, a2, a3;			
			a0 = p3 - p2 - p0 + p1;
			a1 = p0 - p1 - a0;
			a2 = p2 - p0;
			a3 = p1;
			return (a0*k*k2 + a1*k2 + a2*k + a3);
		}

		//! catmull-rom interpolation
		template <class T, class U>
		static T CatmullRomInterpolate(T p1, T p2, T p0, T p3, U k)
		{
			U k2 = k*k;
			T a0, a1, a2, a3;			
			a0 = p0*-0.5f + p1*1.5f - p2*1.5f + p3*0.5f;
			a1 = p0 - p1*2.5f + p2*2.0f - p3*0.5f;
			a2 = p0*-0.5f + p2*0.5f;
			a3 = p1;
			return (a0*k*k2 + a1*k2 + a2*k + a3);
		}

		//! returns the floor of a number
		static f32 Floor(f32 f);

		//! returns the fractional part of a number
		static f32 Fraction(f32 f)
		{
			return f - Floor(f);
		}

		//! returns the square root of a number
		static f32 SquareRoot(f32 f);

		//! performs damping between two values
		template <class T, class U>
		static T Damp(const T src, const T dest, U deltaTime, U duration)
		{
			return Lerp<T, U>(src, dest, (deltaTime/duration));
		}

		//! converts from spherical coords to cartesian coords
		/** \param fRadius: sphere radius
		    \param fAzimuth: azimuth angle [0 - 2Pi] 
			\parem fInclination: inclination angle [0 - Pi] */
		static Vector3 SphericalToCartesian(f32 fRadius, f32 fAzimuth, f32 fInclination);

		//! returns the next power of 2
		static u32 GetNextPow2(u32 i);

		//! factor to convert from degrees to radians
		static const f32 DegToRadFactor;
		static const f32 RadToDegFactor;
	};
}

#endif // _MATH_UTILS_H_INCLUDED_

