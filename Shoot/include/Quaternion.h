/* 

Amine Rehioui
Created: April 10th 2012

*/

#ifndef _QUATERNION_H_INCLUDED_
#define _QUATERNION_H_INCLUDED_

namespace shoot
{
	//! Quaternion
	class Quaternion
	{
	public:

		//! constructor
		Quaternion()
		{
		}

		//! constructor
		Quaternion(f32 x, f32 y, f32 z, f32 w)
			: X(x)
			, Y(y)
			, Z(z)
			, W(w)
		{
		}

		//! constructor
		Quaternion(const Vector3& vUnitAxis, f32 fAngle);

		//! constructor
		Quaternion(const Vector3& vRotation);

		//! normalizes the Quaternion
		Quaternion& Normalize()
		{
			return (*this = static_cast<const Quaternion*>(this)->Normalize());
		}

		//! normalizes the Quaternion
		Quaternion Normalize() const;

		//! inverses the quaternion
		inline Quaternion& Inverse()
		{
			X = -X; Y = -Y; Z = -Z;
			return *this;
		}

		//! inverses the quaternion
		inline Quaternion Inverse() const
		{
			return Quaternion(-X, -Y, -Z, W);
		}

		//! returns the length
		inline f32 GetLength() const
		{
			return Math::SquareRoot((X*X) + (Y*Y) + (Z*Z) + (W*W));
		}

		//! returns the length squared
		inline f32 GetLengthSquared() const
		{
			return (X*X) + (Y*Y) + (Z*Z) + (W*W);
		}

		//! rotates a vector
		Vector3 Rotate(const Vector3& v) const;

		//! returns a matrix representation
		Matrix44 GetMatrix();

		//! sets from euler angles
		Quaternion& Set(const Vector3& vRotation);

		// operators
		Quaternion operator * (const Quaternion& other) const;
		Vector3 operator * (const Vector3 &v) const;

		inline Quaternion& operator *= (const Quaternion& other)
		{
			return (*this = (*this) * other);
		}

		union
		{
			struct
			{
				f32 X, Y, Z;
			};

			Vector3 V;
		};

		f32 W;
	};
}

#endif // _QUATERNION_H_INCLUDED_

