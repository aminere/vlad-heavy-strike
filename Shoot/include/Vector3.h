/* 

Amine Rehioui
Created: February 28th 2010

*/

#ifndef _VECTOR_3_H_INCLUDED_
#define _VECTOR_3_H_INCLUDED_

namespace shoot
{
	//! 3D vector class
	class Vector3
	{
	public:

		inline static Vector3 Create(f32 _X, f32 _Y, f32 _Z)
		{
			Vector3 vec3 = { _X, _Y, _Z };
			return vec3;
		}

		//! Sets the coordinates
		inline void Set(f32 _X, f32 _Y, f32 _Z) 
		{
			X = _X;
			Y = _Y;
			Z = _Z;
		}

		//! returns the length
		inline f32 GetLength() const
		{
			return Math::SquareRoot((X*X) + (Y*Y) + (Z*Z));
		}

		//! returns the length squared
		inline f32 GetLengthSquared() const
		{
			return (X*X) + (Y*Y) + (Z*Z);
		}

		//! returns the dot product
		inline f32 DotProduct(const Vector3& other) const
		{
			return (X*other.X) + (Y*other.Y) + (Z*other.Z);
		}

		//! returns the cross product
		inline Vector3 CrossProduct(const Vector3& other) const
		{
			return Vector3::Create(Y*other.Z - Z*other.Y, Z*other.X - X*other.Z, X*other.Y - Y*other.X);
		}

		//! normalizes the vector
		inline const Vector3& Normalize()
		{
			return (*this = static_cast<const Vector3*>(this)->Normalize());
		}

		//! normalizes the vector
		Vector3 Normalize() const;

		//! rotates the vector
		inline const Vector3& Rotate(const Vector3& vRotationRadians)
		{
			return (*this = static_cast<const Vector3*>(this)->Rotate(vRotationRadians));
		}

		//! rotates the vector
		Vector3 Rotate(const Vector3& vRotationRadians) const;

		//! rotates the vector around an axis
		inline const Vector3& Rotate(const Vector3& vAxis, f32 fAngleRadians)
		{
			return (*this = static_cast<const Vector3*>(this)->Rotate(vAxis, fAngleRadians));
		}

		//! rotates the vector around an axis
		Vector3 Rotate(const Vector3& vAxis, f32 fAngleRadians) const;

		// operators
		inline Vector3 operator + (const Vector3& other) const
		{			
			return Vector3::Create(X+other.X, Y+other.Y, Z+other.Z);
		}

		inline Vector3 operator - (const Vector3& other) const
		{			
			return Vector3::Create(X-other.X, Y-other.Y, Z-other.Z);
		}

		inline Vector3 operator * (const Vector3& other) const
		{
			return Vector3::Create(X*other.X, Y*other.Y, Z*other.Z);
		}

		inline Vector3 operator * (const f32 fValue) const
		{			
			return Vector3::Create(X*fValue, Y*fValue, Z*fValue);
		}

		inline Vector3 operator / (const f32 fValue) const
		{			
			return Vector3::Create(X/fValue, Y/fValue, Z/fValue);
		}

		inline Vector3& operator += (const Vector3& other)
		{
			X += other.X; Y += other.Y;	Z += other.Z;
			return *this;
		}

		inline Vector3& operator -= (const Vector3& other)
		{
			X -= other.X; Y -= other.Y; Z -= other.Z;
			return *this;
		}

		inline Vector3& operator *= (const f32 fValue)
		{
			X *= fValue; Y *= fValue; Z *= fValue; return *this;
		}

		inline Vector3 operator -() const
		{			
			return Vector3::Create(-X, -Y, -Z);
		}

		inline f32& operator[] (u32 index)
		{
			SHOOT_ASSERT(index < 3, "Invalid index");
			return *(&v[index]);
		}

		inline const f32& operator[] (u32 index) const
		{
			SHOOT_ASSERT(index < 3, "Invalid index");
			return *(&v[index]);
		}

		bool operator == (const Vector3& other) const;

		inline bool operator != (const Vector3& other) const
		{
			return !this->operator==(other);
		}

		union
		{
			struct
			{
				f32 X, Y, Z;
			};

			f32 v[3];
		};

		static Vector3 Zero;
		static Vector3 One;
	};
}

#endif // _VECTOR_3_H_INCLUDED_

