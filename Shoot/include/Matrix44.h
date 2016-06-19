/* 

Amine Rehioui
Created: May 5th 2010

*/

#ifndef _MATRIX_44_H_INCLUDED_
#define _MATRIX_44_H_INCLUDED_

namespace shoot
{
	//! 4x4 affine transformation matrix
	class Matrix44
	{
	public:

		//! constructor
		Matrix44()
		{	
		}

		//! constructor
		Matrix44(const f32 pFloats[16]);

		//! sets the translation
		void SetTranslation(const Vector3& vTranslation);

		//! sets the rotation
		void SetRotation(const Vector3& vRotation);

		//! sets the scale
		void SetScale(const Vector3& vScale);

		//! returns the translation set in this matrix
		Vector3 GetTranslation() const;

		//! returns the scaling
		Vector3 GetScale() const;

		//! translates the matrix
		void Translate(const Vector3& vTranslation);

		//! rotates the matrix
		void Rotate(const Vector3& vRotation);

		//! scales the matrix
		void Scale(const Vector3& vScale);

		//! transforms a 3D vector by this matrix
		Vector3 TransformVect(const Vector3& vect) const;

		//! calculates the inverse matrix
		/** returns false if inverse could not be calculated */
		bool GetInverse(Matrix44& out) const;
		
		// calculates a transposed matrix
		Matrix44 GetTranspose() const;

		//! returns a pointer to the internal float data
		inline  const f32* GetFloats() const { return m_f; }

		//! builds a perspective projection matrix
		Matrix44& BuildPerspectiveProjection(f32 fFOVDegrees, f32 fRatio, f32 fZNear, f32 fZFar);

		//! builds a 2D orthographic projection matrix
		Matrix44& Build2DOrthographicProjection(f32 fWidth, f32 fHeight);
		
		//! builds an LH look at matrix
		Matrix44& BuildLookAtLH(const Vector3& vEye, const Vector3& vTarget, const Vector3& vUpVector);

		// operators
		Matrix44 operator * (const Matrix44& other) const;

		inline Matrix44& operator *= (const Matrix44& other)
		{			
			return (*this = (*this) * other);
		}

		inline f32& operator()(const u32 row, const u32 column)
		{
			return m_f[row * 4 + column];
		}

		inline const f32& operator()(const u32 row, const u32 column) const
		{
			return m_f[row * 4 + column];
		}

		inline f32& operator[](u32 index)
		{
			return m_f[index];
		}

		inline const f32& operator[](u32 index) const
		{					
			return m_f[index];
		}

		//! static variables
		static Matrix44 Identity;

	private:

		f32 m_f[16];
	};
}

#endif // _MATRIX_44_H_INCLUDED_

