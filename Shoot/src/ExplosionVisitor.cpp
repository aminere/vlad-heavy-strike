/* 

Amine Rehioui
Created: May 6th 2012

*/

#include "Shoot.h"

#include "ExplosionVisitor.h"

#include "MeshEntity.h"

namespace shoot
{
	DEFINE_OBJECT(ExplosionVisitor);

	//! constructor
	ExplosionVisitor::ExplosionVisitor()
		: m_fDefaultIntensity(10.0f)
		, m_fDefaultDuration(1.3f)
		// properties
		, m_fIntensity(-1.0f)
		, m_fDuration(-1.0f)
		, m_bDepthTest(false)
	{
	}

	//! destructor
	ExplosionVisitor::~ExplosionVisitor()
	{
		if(m_OriginalVertexBuffers.size())
		{
			Leave();
		}
	}

	//! Reads/Writes struct properties from/to a stream
	void ExplosionVisitor::Serialize(PropertyStream& stream)
	{
		stream.Serialize(PT_Float, "Intensity", &m_fIntensity);
		stream.Serialize(PT_Float, "Duration", &m_fDuration);
		stream.Serialize(PT_Bool, "DepthTest", &m_bDepthTest);
		stream.SerializeReference("Mesh", &m_Mesh);
		stream.SerializeArray("ChunkGroups", &m_aChunkGroups, PT_Struct);		
	}

	//! visits a particular entity
	void ExplosionVisitor::Visit(Entity* pTarget)
	{
		SHOOT_ASSERT(pTarget->IsA(MeshEntity::TypeID), "ExplosionVisitor target is not of type MeshEntity");
		MeshEntity* pMesh = static_cast<MeshEntity*>(pTarget);
		if(m_Mesh.IsValid())
		{
			SHOOT_ASSERT(m_Mesh.Get() == pMesh->GetMesh(), "ExplosionVisitor not meant to be applied on this mesh");
		}
		else
		{
			m_Mesh = pMesh->GetMesh();
		}

		m_OriginalVertexBuffers.clear();
		m_VertexBuffers.clear();
		m_OriginalMaterials.clear();

		std::vector<SubMesh*>& subMeshes = pMesh->GetSubMeshes();
		for(u32 i=0; i<subMeshes.size(); ++i)
		{
			GraphicComponent* pGraphic = subMeshes[i]->GetComponent<GraphicComponent>();
			VertexBuffer* pSourceVB = pGraphic->GetVertexBuffer();
			SHOOT_ASSERT(pSourceVB->GetDynamic(), "ExplosionVisitor needs a dynamic vertex buffer");
			SHOOT_ASSERT(pSourceVB->GetPrimitiveType() == GraphicsDriver::PT_Triangle, "ExplosionVisitor only supported triangulated meshes for now");
			SHOOT_ASSERT(pSourceVB->GetIndices(), "ExplosionVisitor only works with indexed vertex buffers");

			// set-up vertex buffer based on chunks
			u16* pSourceIndices = pSourceVB->GetIndices();
			Vertex3D* pSourceVertices = pSourceVB->GetVertices();
			Vertex3D* pVertices = snew Vertex3D[pSourceVB->GetNumVertices()];
			memcpy(pVertices, pSourceVertices, pSourceVB->GetNumVertices()*sizeof(Vertex3D));

			u16* pIndices = snew u16[pSourceVB->GetNumIndices()];
			memset(pIndices, 0, sizeof(u16)*pSourceVB->GetNumIndices());

			// copy the indices defined by the chunks
			// also, calculate the position (center of transformation) for each chunk
			u32 currentIndex = 0;
			Array<Chunk>& aChunks = m_aChunkGroups[i].m_aChunks;
			for(u32 j=0; j<aChunks.GetSize(); ++j)
			{
				aChunks[j].Reset();

				for(u32 k=0; k<aChunks[j].aTriangleIndices.GetSize(); ++k)
				{
					u32 triangleIndex = aChunks[j].aTriangleIndices[k]*3;
					for(u32 l=0; l<3; ++l)
					{
						pIndices[currentIndex] = pSourceIndices[triangleIndex+l];
						pVertices[pIndices[currentIndex]].color = Color::White;

						aChunks[j].vCenter += pVertices[triangleIndex+l].Pos;
						++currentIndex;
					}
				}
				aChunks[j].vCenter = aChunks[j].vCenter / (f32(aChunks[j].aTriangleIndices.GetSize())*3);
				aChunks[j].vDirection = ((const Vector3&)aChunks[j].vCenter).Normalize();
				aChunks[j].vDirection.Z = Math::FAbs(aChunks[j].vDirection.Z);

				// randomize the direction a bit
				f32 fRandom = 1.0f;
				Vector3 vRandom = Vector3::Create(Random::GetFloat(-fRandom, fRandom), Random::GetFloat(-fRandom, fRandom), Random::GetFloat(0.0f, fRandom));
				aChunks[j].vDirection += vRandom;
			}
			
			Material* pMaterial = static_cast<Material*>(pGraphic->GetMaterial()->Copy());			
			pMaterial->SetFlag(Material::MF_AlphaBlending, true);
			pMaterial->SetFlag(Material::MF_DepthTest, m_bDepthTest);
			pMaterial->SetCullMode(GraphicsDriver::CM_None);
			m_OriginalMaterials.push_back(Reference<Material>(pGraphic->GetMaterial()));
			pGraphic->SetMaterial(pMaterial);
			pGraphic->SetRenderingPriority(1);
			
			VertexBuffer* pVB = static_cast<VertexBuffer*>(pSourceVB->Copy());
			pVB->SetVertices(pVertices, pSourceVB->GetNumVertices());
			pVB->SetIndices(pIndices, currentIndex);
			pVB->SetVertexFlag(Vertex3D::VF_Color);

			m_OriginalVertexBuffers.push_back(Reference<VertexBuffer>(pSourceVB));
			m_VertexBuffers.push_back(Reference<VertexBuffer>(pVB));
			pGraphic->SetVertexBuffer(pVB);
		}

		f32 fDuration = (m_fDuration < 0.0f) ? m_fDefaultDuration : m_fDuration;
		m_Timer.Start(fDuration);

		super::Visit(pTarget);
	}

	//! Leaves the visited entity
	void ExplosionVisitor::Leave()
	{
		MeshEntity* pMesh = static_cast<MeshEntity*>(m_pTarget);
		std::vector<SubMesh*>& subMeshes = pMesh->GetSubMeshes();
		for(u32 i=0; i<subMeshes.size(); ++i)
		{
			GraphicComponent* pGraphic = subMeshes[i]->GetComponent<GraphicComponent>();
			pGraphic->SetVertexBuffer(m_OriginalVertexBuffers[i]);
			pGraphic->SetMaterial(m_OriginalMaterials[i]);
		}

		m_OriginalVertexBuffers.clear();
		m_OriginalMaterials.clear();
		m_VertexBuffers.clear();

		super::Leave();
	}

	//! updates the visitor
	void ExplosionVisitor::Update()
	{
		if(!m_bActive)
		{
			return;
		}

		f32 fIntensity = (m_fIntensity < 0.0f) ? m_fDefaultIntensity : m_fIntensity;
		for(u32 i=0; i<m_VertexBuffers.size(); ++i)
		{
			VertexBuffer* pVB = m_VertexBuffers[i];
			Vertex3D* pVertices = pVB->GetVertices();
			Vertex3D* pOriginalVertices = m_OriginalVertexBuffers[i]->GetVertices();
			u16* pIndices = m_OriginalVertexBuffers[i]->GetIndices();

			Array<Chunk>& aChunks = m_aChunkGroups[i].m_aChunks;
			for(u32 j=0; j<aChunks.GetSize(); ++j)
			{
				Vector3 vSpeed = (aChunks[j].vDirection*fIntensity*g_fDeltaTime);						
				aChunks[j].vPosition += vSpeed;
				aChunks[j].vRotation += vSpeed/3.0f;

				Matrix44 matrix = Matrix44::Identity;
				matrix.Translate(-aChunks[j].vCenter);
				matrix.Rotate(aChunks[j].vRotation);
				matrix.Translate(aChunks[j].vCenter + aChunks[j].vPosition);			

				for(u32 k=0; k<aChunks[j].aTriangleIndices.GetSize(); ++k)
				{
					u32 triangleIndex = aChunks[j].aTriangleIndices[k]*3;
					for(u32 l=0; l<3; ++l)
					{
						Vertex3D& v = pVertices[pIndices[triangleIndex+l]];
						Vertex3D& vOriginal = pOriginalVertices[pIndices[triangleIndex+l]];
						v.Pos = matrix.TransformVect(vOriginal.Pos);
						v.color.A = (1.0f-(m_Timer.GetProgressRatio()*m_Timer.GetProgressRatio()));					
					}
				}
			}

			pVB->SetDirty(true);
		}

		if(!m_Timer.IsRunning())
		{
			m_bActive = false;
		}
	}

	//! Reads/Writes struct properties from/to a stream
	void ExplosionVisitor::Chunk::Serialize(PropertyStream& stream)
	{
		stream.SerializeArray("Triangles", &aTriangleIndices, PT_UInt);		
	}
}

