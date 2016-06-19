/* 

Amine Rehioui
Created: September 4th 2012

*/

#ifndef _VECTOR_4_H_INCLUDED_
#define _VECTOR_4_H_INCLUDED_

namespace shoot
{
	//! 4D vector class
	class Vector4
	{
	public:

		inline static Vector4 Create(f32 _X, f32 _Y, f32 _Z, f32 _W)
		{
			Vector4 vec4 = { _X, _Y, _Z, _W };
			return vec4;
		}

		//! Sets the coordinates
		inline void Set(f32 _X, f32 _Y, f32 _Z, f32 _W) 
		{
			X = _X;
			Y = _Y;
			Z = _Z;
			W = _W;
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

		//! returns the dot product
		inline f32 DotProduct(const Vector4& other) const
		{
			return (X*other.X) + (Y*other.Y) + (Z*other.Z) + (W*other.W);
		}

		//! returns the cross product
		inline Vector4 CrossProduct(const Vector4& other) const
		{
			return Vector4::Create(Y*other.Z - Z*other.Y, Z*other.X - X*other.Z, X*other.Y - Y*other.X, 0.0f);
		}

		//! normalizes the vector
		inline Vector4& Normalize()
		{
			return (*this = static_cast<const Vector4*>(this)->Normalize());
		}

		//! normalizes the vector
		Vector4 Normalize() const;

		// operators
		inline Vector4 operator + (const Vector4& other) const
		{			
			return Vector4::Create(X+other.X, Y+other.Y, Z+other.Z, W+other.W);
		}

		inline Vector4 operator - (const Vector4& other) const
		{			
			return Vector4::Create(X-other.X, Y-other.Y, Z-other.Z, W-other.W);
		}

		inline Vector4 operator * (const f32 fValue) const
		{			
			return Vector4::Create(X*fValue, Y*fValue, Z*fValue, W*fValue);
		}

		inline Vector4 operator / (const f32 fValue) const
		{			
			return Vector4::Create(X/fValue, Y/fValue, Z/fValue, W/fValue);
		}

		inline Vector4& operator += (const Vector4& other)
		{
			X += other.X; Y += other.Y;	Z += other.Z; W += other.W;
			return *this;
		}

		inline Vector4& operator -= (const Vector4& other)
		{
			X -= other.X; Y -= other.Y; Z -= other.Z; W -= other.W;
			return *this;
		}

		inline Vector4& operator *= (const f32 fValue)
		{
			X *= fValue; Y *= fValue; Z *= fValue; W *= fValue; return *this;
		}

		inline Vector4 operator -() const
		{			
			return Vector4::Create(-X, -Y, -Z, -W);
		}

		inline f32& operator[] (u32 index)
		{
			SHOOT_ASSERT(index < 4, "Invalid index");
			return *(&v[index]);
		}

		inline const f32& operator[] (u32 index) const
		{
			SHOOT_ASSERT(index < 4, "Invalid index");
			return *(&v[index]);
		}

		bool operator == (const Vector4& other) const;

		inline bool operator != (const Vector4& other) const
		{
			return !this->operator==(other);
		}

		union
		{
			struct
			{
				f32 X, Y, Z, W;
			};

			f32 v[4];
		};

		static Vector4 Zero;
		static Vector4 One;
	};
}

#endif // _VECTOR_3D_H_INCLUDED_

