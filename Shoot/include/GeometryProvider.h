/* 

Amine Rehioui
Created: March 30th 2012

*/

#ifndef _GEOMETRY_PROVIDER_H_INCLUDED_
#define _GEOMETRY_PROVIDER_H_INCLUDED_

#include "VertexBuffer.h"
#include "Text.h"

namespace shoot
{
	// forwards
	class Font;
	class Texture;

	//! Geometry provider
	class GeometryProvider : public Singleton<GeometryProvider>
	{
		DECLARE_SINGLETON(GeometryProvider);

	public:

		//! returns a 2D quad vertex buffer
		VertexBuffer* Get2DQuad(const Vector2& vSize, const Vector2& vUV);

		//! returns a 3D quad vertex buffer
		VertexBuffer* Get3DQuad(const Vector2& vScale);

		//! creates a text vertex buffer
		VertexBuffer* CreateTextVertexBuffer(Font* pFont, const std::string& text, const Size& size, Text::E_Alignement alignement, u32 lineSpacing, const Color& color);

		//! updates a text vertex buffer
		void UpdateTextVertexBuffer(VertexBuffer* pVertexBuffer, Font* pFont, const std::string& text, const Size& size, Text::E_Alignement alignement, u32 lineSpacing, const Color& color);

		//! gets a sky box vertex buffer
		VertexBuffer* GetSkyBoxVertexBuffer(Texture* pTexture);

	private:

		//! Creates a 2D quad vertex buffer
		VertexBuffer* Create2DQuad(const Vector2& vSize, const Vector2& vUV);

		//! Creates a 3D quad vertex buffer
		VertexBuffer* Create3DQuad(const Vector2& vScale);

		//! creates a sky box vertex buffer
		VertexBuffer* CreateSkyBoxVertexBuffer();

		//! creates a sky box vertex buffer when no cube map support
		VertexBuffer* CreateSkyBoxVertexBufferNoCubemap(Texture* pTexture);

		//! adds a characters to a text vertex buffer
		void AddCharacter(VertexBuffer* pVertexBuffer, Font* pFont, char c, const Vector2& vCharacterPos, u32 currentCharacter, const Color& color);

		// geometry pools
		std::list< Handle<VertexBuffer> > m_l3DQuads;
		std::list< Handle<VertexBuffer> > m_l2DQuads;

		//! line info
		struct LineInfo
		{
			LineInfo()
				: numCharacters(0)
			{
			}
			u32 numCharacters;
			std::vector<std::string> words;
		};
	};
}

#endif // _GEOMETRY_PROVIDER_H_INCLUDED_


