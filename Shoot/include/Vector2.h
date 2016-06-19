/* 

Amine Rehioui
Created: February 28th 2010

*/

#ifndef _VECTOR_2_H_INCLUDED_
#define _VECTOR_2_H_INCLUDED_

#include "Size.h"

namespace shoot
{
	//! 2D vector class
	class Vector2
	{
	public:

		//! constructor
		Vector2() : X(0.0f), Y(0.0f)
		{
		}

		//! constructor
		Vector2(f32 _X, f32 _Y) : X(_X), Y(_Y)
		{
		}

		//! constructor
		Vector2(const Size& size) : X(f32(size.Width)), Y(f32(size.Height))
		{
		}
		
		//! Sets the coordinates
		inline void Set(f32 _X, f32 _Y) 
		{
			X = _X;
			Y = _Y;
		}

		//! returns the length
		f32 GetLength() const;

		//! returns the length squared
		f32 GetLengthSquared() const;

		//! returns the dot product
		f32 DotProduct(const Vector2& other) const;

		//! returns a rotated vector
		Vector2 Rotate(f32 fAngleRadians, const Vector2& vOrigin = Vector2(0.0f, 0.0f));

		//! normalizes the vector
		Vector2& Normalize();

		//! normalizes the vector
		Vector2 Normalize() const;

		// operators
		inline Vector2 operator + (const Vector2& other) const
		{
			return Vector2(X+other.X, Y+other.Y);
		}

		inline Vector2 operator - (const Vector2& other) const
		{			
			return Vector2(X-other.X, Y-other.Y);
		}

		inline Vector2 operator * (const Vector2& other) const
		{
			return Vector2(X*other.X, Y*other.Y);
		}

		inline Vector2 operator * (const f32 fValue) const
		{
			return Vector2(X*fValue, Y*fValue);
		}

		inline Vector2 operator / (const f32 fValue) const
		{
			return Vector2(X/fValue, Y/fValue);
		}

		inline Vector2& operator += (const Vector2& other)
		{
			X += other.X; Y += other.Y;
			return *this;
		}

		inline Vector2& operator -= (const Vector2& other)
		{
			X -= other.X; Y -= other.Y;
			return *this;
		}

		inline bool operator != (const Vector2& other) const
		{
			return !(*this == other);
		}

		inline Vector2& operator = (const Size& size)
		{
			X = f32(size.Width); Y = f32(size.Height);
			return *this;
		}		

		inline Vector2& operator *= (const f32 fValue)
		{
			X *= fValue; Y *= fValue; 
			return *this;
		}

		bool operator == (const Vector2& other) const;

		f32 X, Y;
	};
}

#endif // _VECTOR_2_H_INCLUDED_

