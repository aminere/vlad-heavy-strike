/* 

Amine Rehioui
Created: May 30th 2010

*/

#include "Shoot.h"

#include "ParticleGenerator2D.h"

#include "MaterialProvider.h"

namespace shoot
{
	DEFINE_OBJECT(ParticleGenerator2D);

	//! constructor
	ParticleGenerator2D::ParticleGenerator2D()
	: m_bActive(true)
	// properties
	, m_MaxParticles(200)
	, m_ParticlesPerSecond(30)
	, m_rParticleLife(1.0f, 2.0f)
	, m_rParticleInitialSpeed(1.0f, 1.5f)
	, m_fParticleSize(1.0f)
	, m_fNewParticlesCounter(0.0f)
	, m_PoolSize(4096)
	, m_fActiveTime(-1.0f)
	, m_bExplosive(false)
	, m_bApplyWorldTransform(true)
	{
		m_vDirection.Set(0.0f, 1.0f);
		m_BoundingBox.Set(Vector2(), Vector2(10.0f, 10.0f));
	}

	//! serializes the entity to/from a PropertyStream
	void ParticleGenerator2D::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);
		
		bool bMaxParticlesChanged = stream.Serialize(PT_UInt, "MaxParticles", &m_MaxParticles);
		stream.Serialize(PT_Int, "ParticlesPerSecond", &m_ParticlesPerSecond);
		stream.Serialize(PT_Range, "ParticleLife", &m_rParticleLife);
		stream.Serialize(PT_Range, "ParticleSpeed", &m_rParticleInitialSpeed);
		bool bParticleSizeChanged = stream.Serialize(PT_Float, "ParticleSize", &m_fParticleSize);
		stream.Serialize(PT_Vec2, "Direction", &m_vDirection);
		stream.Serialize(PT_Vec2, "Gravity", &m_vGravity);
		stream.Serialize(PT_UInt, "PoolSize", &m_PoolSize);
		bool bApplyWorldTransformChanged = stream.Serialize(PT_Bool, "ApplyWorldTransform", &m_bApplyWorldTransform);
		
		if(stream.Serialize(PT_Float, "ActiveTime", &m_fActiveTime))
		{
			m_bActive = true;
		}
		stream.Serialize(PT_Bool, "Explosive", &m_bExplosive);		
		
		if(IsInitialized())
		{
			VertexBuffer* pVertexBuffer = GetComponent<GraphicComponent>()->GetVertexBuffer();
			if(bMaxParticlesChanged)
			{
				pVertexBuffer->SetVertices(snew Vertex3D[m_MaxParticles], m_MaxParticles);
			}
			if(bParticleSizeChanged)
			{
				static_cast<PointSpriteStateSetter*>(pVertexBuffer->GetRenderStateSetter())->SetPointSize(m_fParticleSize);
			}
			if(bApplyWorldTransformChanged)
			{
				pVertexBuffer->SetApplyWorldTransforms(m_bApplyWorldTransform);
			}
		}
	}	

	//! called during the initialization of the entity
	void ParticleGenerator2D::Init()
	{
		super::Init();

		m_Pool = snew MemoryPool(m_PoolSize);

		VertexBuffer* pVertexBuffer = GraphicsDriver::Instance()->CreateVertexBuffer();		
		pVertexBuffer->SetVertexFlag(Vertex3D::VF_Pos);
		pVertexBuffer->SetVertexFlag(Vertex3D::VF_Color);
		pVertexBuffer->SetPrimitiveType(GraphicsDriver::PT_Point);
		pVertexBuffer->SetVertices(snew Vertex3D[m_MaxParticles], m_MaxParticles);
		pVertexBuffer->SetRenderStateSetter(snew PointSpriteStateSetter(m_fParticleSize));
		pVertexBuffer->SetName(GetClassName());
		pVertexBuffer->SetDynamic(true);
		pVertexBuffer->SetApplyWorldTransforms(m_bApplyWorldTransform);
		GetComponent<GraphicComponent>()->SetVertexBuffer(pVertexBuffer);
	}

	//! called during the update of the entity	
	void ParticleGenerator2D::Update()
	{
		// generate new particles
		if(m_bActive)
		{
			m_fNewParticlesCounter += g_fDeltaTime * m_ParticlesPerSecond;
			u32 numNewParticles = u32(m_fNewParticlesCounter);
			if(m_aParticles.size() + numNewParticles > m_MaxParticles)
			{
				numNewParticles = m_MaxParticles-m_aParticles.size();
			}

			for(u32 i=0; i<numNewParticles; ++i)
			{
				if(Particle* pParticle = m_Pool->Alloc<Particle>())
				{
					pParticle->InitialLife = Random::GetFloat(m_rParticleLife.Min, m_rParticleLife.Max);
					pParticle->Life = pParticle->InitialLife;

					pParticle->Position = Vector2(Random::GetFloat(m_BoundingBox.Min().X, m_BoundingBox.Max().X),
						Random::GetFloat(m_BoundingBox.Min().Y, m_BoundingBox.Max().Y));
					Vector2 vCenter = m_BoundingBox.Min()+m_BoundingBox.Size()/2.0f;

					if(!m_bApplyWorldTransform)
					{
						Vector3 vTranslation = GetTransformationMatrix().GetTranslation();
						pParticle->Position += Vector2(vTranslation.X, vTranslation.Y);
						vCenter += Vector2(vTranslation.X, vTranslation.Y);
					}

					if(m_bExplosive)
					{
						pParticle->Speed = (pParticle->Position-vCenter).Normalize();
						pParticle->Speed *= Random::GetFloat(m_rParticleInitialSpeed.Min, m_rParticleInitialSpeed.Max);
					}
					else
					{
						pParticle->Speed = Vector2(m_vDirection.X * Random::GetFloat(m_rParticleInitialSpeed.Min, m_rParticleInitialSpeed.Max),
							m_vDirection.Y * Random::GetFloat(m_rParticleInitialSpeed.Min, m_rParticleInitialSpeed.Max));
					}

					pParticle->Alpha = 1.0f;
					m_aParticles.push_back(pParticle);
				}
			}

			m_fNewParticlesCounter -= numNewParticles;
			if(m_fActiveTime >= 0.0f)
			{
				m_fActiveTime -= g_fDeltaTime;
				if(m_fActiveTime < 0.0f)
				{
					m_bActive = false;
				}
			}
		}

		// update particles
		GraphicComponent* pGraphic = GetComponent<GraphicComponent>();
		VertexBuffer* pVertexBuffer = pGraphic->GetVertexBuffer();
		Material* pMaterial = pGraphic->GetMaterial();

		u32 currentVertex = 0;
		for(std::list<Particle*>::iterator it = m_aParticles.begin(); it != m_aParticles.end(); )
		{
			if((*it)->Life > 0.0f)
			{				
				(*it)->Position += (*it)->Speed * g_fDeltaTime;
				(*it)->Speed += m_vGravity * g_fDeltaTime;
				(*it)->Alpha = (*it)->Life / (*it)->InitialLife;
				(*it)->Life -= g_fDeltaTime;

				pVertexBuffer->GetVertices()[currentVertex].Pos = Vector3::Create((*it)->Position.X, (*it)->Position.Y, 0.0f);

				const Color& color = pMaterial->GetColor();
				pVertexBuffer->GetVertices()[currentVertex].color = Color(color.R, color.G, color.B, (*it)->Alpha);

				++currentVertex;
				++it;
			}
			else
			{
				// remove particle
				m_Pool->Free((*it));
				it = m_aParticles.erase(it);
			}
		}

		pVertexBuffer->SetNumVertices(currentVertex);
		pVertexBuffer->SetDirty(true);
	}
}

