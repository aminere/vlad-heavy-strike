/* 

Amine Rehioui
Created: April 19th 2012

*/

#ifndef _SKYBOX_VERTEX_BUFFER_NO_CUBEMAP_NO_VAO_H_INCLUDED_
#define _SKYBOX_VERTEX_BUFFER_NO_CUBEMAP_NO_VAO_H_INCLUDED_

#include "OpenGLVertexBufferES_1_1_NoVAO.h"

namespace shoot
{
	// forwards
	class FakeCubeMapTexture;
		
	class SkyBoxVertexBufferNoCubemap_NoVAO : public OpenGLVertexBufferES_1_1_NoVAO
	{
		DECLARE_OBJECT(SkyBoxVertexBufferNoCubemap_NoVAO, OpenGLVertexBufferES_1_1_NoVAO);

	public:

		//! constructor
		SkyBoxVertexBufferNoCubemap_NoVAO()
		{
		}

		//! constructor
		SkyBoxVertexBufferNoCubemap_NoVAO(FakeCubeMapTexture* pTexture);

		//! begins rendering using this vertex buffer
		void Begin();

		//! render using this vertex buffer
		void Draw(const std::vector<Matrix44>& aWorldTransforms, const std::vector<Matrix44>& aTextureTransforms, Material* pMaterial);

		//! ends rendering by this vertex buffer
		void End();

	private:

		FakeCubeMapTexture* m_pTexture;
	};
}

#endif // _SKYBOX_VERTEX_BUFFER_NO_CUBEMAP_NO_VAO_H_INCLUDED_


