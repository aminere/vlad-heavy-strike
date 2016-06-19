/* 

Amine Rehioui
Created: Marche 20th 2011

*/

#include "Shoot.h"

#include "OpenGLVertexBufferES_1_1.h"

namespace shoot
{
	//! instanced render method
	void OpenGLVertexBufferES_1_1::Draw(const std::vector<Matrix44>& aWorldTransforms, const std::vector<Matrix44>& aTextureTransforms, Material* pMaterial)
	{
		static s32 glPrimitives[] = { GL_POINTS,
									  GL_LINES,
									  GL_LINE_LOOP,
									  GL_LINE_STRIP,
									  GL_TRIANGLES,
									  GL_TRIANGLE_STRIP,
									  GL_TRIANGLE_FAN };

		const bool bVertexColor = (m_Flags & Vertex3D::VF_Color) != 0;
		GraphicsDriver::Instance()->SetColor(bVertexColor ? Color::White : pMaterial->GetColor());

		if(m_bApplyWorldTransforms)
		{
			for(u32 i=0; i<aWorldTransforms.size(); ++i)
			{
				GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_World, aWorldTransforms[i]);

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

	//! create a copy of the object
	Object* OpenGLVertexBufferES_1_1::Copy() const
	{
		OpenGLVertexBufferES_1_1* pCopy = snew OpenGLVertexBufferES_1_1();
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

