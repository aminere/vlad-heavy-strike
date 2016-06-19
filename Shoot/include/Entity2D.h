/* 

Amine Rehioui
Created: February 22th 2010

*/

#ifndef _ENTITY_2D_H_INCLUDED_
#define _ENTITY_2D_H_INCLUDED_

#include "RenderableEntity.h"

namespace shoot
{
	//! Base 2D entity class
	class Entity2D : public RenderableEntity
	{
	public:

		//! Macro to provide type information
		DECLARE_OBJECT(Entity2D, RenderableEntity);

		//! constructor
		Entity2D();

		//! serializes the entity to/from a PropertyStream
		virtual void Serialize(PropertyStream& stream);

		//! Sets the position relative to the parent's position
		inline void SetPosition(const Vector2& vPos) 
		{ 
			m_vPosition = vPos;
			m_bLocalTransformationMatrixDirty = true;
		}

		//! Sets the absolute position
		void SetAbsolutePosition(const Vector2& vAbsPos);

		//! returns the absolute scale
		Vector2 GetAbsoluteScale() const;

		//! Get the position relative to the parent's position
		inline const Vector2& GetPosition() const { return m_vPosition; }

		//! Translates the entity
		virtual void Translate(const Vector2& amount);

		//! returns the rotation
		inline const f32& GetRotation() const { return m_fRotation; }

		//! returns the scaling vector
		inline const Vector2& GetScale() const { return m_vScale; }

		//! sets the scaling vector
		inline void SetScale(const Vector2& vScale) 
		{ 
			m_vScale = vScale;
			m_bLocalTransformationMatrixDirty = true;
		}

		//! sets the scale uniformally
		inline void SetScale(f32 fScale) 
		{ 
			m_vScale.Set(fScale, fScale);
			m_bLocalTransformationMatrixDirty = true;
		}

		//! sets the rotation
		inline void SetRotation(f32 fRotation) 
		{ 
			m_fRotation = fRotation;
			m_bLocalTransformationMatrixDirty = true;
		}

		//! rotates the entity
		inline void Rotate(f32 fRotation) 
		{ 
			m_fRotation += fRotation;
			m_bLocalTransformationMatrixDirty = true;
		}

		//! returns the center of local transformation 
		inline const Vector2& GetCenter() const { return m_vCenter; }

		//! sets the center of local transformation
		inline void SetCenter(const Vector2& vCenter) 
		{ 
			m_vCenter = vCenter; 
			m_bLocalTransformationMatrixDirty = true;
		}

		//! returns the affine transformation matrix
		/** TODO: avoid recalculating when nothing changed */
		Matrix44 GetTransformationMatrix();

		//! Sets the local axis-aligned bounding box
		inline void SetBoundingBox(const AABBox2D& bbox) { m_BoundingBox = bbox; }

		//! Get the local axis-aligned bounding box
		virtual AABBox2D GetBoundingBox() const { return m_BoundingBox; }

		//! returns the horizontal flipping status
		inline bool IsHFlipped() const { return m_bHFlip; }

		//! set horizontal flipping
		inline void SetHFlip(bool bSet) { m_bHFlip = bSet; }

		//! returns the horizontal flipping status
		inline bool IsVFlipped() const { return m_bVFlip; }

		//! set horizontal flipping
		inline void SetVFlip(bool bSet) { m_bVFlip = bSet; }

#ifdef SHOOT_EDITOR
		//! returns the affine transformation matrix for the center of the entity
		/** TODO: avoid recalculating when nothing changed */
		Matrix44 GetCenterTransformationMatrix();
#endif // SHOOT_EDITOR

	protected:

		//! update the local affine transformation matrix
		void UpdateLocalTransformationMatrix();

		Matrix44 m_LocalTransformationMatrix;
		bool m_bLocalTransformationMatrixDirty;

		// attributes
		Vector2 m_vPosition;
		f32 m_fRotation;
		Vector2 m_vScale;
		Vector2 m_vCenter;
		AABBox2D m_BoundingBox;
		bool m_bHFlip;
		bool m_bVFlip;
	};
}

#endif // _ENTITY_2D_H_INCLUDED_

