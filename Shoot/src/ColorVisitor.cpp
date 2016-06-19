/* 

Amine Rehioui
Created: April 9th 2011

*/

#include "Shoot.h"	

#include "ColorVisitor.h"

#include "ExplosionVisitor.h"

#include "MeshEntity.h"

namespace shoot
{
	DEFINE_OBJECT(ColorVisitor);

	//! constructor
	ColorVisitor::ColorVisitor()
		// properties
		: m_fAlpha(1.0f)
		, m_fDuration(1.0f)
	{
	}

	//! destructor
	ColorVisitor::~ColorVisitor()
	{
	}

	//! Reads/Writes struct properties from/to a stream
	void ColorVisitor::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);
		
		stream.Serialize(PT_Color, "Color", &m_Color);

		stream.Serialize(PT_Float, "Alpha", &m_fAlpha);
		m_Color.A = m_fAlpha;

		stream.Serialize(PT_Float, "Duration", &m_fDuration);
	}

	//! visits a particular entity
	void ColorVisitor::Visit(Entity* pTarget)
	{
		SHOOT_ASSERT(pTarget->IsA(MeshEntity::TypeID), "ColorVisitor target is not of type MeshEntity");
		SHOOT_ASSERT(pTarget->GetComponent<ExplosionVisitor>() == NULL, "Can't apply ColorVisitor and ExplosionVisitor simultaneously");

		m_OriginalVertexBuffers.clear();
		m_VertexBuffers.clear();

		MeshEntity* pMesh = static_cast<MeshEntity*>(pTarget);
		std::vector<SubMesh*>& subMeshes = pMesh->GetSubMeshes();
		for(u32 i=0; i<subMeshes.size(); ++i)
		{
			GraphicComponent* pGraphic = subMeshes[i]->GetComponent<GraphicComponent>();
			VertexBuffer* pSourceVB = pGraphic->GetVertexBuffer();
			SHOOT_ASSERT(pSourceVB->GetDynamic(), "ColorVisitor needs a dynamic vertex buffer");
			SHOOT_ASSERT(pSourceVB->GetPrimitiveType() == GraphicsDriver::PT_Triangle, "ColorVisitor only supported triangulated meshes for now");
			SHOOT_ASSERT(pSourceVB->GetIndices(), "ColorVisitor only works with indexed vertex buffers");

			VertexBuffer* pVB = static_cast<VertexBuffer*>(pSourceVB->Copy());
			Vertex3D* pVertices = snew Vertex3D[pSourceVB->GetNumVertices()];
			u16* pIndices = snew u16[pSourceVB->GetNumIndices()];
			memcpy(pVertices, pSourceVB->GetVertices(), pSourceVB->GetNumVertices()*sizeof(Vertex3D));
			memcpy(pIndices, pSourceVB->GetIndices(), pSourceVB->GetNumIndices()*sizeof(u16));
			pVB->SetVertices(pVertices, pSourceVB->GetNumVertices());
			pVB->SetIndices(pIndices, pSourceVB->GetNumIndices());
			pVB->SetVertexFlag(Vertex3D::VF_Color);

			m_OriginalVertexBuffers.push_back(pSourceVB);
			m_VertexBuffers.push_back(pVB);
			pGraphic->SetVertexBuffer(pVB);			
		}

		m_fInterpolator = 0.0f;
		GraphicComponent* pGraphic = subMeshes[0]->GetComponent<GraphicComponent>();
		m_SrcColor = pGraphic->GetMaterial()->GetColor();
		m_DestColor = m_Color;

		super::Visit(pTarget);
	}

	//! Leaves the visited entity
	void ColorVisitor::Leave()
	{
		// Optim - entities that are faded out are usually thrown away
		//MeshEntity* pMesh = static_cast<MeshEntity*>(m_pTarget);
		//std::vector<SubMesh*>& subMeshes = pMesh->GetSubMeshes();
		//for(u32 i=0; i<subMeshes.size(); ++i)
		//{
		//	GraphicComponent* pGraphic = subMeshes[i]->GetComponent<GraphicComponent>();
		//	pGraphic->SetVertexBuffer(m_OriginalVertexBuffers[i]);
		//}
		//m_OriginalVertexBuffers.clear();
		//m_VertexBuffers.clear();

		super::Leave();
	}

	//! updates the visitor	
	void ColorVisitor::Update()
	{
		if(!m_bActive)
		{
			return;
		}

		if(m_fInterpolator < 1.0f)
		{
			Color color = Math::Lerp(m_SrcColor, m_DestColor, m_fInterpolator);
			UpdateColor(color);

			m_fInterpolator += (g_fDeltaTime/m_fDuration);
		}		
		else
		{
			UpdateColor(m_DestColor);

			m_iPlayCount++;			

			switch(m_ePlaybackType)
			{
			case PT_Once:
				Leave();
				break;

			case PT_Loop:
			case PT_Toggle:
				if((m_iMaxPlayCount < 0) || (m_iPlayCount < m_iMaxPlayCount))
				{	
					(m_ePlaybackType == PT_Toggle) ? Math::Swap(m_SrcColor, m_DestColor) : void();
					m_fInterpolator = 0.0f;
				}
				else
				{
					Leave();
				}
				break;
				
			default:
				Leave();								
			}			
		}
	}

	//! updates the color
	void ColorVisitor::UpdateColor(const Color& color)
	{
		for(u32 i=0; i<m_VertexBuffers.size(); ++i)
		{
			VertexBuffer* pVB = m_VertexBuffers[i];
			Vertex3D* pVertices = pVB->GetVertices();
			for(u32 i=0; i<pVB->GetNumVertices(); ++i)
			{
				pVertices[i].color = color;
			}
			pVB->SetDirty(true);
		}
	}
}

