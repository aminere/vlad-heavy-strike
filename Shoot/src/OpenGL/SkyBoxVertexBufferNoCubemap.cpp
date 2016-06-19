/* 

Amine Rehioui
Created: April 19th 2012

*/

#include "Shoot.h"

#include "SkyBoxVertexBufferNoCubemap.h"

#include "FakeCubeMapTexture.h"

namespace shoot
{
	DEFINE_OBJECT(SkyBoxVertexBufferNoCubemap);

	//! constructor
	SkyBoxVertexBufferNoCubemap::SkyBoxVertexBufferNoCubemap(FakeCubeMapTexture* pTexture)
		: super()
		, m_pTexture(pTexture)
	{
	}

	//! begins rendering using this vertex buffer
	void SkyBoxVertexBufferNoCubemap::Begin()
	{
		VertexBuffer::Begin();
	}

	//! render using this vertex buffer
	void SkyBoxVertexBufferNoCubemap::Draw(const std::vector<Matrix44>& aWorldTransforms, const std::vector<Matrix44>& aTextureTransforms, Material* pMaterial)
	{
		GraphicsDriver::Instance()->SetColor(pMaterial->GetColor());
		GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_World, aWorldTransforms[0]);
		GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_Texture, Matrix44::Identity);
		for(u32 i=0; i<6; ++i)
		{
			m_pTexture->GetTexture(i)->Begin(0);
			GraphicsDriver::Instance()->DrawPrimitiveList(GraphicsDriver::PT_Triangle, m_Flags, 6, &m_pVertices[i*6]);
		}
		m_pTexture->GetTexture(5)->End();
	}

	//! ends rendering by this vertex buffer
	void SkyBoxVertexBufferNoCubemap::End()
	{
		VertexBuffer::End();
	}
}
