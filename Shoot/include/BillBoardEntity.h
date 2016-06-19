/* 

Amine Rehioui
Created: May 31th 2010

*/

#ifndef _BILLBOARD_ENTITY_H_INCLUDED_
#define _BILLBOARD_ENTITY_H_INCLUDED_

namespace shoot
{
	//! 3D BillBoard entity
	class BillBoardEntity : public Entity3D				 
	{
	public:

		//! Macro to provide type information
		DECLARE_OBJECT(BillBoardEntity, Entity3D);

		//! Constructor
		BillBoardEntity();

		//! serializes the entity to/from a PropertyStream
		virtual void Serialize(PropertyStream& stream);

		//! called during the initialization of the entity
		virtual void Init();

		//! called during the update of the entity		
		virtual void Update();

		//! returns the affine transformation matrix
		Matrix44 GetTransformationMatrix();

		//! sets the scaling vector
		void SetScale(const Vector3& vScale) 
		{
			m_vScale = vScale;
			m_bLocalTransformationMatrixDirty = true;
		}

	private:

		Vector3 m_vPreviousScale;
	};
}

#endif // _BILLBOARD_ENTITY_H_INCLUDED_

