/* 

Amine Rehioui
Created: May 30th 2010

*/

#include "Shoot.h"

#include "ParticleGenerator.h"

#include "MaterialProvider.h"

#include "GraphicExtensionHandler.h"

namespace shoot
{
	DEFINE_OBJECT(ParticleGenerator);

	//! constructor
	ParticleGenerator::ParticleGenerator()
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
		m_vDirection.Set(0.0f, 0.0f, 1.0f);
		m_vGravity = Vector3::Zero;
		m_EmitBox = m_BoundingBox = AABBox3D(Vector3::Zero, Vector3::One);
	}

	//! serializes the entity to/from a PropertyStream
	void ParticleGenerator::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);
		
		bool bMaxParticlesChanged = stream.Serialize(PT_UInt, "MaxParticles", &m_MaxParticles);
		stream.Serialize(PT_Int, "ParticlesPerSecond", &m_ParticlesPerSecond);
		stream.Serialize(PT_Range, "ParticleLife", &m_rParticleLife);
		stream.Serialize(PT_Range, "ParticleSpeed", &m_rParticleInitialSpeed);
		bool bParticleSizeChanged = stream.Serialize(PT_Float, "ParticleSize", &m_fParticleSize);
		stream.Serialize(PT_Vec3, "Direction", &m_vDirection);
		stream.Serialize(PT_Vec3, "Gravity", &m_vGravity);
		stream.Serialize(PT_AABBox3D, "EmitBox", &m_EmitBox);
		stream.Serialize(PT_AABBox3D, "BoundingBox", &m_BoundingBox);
		stream.Serialize(PT_UInt, "PoolSize", &m_PoolSize);
		if(stream.Serialize(PT_Float, "ActiveTime", &m_fActiveTime))
		{
			m_bActive = true;
		}
		stream.Serialize(PT_Bool, "Explosive", &m_bExplosive);
		bool bApplyWorldTransformChanged = stream.Serialize(PT_Bool, "ApplyWorldTransform", &m_bApplyWorldTransform);		
		
		if(IsInitialized())
		{
			VertexBuffer* pVertexBuffer = GetComponent<GraphicComponent>()->GetVertexBuffer();
			bool bPointSprite = GraphicExtensionHandler::Instance()->HasExtension(GraphicExtensionHandler::E_PointSprite);
			if(bMaxParticlesChanged)
			{
				u32 numVertices = bPointSprite ? m_MaxParticles : m_MaxParticles*6;
				pVertexBuffer->SetVertices(snew Vertex3D[numVertices], numVertices);
			}
			if(bParticleSizeChanged && bPointSprite)
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
	void ParticleGenerator::Init()
	{
		super::Init();

		m_Pool = snew MemoryPool(m_PoolSize);

		VertexBuffer* pVertexBuffer = GraphicsDriver::Instance()->CreateVertexBuffer();		
		pVertexBuffer->SetVertexFlag(Vertex3D::VF_Pos);
		pVertexBuffer->SetVertexFlag(Vertex3D::VF_Color);
		pVertexBuffer->SetName(GetClassName());
		pVertexBuffer->SetDynamic(true);
		pVertexBuffer->SetApplyWorldTransforms(m_bApplyWorldTransform);

		if(GraphicExtensionHandler::Instance()->HasExtension(GraphicExtensionHandler::E_PointSprite))
		{
			pVertexBuffer->SetPrimitiveType(GraphicsDriver::PT_Point);
			pVertexBuffer->SetVertices(snew Vertex3D[m_MaxParticles], m_MaxParticles);
			pVertexBuffer->SetRenderStateSetter(snew PointSpriteStateSetter(m_fParticleSize));
		}
		else
		{
			pVertexBuffer->SetPrimitiveType(GraphicsDriver::PT_Triangle);
			pVertexBuffer->SetVertices(snew Vertex3D[m_MaxParticles*6], m_MaxParticles*6);
			pVertexBuffer->SetVertexFlag(Vertex3D::VF_UV);
		}

		GetComponent<GraphicComponent>()->SetVertexBuffer(pVertexBuffer);
	}

	//! called during the update of the entity	
	void ParticleGenerator::Update()
	{
		// generate new particles
		if(m_bActive)
		{
			m_fNewParticlesCounter += g_fDeltaTime * m_ParticlesPerSecond;
			u32 numNewParticles = u32(m_fNewParticlesCounter);
			if(m_aParticles.size() + numNewParticles > m_MaxParticles)
			{
				numNewParticles = m_MaxParticles - (u32)m_aParticles.size();
			}

			for(u32 i=0; i<numNewParticles; ++i)
			{
				if(Particle* pParticle = m_Pool->Alloc<Particle>())
				{
					pParticle->InitialLife = Random::GetFloat(m_rParticleLife.Min, m_rParticleLife.Max);
					pParticle->Life = pParticle->InitialLife;

					pParticle->Position = Vector3::Create(Random::GetFloat(m_EmitBox.Min().X, m_EmitBox.Max().X),
						Random::GetFloat(m_EmitBox.Min().Y, m_EmitBox.Max().Y),
						Random::GetFloat(m_EmitBox.Min().Z, m_EmitBox.Max().Z));

					Vector3 vCenter = m_EmitBox.Min()+m_EmitBox.Size()/2.0f;
					if(!m_bApplyWorldTransform)
					{
						Vector3 vTranslation = GetTransformationMatrix().GetTranslation();
						pParticle->Position += vTranslation;
						vCenter += vTranslation;
					}

					if(m_bExplosive)
					{
						pParticle->Speed = (pParticle->Position-vCenter).Normalize();
						pParticle->Speed *= Random::GetFloat(m_rParticleInitialSpeed.Min, m_rParticleInitialSpeed.Max);
					}
					else
					{
						pParticle->Speed = Vector3::Create(m_vDirection.X * Random::GetFloat(m_rParticleInitialSpeed.Min, m_rParticleInitialSpeed.Max),
							m_vDirection.Y * Random::GetFloat(m_rParticleInitialSpeed.Min, m_rParticleInitialSpeed.Max),
							m_vDirection.Z * Random::GetFloat(m_rParticleInitialSpeed.Min, m_rParticleInitialSpeed.Max));
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
		const Matrix44& viewInverse = EntityRenderer::Instance()->Get3DCamera()->GetViewMatrixInverse();
		const f32 fHalfSize = m_fParticleSize/2.0f;
		const bool bPointSprite = GraphicExtensionHandler::Instance()->HasExtension(GraphicExtensionHandler::E_PointSprite);

		u32 vtx = 0;
		for(std::list<Particle*>::iterator it = m_aParticles.begin(); it != m_aParticles.end(); )
		{
			if((*it)->Life > 0.0f)
			{				
				(*it)->Position += (*it)->Speed * g_fDeltaTime;
				(*it)->Speed += m_vGravity * g_fDeltaTime;
				(*it)->Alpha = (*it)->Life / (*it)->InitialLife;
				(*it)->Life -= g_fDeltaTime;

				const Color& color = pMaterial->GetColor();
				Vertex3D* pVertices = pVertexBuffer->GetVertices();
				const Vector3& pos = (*it)->Position;

				if(bPointSprite)
				{
					pVertices[vtx].Pos = pos;
					pVertices[vtx].color = Color(color.R, color.G, color.B, (*it)->Alpha);
					++vtx;
				}
				else
				{
					Vector3 vScale = GetAbsoluteScale();
					pVertices[vtx+0].UV = Vector2(0.0f, 1.0f); pVertices[vtx+0].Pos = Vector3::Create(-fHalfSize, -fHalfSize, 0.0f)*vScale;
					pVertices[vtx+1].UV = Vector2(0.0f, 0.0f); pVertices[vtx+1].Pos = Vector3::Create(-fHalfSize, fHalfSize, 0.0f)*vScale;
					pVertices[vtx+2].UV = Vector2(1.0f, 0.0f); pVertices[vtx+2].Pos = Vector3::Create(fHalfSize, fHalfSize, 0.0f)*vScale;
					pVertices[vtx+3].UV = Vector2(0.0f, 1.0f); pVertices[vtx+3].Pos = Vector3::Create(-fHalfSize, -fHalfSize, 0.0f)*vScale;
					pVertices[vtx+4].UV = Vector2(1.0f, 0.0f); pVertices[vtx+4].Pos = Vector3::Create(fHalfSize, fHalfSize, 0.0f)*vScale;
					pVertices[vtx+5].UV = Vector2(1.0f, 1.0f); pVertices[vtx+5].Pos = Vector3::Create(fHalfSize, -fHalfSize, 0.0f)*vScale;
					Matrix44 transform = viewInverse;
					transform.SetTranslation(super::GetTransformationMatrix().TransformVect(pos));
					for(u32 i=0; i<6; ++i)
					{
						pVertices[vtx+i].Pos = transform.TransformVect(pVertices[vtx+i].Pos);
						pVertices[vtx+i].color = Color(color.R, color.G, color.B, (*it)->Alpha);
					}
					vtx += 6;
				}

				++it;
			}
			else
			{
				// remove particle
				m_Pool->Free((*it));
				it = m_aParticles.erase(it);
			}
		}

		pVertexBuffer->SetNumVertices(vtx);
		pVertexBuffer->SetDirty(true);
	}

	//! returns the affine transformation matrix
	Matrix44 ParticleGenerator::GetTransformationMatrix()
	{
		if(GraphicExtensionHandler::Instance()->HasExtension(GraphicExtensionHandler::E_PointSprite))
		{
			return super::GetTransformationMatrix();
		}
		return Matrix44::Identity;
	}

	//! Get the local axis-aligned bounding box
	AABBox3D ParticleGenerator::GetBoundingBox() const
	{ 
		const Entity3D* pEntity3D = static_cast<const Entity3D*>(this);
		return m_BoundingBox + const_cast<Entity3D*>(pEntity3D)->Entity3D::GetTransformationMatrix().GetTranslation();
	}
}

