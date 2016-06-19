/* 

Amine Rehioui
Created: December 10th 2010

*/

#include "Shoot.h"

#include "VertexBuffer.h"

namespace shoot
{
	DEFINE_OBJECT(VertexBuffer);

	//! constructor
	VertexBuffer::VertexBuffer()
		: m_pVertices(NULL)
		, m_MaxVertices(0)
		, m_NumVertices(0)
		, m_AllocatedVertices(0)
		, m_pIndices(NULL)
		, m_MaxIndices(0)
		, m_NumIndices(0)
		, m_Flags(0)
		, m_ePrimitiveType(GraphicsDriver::PT_Triangle)
		, m_bDynamic(false)
		, m_bDirty(true)
		, m_bApplyWorldTransforms(true)
	{
	}

	//! destructor
	VertexBuffer::~VertexBuffer()
	{
		sdelete_array(m_pVertices);
		sdelete_array(m_pIndices);
	}

	//! begins rendering using this vertex buffer
	void VertexBuffer::Begin()
	{
		if(RenderStateSetter* pSetter = m_RenderStateSetter.Get())
		{
			pSetter->Begin();
		}

		if(m_bDirty)
		{
			UpdateData();
		}
	}

	//! ends rendering by this vertex buffer
	void VertexBuffer::End()
	{
		if(RenderStateSetter* pSetter = m_RenderStateSetter.Get())
		{
			pSetter->End();
		}
	}	

	//! sets the pointers to the vertices
	void VertexBuffer::SetVertices(Vertex3D* pVertices, u32 numVertices)
	{
		if(pVertices != m_pVertices)
		{
			sdelete_array(m_pVertices);
			m_pVertices = pVertices;
			m_MaxVertices = m_NumVertices = m_AllocatedVertices = numVertices;
			m_bDirty = true;
		}
	}

	//! sets the pointers to the indices
	void VertexBuffer::SetIndices(u16* pIndices, u32 numIndices)
	{
		if(pIndices != m_pIndices)
		{
			sdelete_array(m_pIndices);
			m_pIndices = pIndices;
			m_MaxIndices = numIndices;
			m_NumIndices = numIndices;
			m_bDirty = true;
		}
	}	
}

