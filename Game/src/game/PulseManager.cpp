/* 

Amine Rehioui
Created: February 5th 2012

*/

#include "ShootTest.h"

#include "PulseManager.h"

#include "MaterialProvider.h"

#include "GraphicExtensionHandler.h"

namespace shoot
{
	DEFINE_OBJECT(PulseManager);

	//! Constructor
	PulseManager::PulseManager()
		// properties
		: m_fBulletSize(8.0f)
	{
	}

	//! destructor
	PulseManager::~PulseManager()
	{
	}

	//! serializes the entity to/from a PropertyStream
	void PulseManager::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);

		bool bBulletSizeChanged = stream.Serialize(PT_Float, "BulletSize", &m_fBulletSize);

		if(bBulletSizeChanged && IsInitialized())
		{
			SetBulletSize(m_fBulletSize);
		}		
	}

	//! called during the initialization of the entity
	void PulseManager::Init()
	{
		super::Init();
		
		GraphicComponent* pGraphic = GetComponent<GraphicComponent>();

		if(!pGraphic->GetMaterial())
		{
			pGraphic->SetMaterial(snew Material());
		}

		u32 maxBullets = m_BulletPoolSize/sizeof(Bullet);
		VertexBuffer* pVB = GraphicsDriver::Instance()->CreateVertexBuffer();		
		pVB->SetVertexFlag(Vertex3D::VF_Pos);
		pVB->SetName(GetClassName());
		if(GraphicExtensionHandler::Instance()->HasExtension(GraphicExtensionHandler::E_PointSprite))
		{
			pVB->SetPrimitiveType(GraphicsDriver::PT_Point);
			pVB->SetVertices(snew Vertex3D[maxBullets], maxBullets);
			pVB->SetRenderStateSetter(snew PointSpriteStateSetter(m_fBulletSize));
		}
		else
		{
			pVB->SetPrimitiveType(GraphicsDriver::PT_Triangle);
			pVB->SetVertices(snew Vertex3D[maxBullets*6], maxBullets*6);
			pVB->SetVertexFlag(Vertex3D::VF_UV);
		}

		pGraphic->SetVertexBuffer(pVB);

		pVB->SetDynamic(true);
	}	

	//! Prepares a bullet for rendering
	void PulseManager::SetupRendering(Bullet* pBullet, u32 index, VertexBuffer* pVB)
	{
		Vertex3D* pVertices = pVB->GetVertices();
		if(GraphicExtensionHandler::Instance()->HasExtension(GraphicExtensionHandler::E_PointSprite))
		{
			pVertices[index].Pos = pBullet->vPosition;
			pVB->SetNumVertices(index+1);
		}
		else
		{
			const Matrix44& viewInverse = EntityRenderer::Instance()->Get3DCamera()->GetViewMatrixInverse();
			const f32 fHalfSize = pBullet->fRadius;
			const u32 vtx = index*6;
			pVertices[vtx+0].UV = Vector2(0.0f, 1.0f); pVertices[vtx+0].Pos = Vector3::Create(-fHalfSize, -fHalfSize, 0.0f);
			pVertices[vtx+1].UV = Vector2(0.0f, 0.0f); pVertices[vtx+1].Pos = Vector3::Create(-fHalfSize, fHalfSize, 0.0f);
			pVertices[vtx+2].UV = Vector2(1.0f, 0.0f); pVertices[vtx+2].Pos = Vector3::Create(fHalfSize, fHalfSize, 0.0f);
			pVertices[vtx+3].UV = Vector2(0.0f, 1.0f); pVertices[vtx+3].Pos = Vector3::Create(-fHalfSize, -fHalfSize, 0.0f);
			pVertices[vtx+4].UV = Vector2(1.0f, 0.0f); pVertices[vtx+4].Pos = Vector3::Create(fHalfSize, fHalfSize, 0.0f);
			pVertices[vtx+5].UV = Vector2(1.0f, 1.0f); pVertices[vtx+5].Pos = Vector3::Create(fHalfSize, -fHalfSize, 0.0f);
			Matrix44 transform = viewInverse;
			transform.SetTranslation(pBullet->vPosition);
			for(u32 i=0; i<6; ++i)
			{
				pVertices[vtx+i].Pos = transform.TransformVect(pVertices[vtx+i].Pos);				
			}
			pVB->SetNumVertices(vtx+6);
		}
	}

	//! sets the bullet size
	void PulseManager::SetBulletSize(f32 fSize)
	{
		VertexBuffer* pVB = GetComponent<GraphicComponent>()->GetVertexBuffer();
		if(GraphicExtensionHandler::Instance()->HasExtension(GraphicExtensionHandler::E_PointSprite))
		{
			static_cast<PointSpriteStateSetter*>(pVB->GetRenderStateSetter())->SetPointSize(fSize);
		}
		m_fBulletSize = fSize;
	}
}

