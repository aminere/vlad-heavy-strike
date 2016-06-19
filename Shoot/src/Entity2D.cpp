/* 

Amine Rehioui
Created: February 22th 2010

*/

#include "Shoot.h"

namespace shoot
{
	DEFINE_OBJECT(Entity2D);

	//! constructor
	Entity2D::Entity2D()
		// properties
		: m_fRotation(0.0f)
		, m_vScale(1.0f, 1.0f)
		, m_bHFlip(false)
		, m_bVFlip(false)
		, m_bLocalTransformationMatrixDirty(true)
	{
	}

	//! serializes the entity to/from a PropertyStream
	void Entity2D::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);

		stream.Serialize(PT_Vec2, "Position", &m_vPosition);
		stream.Serialize(PT_Float, "Rotation", &m_fRotation);
		stream.Serialize(PT_Vec2, "Scale", &m_vScale);
		stream.Serialize(PT_Vec2, "Center", &m_vCenter);
		stream.Serialize(PT_AABBox2D, "BBox", &m_BoundingBox);	

		stream.Serialize(PT_Bool, "HFlip", &m_bHFlip);
		stream.Serialize(PT_Bool, "VFlip", &m_bVFlip);

		m_bLocalTransformationMatrixDirty = true;
	}

	//! Sets the absolute position
	void Entity2D::SetAbsolutePosition(const Vector2& vAbsPos)
	{
		Vector3 vPos = Vector3::Create(vAbsPos.X, vAbsPos.Y, 0.0f);
		if(Entity2D* pParent = GetAncestor<Entity2D>())
		{
			Matrix44 inverse;
			if(pParent->GetTransformationMatrix().GetInverse(inverse))
			{
				vPos = inverse.TransformVect(vPos);
			}
		}
		SetPosition(Vector2(vPos.X, vPos.Y) + (m_vCenter*m_vScale));
	}

	//! returns the absolute scale
	Vector2 Entity2D::GetAbsoluteScale() const
	{
		Vector2 vScale = m_vScale;
		if(Entity2D* p2DEntity = GetAncestor<Entity2D>())
		{
			vScale = vScale * p2DEntity->GetAbsoluteScale();
		}
		return vScale;
	}

	//! Translates the entity
	void Entity2D::Translate(const Vector2& amount)
	{
		m_vPosition += amount;
		m_bLocalTransformationMatrixDirty = true;
	}

	//! returns the affine transformation matrix
	/** TODO: avoid recalculating when nothing changed */
	Matrix44 Entity2D::GetTransformationMatrix()
	{		
		if(m_bLocalTransformationMatrixDirty)
		{
			UpdateLocalTransformationMatrix();
		}

		Matrix44 matrix = m_LocalTransformationMatrix;		
		if(Entity2D* p2DEntity = GetAncestor<Entity2D>())
		{
			matrix *= p2DEntity->GetTransformationMatrix();
		}
		return matrix;
	}	

	//! update the local affine transformation matrix
	void Entity2D::UpdateLocalTransformationMatrix()
	{
		m_LocalTransformationMatrix = Matrix44::Identity;
		m_LocalTransformationMatrix.Translate(-Vector3::Create(m_vCenter.X, m_vCenter.Y, 0.0f));
		m_LocalTransformationMatrix.Scale(Vector3::Create(m_vScale.X, m_vScale.Y, 1.0f));
		m_LocalTransformationMatrix.Rotate(Vector3::Create(0.0f, 0.0f, m_fRotation*Math::DegToRadFactor));		
		m_LocalTransformationMatrix.Translate(Vector3::Create(m_vPosition.X, m_vPosition.Y, 0.0f));
		m_bLocalTransformationMatrixDirty = false;
	}

#ifdef SHOOT_EDITOR
	//! returns the affine transformation matrix for the center of the entity
	/** TODO: avoid recalculating when nothing changed */
	Matrix44 Entity2D::GetCenterTransformationMatrix()
	{
		Matrix44 matrix = Matrix44::Identity;
		matrix.Scale(Vector3::Create(m_vScale.X, m_vScale.Y, 1.0f));
		matrix.Rotate(Vector3::Create(0.0f, 0.0f, m_fRotation*Math::DegToRadFactor));		
		matrix.Translate(Vector3::Create(m_vPosition.X, m_vPosition.Y, 0.0f));

		// TODO: use type checking at initialization time in AddChild() and only use static_cast here		
		if(Entity2D* pEntity2D = GetAncestor<Entity2D>())
		{
			matrix *= pEntity2D->GetTransformationMatrix();
		}
		
		return matrix;
	}
#endif // SHOOT_EDITOR
}

