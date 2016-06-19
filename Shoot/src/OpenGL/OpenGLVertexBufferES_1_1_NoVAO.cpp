/* 

Amine Rehioui
Created: Marche 20th 2011

*/

#include "Shoot.h"

#include "OpenGLVertexBufferES_1_1_NoVAO.h"

#include "OpenGLExtensionHandler.h"

namespace shoot
{
	//! Load into video memory
	void OpenGLVertexBufferES_1_1_NoVAO::GraphicLoad()
	{
		SHOOT_ASSERT(GraphicExtensionHandler::Instance()->HasExtension(GraphicExtensionHandler::E_VBO), "VBO extension not supported");
		SHOOT_ASSERT(m_VBO_ID == 0, "VertexBuffer already loaded");

		OpenGLExtensionHandler::Instance()->extGlGenBuffers(1, &m_VBO_ID);

		if((m_IBO_ID == 0) && m_pIndices)
		{
			OpenGLExtensionHandler::Instance()->extGlGenBuffers(1, &m_IBO_ID);
			OpenGLExtensionHandler::Instance()->extGlBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO_ID);
			OpenGLExtensionHandler::Instance()->extGlBufferData(GL_ELEMENT_ARRAY_BUFFER, m_NumIndices*sizeof(u16), m_pIndices, GL_STATIC_DRAW);				
		}

		UpdateData();

		if(!m_bDynamic)
		{
			sdelete_array(m_pVertices);
			sdelete_array(m_pIndices);
		}
	}

	//! updates the vertex buffer data
	void OpenGLVertexBufferES_1_1_NoVAO::UpdateData()
	{
		if(m_pVertices)
		{
			GLenum usage = m_bDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
			OpenGLExtensionHandler::Instance()->extGlBindBuffer(GL_ARRAY_BUFFER, m_VBO_ID);
			OpenGLExtensionHandler::Instance()->extGlBufferData(GL_ARRAY_BUFFER, m_MaxVertices*sizeof(Vertex3D), m_pVertices, usage);
		}		
		m_bDirty = false;
	}

	//! begins rendering using this vertex buffer
	void OpenGLVertexBufferES_1_1_NoVAO::Begin()
	{
		if(m_VBO_ID == 0)
		{
			GraphicLoad();
		}

		VertexBuffer::Begin();

		OpenGLExtensionHandler::Instance()->extGlBindBuffer(GL_ARRAY_BUFFER, m_VBO_ID);
		
		const bool bPos = ((m_Flags & Vertex3D::VF_Pos) != 0);
		const bool bUV = ((m_Flags & Vertex3D::VF_UV) != 0);
		const bool bNormal = ((m_Flags & Vertex3D::VF_Normal) != 0);
		const bool bColor = ((m_Flags & Vertex3D::VF_Color) != 0);
		const bool bUVW = ((m_Flags & Vertex3D::VF_UVW) != 0);

		//struct Vertex3D
		//{
		//	Vector3 Pos;
		//	Vector2 UV;
		//	Vector3 Normal;
		//	Color color;
		//	Vector3 UVW;
		//};
		
		void* vertexOffset = 0;
		void* uvOffset = (u8*)vertexOffset+sizeof(Vector3);
		void* normalOffset = (u8*)uvOffset+sizeof(Vector2);
		void* colorOffset = (u8*)normalOffset+sizeof(Vector3);
		void* uvwOffset = (u8*)colorOffset+sizeof(Color);

		if(bPos)
		{
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(3, GL_FLOAT, sizeof(Vertex3D), vertexOffset);
		}
		else
		{
			glDisableClientState(GL_VERTEX_ARRAY);
		}

		if(bUV || bUVW)
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			if(bUV)
			{
				glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex3D), uvOffset);
			}
			else			
			{
				glTexCoordPointer(3, GL_FLOAT, sizeof(Vertex3D), uvwOffset);
			}
		}
		else
		{
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
		
		if(bNormal)
		{
			glEnableClientState(GL_NORMAL_ARRAY);
			glNormalPointer(GL_FLOAT, sizeof(Vertex3D), normalOffset);
		}
		else
		{
			glDisableClientState(GL_NORMAL_ARRAY);
		}

		if(bColor)
		{
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(4, GL_FLOAT, sizeof(Vertex3D), colorOffset);
		}
		else
		{
			glDisableClientState(GL_COLOR_ARRAY);
		}		

		if(m_IBO_ID)
		{
			OpenGLExtensionHandler::Instance()->extGlBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO_ID);
		}
	}

	//! ends rendering by this vertex buffer
	void OpenGLVertexBufferES_1_1_NoVAO::End()
	{
		VertexBuffer::End();

		OpenGLExtensionHandler::Instance()->extGlBindBuffer(GL_ARRAY_BUFFER, 0);

		if(m_IBO_ID)
		{
			OpenGLExtensionHandler::Instance()->extGlBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}		
	}

	//! create a copy of the object
	Object* OpenGLVertexBufferES_1_1_NoVAO::Copy() const
	{
		OpenGLVertexBufferES_1_1_NoVAO* pCopy = snew OpenGLVertexBufferES_1_1_NoVAO();
		u32 ID = pCopy->GetID();

		*pCopy = *this;

		// make sure copy properties are initialized properly
		pCopy->SetID(ID);
		pCopy->m_pVertices = NULL;
		pCopy->m_pIndices = NULL;
		pCopy->m_VAO_ID = 0;
		pCopy->m_VBO_ID = 0;
		pCopy->m_IBO_ID = 0;
		return pCopy;
	}
}

