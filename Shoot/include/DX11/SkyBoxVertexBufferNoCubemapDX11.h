/* 

Amine Rehioui
Created: August 1st 2013

*/

#ifndef _SKYBOX_VERTEX_BUFFER_NO_CUBEMAP_DX11_H_INCLUDED_
#define _SKYBOX_VERTEX_BUFFER_NO_CUBEMAP_DX11_H_INCLUDED_

#include "VertexBufferDX11.h"

namespace shoot
{
	// forwards
	class FakeCubeMapTexture;
		
	//! SkyBoxVertexBufferNoCubemapDX11
	class SkyBoxVertexBufferNoCubemapDX11 : public VertexBufferDX11
	{
		DECLARE_OBJECT(SkyBoxVertexBufferNoCubemapDX11, VertexBufferDX11);

	public:

		//! constructor
		SkyBoxVertexBufferNoCubemapDX11()
		{
		}

		//! constructor
		SkyBoxVertexBufferNoCubemapDX11(FakeCubeMapTexture* pTexture);

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

#endif // _SKYBOX_VERTEX_BUFFER_NO_CUBEMAP_DX11_H_INCLUDED_


