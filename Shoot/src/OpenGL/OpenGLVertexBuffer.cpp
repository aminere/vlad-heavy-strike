/* 

Amine Rehioui
Created: Marche 20th 2011

*/

#include "Shoot.h"

#include "OpenGLVertexBuffer.h"
#include "OpenGLShader.h"

#include "OpenGLExtensionHandler.h"

namespace shoot
{
	//! constructor
	OpenGLVertexBuffer::OpenGLVertexBuffer()
		: m_VAO_ID(0)
		, m_VBO_ID(0)
		, m_IBO_ID(0)
	{
	}

	//! destructor
	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		GraphicUnload();
	}

	//! Load into video memory
	void OpenGLVertexBuffer::GraphicLoad()
	{
		SHOOT_ASSERT(m_VAO_ID == 0, "VertexBuffer already loaded");
		SHOOT_ASSERT(m_VBO_ID == 0, "OpenGLVertexBuffer error");
		SHOOT_ASSERT(m_IBO_ID == 0, "OpenGLVertexBuffer error");

		OpenGLExtensionHandler::Instance()->extGlGenVertexArrays(1, &m_VAO_ID);
		OpenGLExtensionHandler::Instance()->extGlGenBuffers(1, &m_VBO_ID);
		if(m_pIndices)
		{
			OpenGLExtensionHandler::Instance()->extGlGenBuffers(1, &m_IBO_ID);
		}

		OpenGLExtensionHandler::Instance()->extGlBindVertexArray(m_VAO_ID);
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
			GL_CHECK(glEnableClientState(GL_VERTEX_ARRAY));
			GL_CHECK(glVertexPointer(3, GL_FLOAT, sizeof(Vertex3D), vertexOffset));
		}
		else
		{
			GL_CHECK(glDisableClientState(GL_VERTEX_ARRAY));
		}

		if(bUV || bUVW)
		{
			GL_CHECK(glEnableClientState(GL_TEXTURE_COORD_ARRAY));

			if(bUV)
			{
				GL_CHECK(glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex3D), uvOffset));
			}
			else
			{
				GL_CHECK(glTexCoordPointer(3, GL_FLOAT, sizeof(Vertex3D), uvwOffset));
			}
		}
		else
		{
			GL_CHECK(glDisableClientState(GL_TEXTURE_COORD_ARRAY));
		}

		if(bNormal)
		{
			GL_CHECK(glEnableClientState(GL_NORMAL_ARRAY));
			GL_CHECK(glNormalPointer(GL_FLOAT, sizeof(Vertex3D), normalOffset));
		}
		else
		{
			GL_CHECK(glDisableClientState(GL_NORMAL_ARRAY));
		}

		if(bColor)
		{
			GL_CHECK(glEnableClientState(GL_COLOR_ARRAY));
			GL_CHECK(glColorPointer(4, GL_FLOAT, sizeof(Vertex3D), colorOffset));
		}
		else
		{
			GL_CHECK(glDisableClientState(GL_COLOR_ARRAY));
		}

		if(m_pVertices)
		{
			GLenum usage = m_bDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
			OpenGLExtensionHandler::Instance()->extGlBufferData(GL_ARRAY_BUFFER, m_MaxVertices*sizeof(Vertex3D), m_pVertices, usage);
			m_bDirty = false;
		}			

		if(m_IBO_ID != 0)
		{
			OpenGLExtensionHandler::Instance()->extGlBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO_ID);
			OpenGLExtensionHandler::Instance()->extGlBufferData(GL_ELEMENT_ARRAY_BUFFER, m_NumIndices*sizeof(u16), m_pIndices, GL_STATIC_DRAW);
		}
		else
		{
			OpenGLExtensionHandler::Instance()->extGlBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		// reserve vertex attributes 0 to 3 for a world matrix to be used for instanced rendering
		// TODO check if ARB_instanced_array extension is available
		//OpenGLExtensionHandler::Instance()->extGlGenBuffers(1, &m_WorldMatrixBufferID);
		//OpenGLExtensionHandler::Instance()->extGlBindBuffer(GL_ARRAY_BUFFER, m_WorldMatrixBufferID);
		//for(u32 i=0; i<4; ++i)
		//{
		//	OpenGLExtensionHandler::Instance()->extGlEnableVertexAttribArray(i);
		//	OpenGLExtensionHandler::Instance()->extGlVertexAttribPointer(i, 4, GL_FLOAT, false, sizeof(Matrix44), (const void*)(sizeof(GL_FLOAT) * i * 4));
		//	OpenGLExtensionHandler::Instance()->extGlVertexAttribDivisor(i, 1);
		//}

		if(!m_bDynamic)
		{
#ifndef SHOOT_EDITOR // keep the vertices when in the editor, needed for triangle picking
			sdelete_array(m_pVertices);
			sdelete_array(m_pIndices);
#endif // SHOOT_EDITOR
		}

		OpenGLExtensionHandler::Instance()->extGlBindVertexArray(0);
		OpenGLExtensionHandler::Instance()->extGlBindBuffer(GL_ARRAY_BUFFER, 0);
		OpenGLExtensionHandler::Instance()->extGlBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		bPos ? glDisableClientState(GL_VERTEX_ARRAY) : void();
		(bUV || bUVW) ? glDisableClientState(GL_TEXTURE_COORD_ARRAY) : void();
		bNormal ? glDisableClientState(GL_NORMAL_ARRAY) : void();
		bColor ? glDisableClientState(GL_COLOR_ARRAY) : void();
	}

	//! updates the vertex buffer data
	void OpenGLVertexBuffer::UpdateData()
	{
		if(m_pVertices)
		{
			OpenGLExtensionHandler::Instance()->extGlBindBuffer(GL_ARRAY_BUFFER, m_VBO_ID);
			// Crashes in the editor if reloading a mesh with a bigger vertex buffer
			// TODO: make this game only?
			//if(void* pData = OpenGLExtensionHandler::Instance()->extGlMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY))
			//{
			//	memcpy(pData, m_pVertices, m_MaxVertices*sizeof(Vertex3D));
			//	OpenGLExtensionHandler::Instance()->extGlUnmapBuffer(GL_ARRAY_BUFFER);
			//}
			//else
			{
				// create the data store
				GLenum usage = m_bDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
				OpenGLExtensionHandler::Instance()->extGlBufferData(GL_ARRAY_BUFFER, m_MaxVertices*sizeof(Vertex3D), m_pVertices, usage);
			}
			OpenGLExtensionHandler::Instance()->extGlBindBuffer(GL_ARRAY_BUFFER, 0);

			if(m_IBO_ID != 0)
			{
				OpenGLExtensionHandler::Instance()->extGlBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO_ID);
				OpenGLExtensionHandler::Instance()->extGlBufferData(GL_ELEMENT_ARRAY_BUFFER, m_NumIndices*sizeof(u16), m_pIndices, GL_STATIC_DRAW);
				OpenGLExtensionHandler::Instance()->extGlBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			}
		}
		m_bDirty = false;
	}

	//! begins rendering using this vertex buffer
	void OpenGLVertexBuffer::Begin()
	{
		if(m_VAO_ID == 0)
		{
			GraphicLoad();
		}

		VertexBuffer::Begin();

		OpenGLExtensionHandler::Instance()->extGlBindVertexArray(m_VAO_ID);
	}

	//! default render method
	void OpenGLVertexBuffer::Draw()
	{
		static s32 glPrimitives[] = { GL_POINTS,
									  GL_LINES,
									  GL_LINE_LOOP,
									  GL_LINE_STRIP,
									  GL_TRIANGLES,
									  GL_TRIANGLE_STRIP,
									  GL_TRIANGLE_FAN };

		if(m_IBO_ID)
		{
			GL_CHECK(glDrawElements(glPrimitives[m_ePrimitiveType], m_NumIndices, GL_UNSIGNED_SHORT, 0));
		}
		else
		{
			GL_CHECK(glDrawArrays(glPrimitives[m_ePrimitiveType], 0, m_NumVertices));
		}
	}

	//! instanced render method
	void OpenGLVertexBuffer::Draw(const std::vector<Matrix44>& aWorldTransforms, const std::vector<Matrix44>& aTextureTransforms, Material* pMaterial)
	{		
		static s32 glPrimitives[] = { GL_POINTS,
									  GL_LINES,
									  GL_LINE_LOOP,
									  GL_LINE_STRIP,
									  GL_TRIANGLES,
									  GL_TRIANGLE_STRIP,
									  GL_TRIANGLE_FAN };

		const Shader* pShader = pMaterial->GetShader();
		if(pShader)
		{
			GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_World, Matrix44::Identity);			
		}

		const bool bVertexColor = (m_Flags & Vertex3D::VF_Color) != 0;
		GraphicsDriver::Instance()->SetColor(bVertexColor ? Color::White : pMaterial->GetColor());

		if(m_bApplyWorldTransforms)
		{
			for(u32 i=0; i<aWorldTransforms.size(); ++i)
			{
				if(const OpenGLShader* pGLShader = (pShader ? static_cast<const OpenGLShader*>(pShader->GetImpl()) : NULL))
				{
					OpenGLExtensionHandler::Instance()->extGlUniformMatrix4fv(pGLShader->GetWorldMatrixLocation(), 1, false, aWorldTransforms[i].GetFloats());
				}
				else
				{
					GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_World, aWorldTransforms[i]);
				}

				GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_Texture, aTextureTransforms[i]);

				if(m_IBO_ID)
				{
					GL_CHECK(glDrawElements(glPrimitives[m_ePrimitiveType], m_NumIndices, GL_UNSIGNED_SHORT, 0));
				}
				else
				{
					GL_CHECK(glDrawArrays(glPrimitives[m_ePrimitiveType], 0, m_NumVertices));
				}

				EntityRenderer::Instance()->GetStats().Increment(this);
			}
		}
		else
		{
			GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_World, Matrix44::Identity);
			GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_Texture, Matrix44::Identity);

			if(m_IBO_ID)
			{
				GL_CHECK(glDrawElements(glPrimitives[m_ePrimitiveType], m_NumIndices, GL_UNSIGNED_SHORT, 0));
			}
			else
			{
				GL_CHECK(glDrawArrays(glPrimitives[m_ePrimitiveType], 0, m_NumVertices));
			}

			EntityRenderer::Instance()->GetStats().Increment(this);
		}
	}

	//! ends rendering by this vertex buffer
	void OpenGLVertexBuffer::End()
	{
		VertexBuffer::End();

		OpenGLExtensionHandler::Instance()->extGlBindVertexArray(0);
	}

	//! Unload from video memory
	void OpenGLVertexBuffer::GraphicUnload()
	{
		if(m_VAO_ID)
		{
			OpenGLExtensionHandler::Instance()->extGlDeleteVertexArrays(1, &m_VAO_ID);
			m_VAO_ID = 0;
		}

		if(m_VBO_ID)
		{
			OpenGLExtensionHandler::Instance()->extGlDeleteBuffers(1, &m_VBO_ID);
			m_VBO_ID = 0;
		}

		if(m_IBO_ID)
		{
			OpenGLExtensionHandler::Instance()->extGlDeleteBuffers(1, &m_IBO_ID);
			m_IBO_ID = 0;
		}
	}

	//! create a copy of the object
	Object* OpenGLVertexBuffer::Copy() const
	{
		OpenGLVertexBuffer* pCopy = snew OpenGLVertexBuffer();
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

