/* 

Amine Rehioui
Created: March 20th 2010

*/

#include "Shoot.h"

#include "Quaternion.h"

namespace shoot
{
	//! static variables initialization
	Vector3 Vector3::Zero = Vector3::Create(0.0f, 0.0f, 0.0f);
	Vector3 Vector3::One = Vector3::Create(1.0f, 1.0f, 1.0f);

	//! normalizes the vector
	Vector3 Vector3::Normalize() const
	{
		Vector3 result = *this;
		f32 length = GetLength();
		if(length > Math::Epsilon)
		{
			result.X /= length;
			result.Y /= length;
			result.Z /= length;
		}
		return result;
	}

	//! rotates the vector
	Vector3 Vector3::Rotate(const Vector3& vRotationRadians) const
	{		
		Matrix44 matrix;
		matrix.SetRotation(vRotationRadians);
		Vector3 result = *this;
		result = matrix.TransformVect(result);
		return result;
	}

	//! rotates the vector around an axis
	Vector3 Vector3::Rotate(const Vector3& vAxis, f32 fAngleRadians) const
	{
		return Quaternion(vAxis, fAngleRadians).Rotate(*this);
	}

	bool Vector3::operator == (const Vector3& other) const
	{
		return Math::FEqual(X, other.X) 
			&& Math::FEqual(Y, other.Y) 
			&& Math::FEqual(Z, other.Z);
	}
}

