/* 

Amine Rehioui
Created: May 31th 2010

*/

#include "Shoot.h"

#include "BillBoardEntity.h"

#include "GeometryProvider.h"

namespace shoot
{
	DEFINE_OBJECT(BillBoardEntity);

	//! Constructor
	BillBoardEntity::BillBoardEntity()	
	{
		m_vPreviousScale = Vector3::One;
	}

	//! serializes the entity to/from a PropertyStream
	void BillBoardEntity::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);

		if(IsInitialized())
		{
			GetComponent<GraphicComponent>()->SetVertexBuffer(GeometryProvider::Instance()->Get3DQuad(Vector2(m_vScale.X, m_vScale.Y)));
			m_vPreviousScale = m_vScale;
		}
	}

	//! called during the initialization of the entity
	void BillBoardEntity::Init()
	{
		super::Init();

		GetComponent<GraphicComponent>()->SetVertexBuffer(GeometryProvider::Instance()->Get3DQuad(Vector2(m_vScale.X, m_vScale.Y)));
	}	

	//! called during the update of the entity		
	void BillBoardEntity::Update()
	{
		if(m_vPreviousScale != m_vScale)
		{
			GetComponent<GraphicComponent>()->SetVertexBuffer(GeometryProvider::Instance()->Get3DQuad(Vector2(m_vScale.X, m_vScale.Y)));
			m_vPreviousScale = m_vScale;
		}
	}

	//! returns the affine transformation matrix
	Matrix44 BillBoardEntity::GetTransformationMatrix()
	{
		if(Camera* pCamera = EntityRenderer::Instance()->Get3DCamera())
		{
			Matrix44 orientMatrix = pCamera->GetViewMatrixInverse();
			orientMatrix.SetTranslation(super::GetTransformationMatrix().GetTranslation());
			return orientMatrix;
		}
		return super::GetTransformationMatrix();
	}
}

