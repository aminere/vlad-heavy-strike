/* 

Amine Rehioui
Created: April 10th 2012

*/

#include "Shoot.h"

#include "Quaternion.h"

namespace shoot
{
	//! constructor
	Quaternion::Quaternion(const Vector3& vUnitAxis, f32 fAngle)
	{ 
		f32 halfAngle = fAngle/2.0f;       
		W = Math::Cos(halfAngle);
		V = vUnitAxis*Math::Sin(halfAngle);
	}	

	//! constructor
	Quaternion::Quaternion(const Vector3& vRotation)
	{
		Set(vRotation);
	}

	//! normalizes the Quaternion
	Quaternion Quaternion::Normalize() const
	{
		Quaternion result = *this;
		f32 lengthSquared = GetLengthSquared();
		if(Math::FAbs(lengthSquared) > Math::Epsilon && Math::FAbs(lengthSquared - 1.0f) > Math::Epsilon)
		{
			f32 invLength = 1.0f/Math::SquareRoot(lengthSquared);			
			result.X *= invLength;
			result.Y *= invLength;
			result.Z *= invLength;
			result.W *= invLength;
		}
		return result;
	}

	//! rotates a vector
	Vector3 Quaternion::Rotate(const Vector3& v) const
    {
		// Irrlicht / nVidia SDK implementation
		Vector3 qvec = V;
		Vector3 uv = qvec.CrossProduct(v);
		Vector3 uuv = qvec.CrossProduct(uv);
		uv *= (2.0f * W);
		uuv *= 2.0f;
		return v + uv + uuv;
    }

	//! returns a matrix representation
	Matrix44 Quaternion::GetMatrix()
	{
		f32 xx = X * X;
        f32 yy = Y * Y;
        f32 zz = Z * Z;
        f32 xy = X * Y;
        f32 xz = X * Z;
        f32 yz = Y * Z;
        f32 wx = W * X;
        f32 wy = W * Y;
        f32 wz = W * Z;   
        Matrix44 m;
        m[0] = 1.0f - (yy + zz)*2.0f;
        m[1] = (xy + wz)*2.0f;
        m[2] = (xz - wy)*2.0f;
        m[3] = 0.0f;
        m[4] = (xy - wz)*2.0f;
        m[5] = 1.0f - (xx + zz)*2.0f;
        m[6] = (yz + wx)*2.0f;
        m[7] = 0.0f;
        m[8] = (xz + wy)*2.0f;
        m[9] = (yz - wx)*2.0f;
        m[10] = 1.0f - (xx + yy)*2.0f;
        m[11] = 0.0f;
        m[12] = 0.0f;
        m[13] = 0.0f;
        m[14] = 0.0f;
        m[15] = 1.0f;
        return m;
	}

	//! sets from euler angles
	Quaternion& Quaternion::Set(const Vector3& vRotation)
	{
		/*f32 angle = vRotation.X * 0.5f;
		const f32 sr = Math::Sin(angle);
		const f32 cr = Math::Cos(angle);

		angle = vRotation.Y * 0.5f;
		const f32 sp = Math::Sin(angle);
		const f32 cp = Math::Cos(angle);

		angle = vRotation.Z * 0.5f;
		const f32 sy = Math::Sin(angle);
		const f32 cy = Math::Cos(angle);

		const f32 cpcy = cp * cy;
		const f32 spcy = sp * cy;
		const f32 cpsy = cp * sy;
		const f32 spsy = sp * sy;

		X = (f32)(sr * cpcy - cr * spsy);
		Y = (f32)(cr * spcy + sr * cpsy);
		Z = (f32)(cr * cpsy - sr * spcy);
		W = (f32)(cr * cpcy + sr * spsy);

		Normalize();*/

		const f32 p = vRotation.X * 0.5f;
		const f32 y = vRotation.Y * 0.5f;
		const f32 r = vRotation.Z * 0.5f;
		const f32 sinp = Math::Sin(p);
		const f32 siny = Math::Sin(y);
		const f32 sinr = Math::Sin(r);
		const f32 cosp = Math::Cos(p);
		const f32 cosy = Math::Cos(y);
		const f32 cosr = Math::Cos(r);

		X = sinr * cosp * cosy - cosr * sinp * siny;
		Y = cosr * sinp * cosy + sinr * cosp * siny;
		Z = cosr * cosp * siny - sinr * sinp * cosy;
		W = cosr * cosp * cosy + sinr * sinp * siny;

		Normalize();

		return *this;
	}

	// operators
	Quaternion Quaternion::operator * (const Quaternion& other) const
	{
		f32 w = W*other.W - V.DotProduct(other.V);
        Vector3 v = other.V*W + V*other.W + V.CrossProduct(other.V);
        return Quaternion(v.X, v.Y, v.X, w);
	}	
}

