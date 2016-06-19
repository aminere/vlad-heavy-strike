/* 

Amine Rehioui
Created: September 25th 2011

*/

#include "ShootTest.h"

#include "Starfield.h"

#include "MaterialProvider.h"

#include "MeshEntity.h"

#include "GraphicExtensionHandler.h"

namespace shoot
{
	DEFINE_OBJECT(Starfield);

	//! constructor
	Starfield::Starfield()
		// properties
		: m_MaxParticles(100)
		, m_fRadius(100.f)
		, m_fFadeInDist(10.0f)
		, m_fParticleSize(2.0f)
		, m_fSpeed(5.0f)
	{
	}

	//! serializes the entity to/from a PropertyStream
	void Starfield::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);
		
		bool maxParticlesChanged = stream.Serialize(PT_UInt, "MaxParticles", &m_MaxParticles);
		bool radiusChanged = stream.Serialize(PT_Float, "Radius", &m_fRadius);
		stream.Serialize(PT_Float, "FadeInDist", &m_fFadeInDist);
		bool bParticleSizeChanged = stream.Serialize(PT_Float, "ParticleSize", &m_fParticleSize);
		stream.Serialize(PT_Float, "Speed", &m_fSpeed);		

		if(IsInitialized())
		{
			if(maxParticlesChanged || radiusChanged)
			{
				InitParticles();
			}
			if(bParticleSizeChanged && GraphicExtensionHandler::Instance()->HasExtension(GraphicExtensionHandler::E_PointSprite))
			{
				static_cast<PointSpriteStateSetter*>(GetComponent<GraphicComponent>()->GetVertexBuffer()->GetRenderStateSetter())->SetPointSize(m_fParticleSize);
			}
		}
	}	

	//! called during the initialization of the entity
	void Starfield::Init()
	{
		super::Init();

		GraphicComponent* pGraphic = GetComponent<GraphicComponent>();
		if(!pGraphic)
		{
			pGraphic = snew GraphicComponent();
			pGraphic->SetRenderingPass(GraphicComponent::RP_Transparent3D);
			AddComponent(pGraphic, true);
		}

		if(!pGraphic->GetMaterial())
		{
			pGraphic->SetMaterial(snew Material());
		}

		InitParticles();
	}

	//! called during the update of the entity	
	void Starfield::Update()
	{
		Player* pPlayer = Player::Instance();
		const Vector3& vPlayerPos = pPlayer->GetMeshEntity()->GetTransformationMatrix().GetTranslation();
		while(!m_lDeadParticles.empty())
		{
			Vector3 pos = Math::SphericalToCartesian(m_fRadius-0.01f, Random::GetFloat(0.0f, Math::PI), Random::GetFloat(0.0f, Math::PI));
			*m_lDeadParticles.front() = vPlayerPos + pos;
			m_lParticles.push_back(m_lDeadParticles.front());
			m_lDeadParticles.pop_front();
		}		

		const Matrix44& viewInverse = EntityRenderer::Instance()->Get3DCamera()->GetViewMatrixInverse();
		const f32 fHalfSize = m_fParticleSize/2.0f;
		Vector3 vSpeed = Vector3::Create(0.0f, -1.0f, 0.0f)*m_fSpeed*g_fDeltaTime;
		VertexBuffer* pVB = GetComponent<GraphicComponent>()->GetVertexBuffer();
		Vertex3D* pVertices = pVB->GetVertices();
		u32 vtx = 0;
		for(std::list<Vector3*>::iterator it = m_lParticles.begin(); it != m_lParticles.end(); )
		{
			Vector3* pPos = (*it);
			f32 dist = (vPlayerPos - (*pPos)).GetLength();
			if(dist <= m_fRadius)
			{
				const f32 fAlpha = Math::Clamp((m_fRadius - dist)/(m_fRadius-m_fFadeInDist), 0.0f, 1.0f);
				(*pPos) += vSpeed;

				if(GraphicExtensionHandler::Instance()->HasExtension(GraphicExtensionHandler::E_PointSprite))
				{
					pVertices[vtx].Pos = *pPos;
					pVertices[vtx].color = Color(1.0f, 1.0f, 1.0f, fAlpha);
					++vtx;
				}
				else
				{
					pVertices[vtx+0].UV = Vector2(0.0f, 1.0f); pVertices[vtx+0].Pos = Vector3::Create(-fHalfSize, -fHalfSize, 0.0f);
					pVertices[vtx+1].UV = Vector2(0.0f, 0.0f); pVertices[vtx+1].Pos = Vector3::Create(-fHalfSize, fHalfSize, 0.0f);
					pVertices[vtx+2].UV = Vector2(1.0f, 0.0f); pVertices[vtx+2].Pos = Vector3::Create(fHalfSize, fHalfSize, 0.0f);
					pVertices[vtx+3].UV = Vector2(0.0f, 1.0f); pVertices[vtx+3].Pos = Vector3::Create(-fHalfSize, -fHalfSize, 0.0f);
					pVertices[vtx+4].UV = Vector2(1.0f, 0.0f); pVertices[vtx+4].Pos = Vector3::Create(fHalfSize, fHalfSize, 0.0f);
					pVertices[vtx+5].UV = Vector2(1.0f, 1.0f); pVertices[vtx+5].Pos = Vector3::Create(fHalfSize, -fHalfSize, 0.0f);
					Matrix44 transform = viewInverse;
					transform.SetTranslation(*pPos);
					for(u32 i=0; i<6; ++i)
					{
						pVertices[vtx+i].Pos = transform.TransformVect(pVertices[vtx+i].Pos);
						pVertices[vtx+i].color = Color(1.0f, 1.0f, 1.0f, fAlpha);
					}
					vtx += 6;
				}

				++it;
			}
			else
			{
				m_lDeadParticles.push_back(pPos);
				it = m_lParticles.erase(it);
			}
		}

		pVB->SetNumVertices(vtx);
		pVB->SetDirty(true);
	}	
	
	//! init the particles
	void Starfield::InitParticles()
	{
		m_Pool = snew MemoryPool(m_MaxParticles*(sizeof(Vector3)+MemoryPool::AllocHeaderSize));
		m_lParticles.clear();

		VertexBuffer* pVB = GraphicsDriver::Instance()->CreateVertexBuffer();		
		pVB->SetVertexFlag(Vertex3D::VF_Pos);
		pVB->SetVertexFlag(Vertex3D::VF_Color);

		if(GraphicExtensionHandler::Instance()->HasExtension(GraphicExtensionHandler::E_PointSprite))
		{
			pVB->SetPrimitiveType(GraphicsDriver::PT_Point);
			pVB->SetRenderStateSetter(snew PointSpriteStateSetter(m_fParticleSize));
			pVB->SetVertices(snew Vertex3D[m_MaxParticles], m_MaxParticles);
		}
		else
		{
			pVB->SetPrimitiveType(GraphicsDriver::PT_Triangle);
			pVB->SetVertexFlag(Vertex3D::VF_UV);
			pVB->SetVertices(snew Vertex3D[m_MaxParticles*6], m_MaxParticles*6);
		}
		
		pVB->SetName(GetClassName());
		GetComponent<GraphicComponent>()->SetVertexBuffer(pVB);
		const Vector3& vPlayerPos = Player::Instance()->GetMeshEntity()->GetTransformationMatrix().GetTranslation();

		for(u32 i=0; i<m_MaxParticles; ++i)
		{
			Vector3* pPos = m_Pool->Alloc<Vector3>();
			*pPos = vPlayerPos + Math::SphericalToCartesian(Random::GetFloat(0.0f, m_fRadius), Random::GetFloat(0.0f, Math::PI*2.0f), Random::GetFloat(0.0f, Math::PI));
			m_lParticles.push_back(pPos);
		}

		pVB->SetDynamic(true);
	}	
}

