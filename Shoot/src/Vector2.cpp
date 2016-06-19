/* 

Amine Rehioui
Created: March 20th 2010

*/

#include "Shoot.h"

#include "Vector2.h"

namespace shoot
{
	//! returns the length
	f32 Vector2::GetLength() const
	{
		f32 length = Math::SquareRoot((X*X) + (Y*Y));
		return length;
	}

	//! returns the length squared
	f32 Vector2::GetLengthSquared() const
	{
		f32 lengthSquared = (X*X) + (Y*Y);
		return lengthSquared;
	}

	//! returns the dot product
	f32 Vector2::DotProduct(const Vector2& other) const
	{
		f32 dotProduct = (X*other.X) + (Y*other.Y);
		return dotProduct;
	}

	//! returns a rotated vector
	Vector2 Vector2::Rotate(f32 fAngleRadians, const Vector2& vOrigin /*= Vector2(0.0f, 0.0f)*/)
	{
		// TODO: handle non zero origin
		Vector2 result;
		result.X = X*Math::Cos(fAngleRadians) - Y*Math::Sin(fAngleRadians);
		result.Y = X*Math::Sin(fAngleRadians) + Y*Math::Cos(fAngleRadians);
		return result;
	}

	//! normalizes the vector
	Vector2& Vector2::Normalize()
	{
		const Vector2& constPtr = *this;
		*this = constPtr.Normalize();
		return *this;
	}

	//! normalizes the vector
	Vector2 Vector2::Normalize() const
	{
		Vector2 result = *this;
		f32 length = GetLength();
		if(length > Math::Epsilon)
		{
			result.X /= length;
			result.Y /= length;
		}
		return result;
	}

	// operators
	bool Vector2::operator == (const Vector2& other) const
	{
		return Math::FEqual(X, other.X) && Math::FEqual(Y, other.Y);
	}
}

