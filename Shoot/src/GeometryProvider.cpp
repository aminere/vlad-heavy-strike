/* 

Amine Rehioui
Created: March 30th 2012

*/

#include "Shoot.h"

#include "GeometryProvider.h"

#include "Font.h"

#include "FakeCubeMapTexture.h"

#include "GraphicExtensionHandler.h"

#include "Utils.h"

namespace shoot
{
	//! constructor
	GeometryProvider::GeometryProvider()
	{
	}

	//! destructor
	GeometryProvider::~GeometryProvider()
	{
	}

	//! creates a text vertex buffer
	VertexBuffer* GeometryProvider::CreateTextVertexBuffer(Font* pFont, const std::string& text, const Size& size, Text::E_Alignement alignement, u32 lineSpacing, const Color& color)
	{
		if(!text.empty())
		{
			VertexBuffer* pVertexBuffer = GraphicsDriver::Instance()->CreateVertexBuffer();
			pVertexBuffer->SetVertexFlag(Vertex3D::VF_Pos);
			pVertexBuffer->SetVertexFlag(Vertex3D::VF_UV);
			pVertexBuffer->SetVertexFlag(Vertex3D::VF_Color);
			pVertexBuffer->SetName(text);

			UpdateTextVertexBuffer(pVertexBuffer, pFont, text, size, alignement, lineSpacing, color);

			pVertexBuffer->SetDynamic(true);
			return pVertexBuffer;
		}
		return NULL;
	}	

	//! returns a 2D quad vertex buffer
	VertexBuffer* GeometryProvider::Get2DQuad(const Vector2& vSize, const Vector2& vUV)
	{
		for(std::list< Handle<VertexBuffer> >::iterator it = m_l2DQuads.begin(); it != m_l2DQuads.end(); )
		{
			if((*it).IsValid())
			{
				VertexBuffer* pVertexBuffer = (*it);
				Vector2 _vSize(pVertexBuffer->GetVertices()[2].Pos.X, pVertexBuffer->GetVertices()[2].Pos.Y);
				if(_vSize == vSize)
				{
					return pVertexBuffer;
				}
				++it;
			}
			else
			{
				it = m_l2DQuads.erase(it);
			}
		}
		VertexBuffer* pVertexBuffer = Create2DQuad(vSize, vUV);		
		m_l2DQuads.push_back(Handle<VertexBuffer>(pVertexBuffer));
		return pVertexBuffer;
	}

	//! Creates a 2D quad vertex buffer
	VertexBuffer* GeometryProvider::Create2DQuad(const Vector2& vSize, const Vector2& vUV)
	{
		VertexBuffer* pVertexBuffer = GraphicsDriver::Instance()->CreateVertexBuffer();
		pVertexBuffer->SetVertexFlag(Vertex3D::VF_Pos);
		pVertexBuffer->SetVertexFlag(Vertex3D::VF_UV);
		pVertexBuffer->SetVertexFlag(Vertex3D::VF_2DQuad);
		pVertexBuffer->SetName("2DQuad");

		Vertex3D* pVertices = snew Vertex3D[6];
		pVertices[0].UV = Vector2(0.0f, 0.0f); pVertices[0].Pos = Vector3::Zero; // Top Left
		pVertices[1].UV = Vector2(vUV.X, 0.0f); pVertices[1].Pos = Vector3::Create(vSize.X, 0.0f, 0.0f); // Top Right
		pVertices[2].UV = Vector2(vUV.X, vUV.Y); pVertices[2].Pos = Vector3::Create(vSize.X, vSize.Y, 0.0f); // Bottom Right
		pVertices[3].UV = Vector2(0.0f, vUV.Y); pVertices[3].Pos = Vector3::Create(0.0f, vSize.Y, 0.0f); // Bottom Left
		pVertices[4].UV = Vector2(0.0f, 0.0f); pVertices[4].Pos = Vector3::Zero; // Top Left
		pVertices[5].UV = Vector2(vUV.X, vUV.Y); pVertices[5].Pos = Vector3::Create(vSize.X, vSize.Y, 0.0f); // Bottom Right

		pVertexBuffer->SetVertices(pVertices, 6);
		pVertexBuffer->SetDynamic(true);
		return pVertexBuffer;
	}

	//! returns a 3D quad vertex buffer
	VertexBuffer* GeometryProvider::Get3DQuad(const Vector2& vScale)
	{
		for(std::list< Handle<VertexBuffer> >::iterator it = m_l3DQuads.begin(); it != m_l3DQuads.end(); )
		{
			if((*it).IsValid())
			{
				VertexBuffer* pVertexBuffer = (*it);
				Vector2 _vScale(pVertexBuffer->GetVertices()[2].Pos.X, pVertexBuffer->GetVertices()[2].Pos.Y);
				if(_vScale == vScale)
				{
					return pVertexBuffer;
				}
				++it;
			}
			else
			{
				it = m_l3DQuads.erase(it);
			}
		}
		VertexBuffer* pVertexBuffer = Create3DQuad(vScale);		
		m_l3DQuads.push_back(Handle<VertexBuffer>(pVertexBuffer));
		return pVertexBuffer;
	}

	//! Creates a 3D quad vertex buffer
	VertexBuffer* GeometryProvider::Create3DQuad(const Vector2& vScale)
	{
		VertexBuffer* pVertexBuffer = GraphicsDriver::Instance()->CreateVertexBuffer();
		pVertexBuffer->SetVertexFlag(Vertex3D::VF_Pos);
		pVertexBuffer->SetVertexFlag(Vertex3D::VF_UV);
		pVertexBuffer->SetVertexFlag(Vertex3D::VF_3DQuad);
		pVertexBuffer->SetName("3DQuad");

		Vertex3D* pVertices = snew Vertex3D[6];
		pVertices[0].UV = Vector2(0.0f, 1.0f); pVertices[0].Pos = Vector3::Create(-vScale.X, -vScale.Y, 0.0f);
		pVertices[1].UV = Vector2(0.0f, 0.0f); pVertices[1].Pos = Vector3::Create(-vScale.X, vScale.Y, 0.0f);
		pVertices[2].UV = Vector2(1.0f, 0.0f); pVertices[2].Pos = Vector3::Create(vScale.X, vScale.Y, 0.0f);
		pVertices[3].UV = Vector2(0.0f, 1.0f); pVertices[3].Pos = Vector3::Create(-vScale.X, -vScale.Y, 0.0f);
		pVertices[4].UV = Vector2(1.0f, 0.0f); pVertices[4].Pos = Vector3::Create(vScale.X, vScale.Y, 0.0f);
		pVertices[5].UV = Vector2(1.0f, 1.0f); pVertices[5].Pos = Vector3::Create(vScale.X, -vScale.Y, 0.0f);

		pVertexBuffer->SetVertices(pVertices, 6);
		pVertexBuffer->SetDynamic(true);
		return pVertexBuffer;
	}

	//! updates a text vertex buffer
	void GeometryProvider::UpdateTextVertexBuffer(VertexBuffer* pVertexBuffer, Font* pFont, const std::string& text, const Size& size, Text::E_Alignement alignement, u32 lineSpacing, const Color& color)
	{
		s32 maxCharactersPerLine = size.Width/s32(pFont->GetCharacterSize().X);

		std::vector<std::string> words;
		Utils::Tokenize(text.c_str(), " ", words);

		std::vector< LineInfo > lines;
		u32 currentLine = 0;
		s32 charactersRemainingThisLine = maxCharactersPerLine;
		u32 totalCharacters = 0;
		for(u32 currentWord=0; currentWord < words.size(); ++currentWord)
		{
			if(currentLine == lines.size())
			{
				lines.push_back(LineInfo());
			}

			if(s32(words[currentWord].length()) < charactersRemainingThisLine
			|| lines[currentLine].words.empty())
			{
				charactersRemainingThisLine -= words[currentWord].length();
				--charactersRemainingThisLine; // consume one space
				if(!lines[currentLine].words.empty())
				{
					lines[currentLine].numCharacters++; // consume one space
					++totalCharacters;
				}
				u32 wordLength = words[currentWord].length();
				std::string word = words[currentWord].substr(0, Math::Min(wordLength, u32(maxCharactersPerLine)));
				lines[currentLine].words.push_back(word);
				lines[currentLine].numCharacters += word.length();
				totalCharacters += word.length();
			}
			else
			{
				charactersRemainingThisLine = maxCharactersPerLine;
				++currentLine;
				--currentWord;
			}
		}

		u32 numVertices = totalCharacters*6;
		if(numVertices > pVertexBuffer->GetMaxVertices())
		{
			Vertex3D* pVertices = snew Vertex3D[numVertices];
			pVertexBuffer->SetVertices(pVertices, numVertices);
		}
		pVertexBuffer->SetNumVertices(0);
		pVertexBuffer->SetMaxVertices(numVertices);

		u32 currentCharacter = 0;
		for(u32 i=0; i<lines.size(); ++i)
		{
			Vector2 vCharacterPos(0.0f, f32(i*lineSpacing));

			switch(alignement)
			{
			case Text::A_Center:
				vCharacterPos.X = (f32(size.Width)-(pFont->GetCharacterSize().X*lines[i].numCharacters))/2.0f;
				break;

			case Text::A_Right:
				vCharacterPos.X = f32(size.Width)-(pFont->GetCharacterSize().X*lines[i].numCharacters);
				break;
			}

			for(u32 j=0; j<lines[i].words.size(); ++j)
			{
				for(u32 k=0; k<lines[i].words[j].length(); ++k)
				{
					AddCharacter(pVertexBuffer, pFont, lines[i].words[j].at(k), vCharacterPos, currentCharacter++, color);
					vCharacterPos.X += pFont->GetCharacterSize().X;
				}

				if(j+1 < lines[i].words.size())
				{
					AddCharacter(pVertexBuffer, pFont, ' ', vCharacterPos, currentCharacter++, color);
					vCharacterPos.X += pFont->GetCharacterSize().X;
				}				
			}
		}
	}

	//! gets a sky box vertex buffer
	VertexBuffer* GeometryProvider::GetSkyBoxVertexBuffer(Texture* pTexture)
	{
		if(GraphicExtensionHandler::Instance()->HasExtension(GraphicExtensionHandler::E_CubeMap))
		{
			return CreateSkyBoxVertexBuffer();
		}
		else
		{
			return CreateSkyBoxVertexBufferNoCubemap(pTexture);
		}
	}

	//! creates a sky box vertex buffer
	VertexBuffer* GeometryProvider::CreateSkyBoxVertexBuffer()
	{
		VertexBuffer* pVertexBuffer = GraphicsDriver::Instance()->CreateVertexBuffer();
		pVertexBuffer->SetVertexFlag(Vertex3D::VF_Pos);
		pVertexBuffer->SetVertexFlag(Vertex3D::VF_UVW);
		pVertexBuffer->SetName("SkyBox");

		Vertex3D* pVertices = snew Vertex3D[36];
		f32 t = 1.0f;

		// -X
		pVertices[6*0+0].UVW = Vector3::Create(-t, -t, -t); pVertices[6*0+0].Pos = Vector3::Create(-t, -t, t);
		pVertices[6*0+1].UVW = Vector3::Create(-t, t, -t); pVertices[6*0+1].Pos = Vector3::Create(-t, t, t);
		pVertices[6*0+2].UVW = Vector3::Create(-t, t, t); pVertices[6*0+2].Pos = Vector3::Create(-t, t, -t);
		pVertices[6*0+3].UVW = Vector3::Create(-t, -t, -t); pVertices[6*0+3].Pos = Vector3::Create(-t, -t, t);
		pVertices[6*0+4].UVW = Vector3::Create(-t, t, t); pVertices[6*0+4].Pos = Vector3::Create(-t, t, -t);
		pVertices[6*0+5].UVW = Vector3::Create(-t, -t, t); pVertices[6*0+5].Pos = Vector3::Create(-t, -t, -t);

		// +X
		pVertices[6*1+0].UVW = Vector3::Create(t, -t, t); pVertices[6*1+0].Pos = Vector3::Create(t, -t, -t);
		pVertices[6*1+1].UVW = Vector3::Create(t, t, t); pVertices[6*1+1].Pos = Vector3::Create(t, t, -t);
		pVertices[6*1+2].UVW = Vector3::Create(t, t, -t); pVertices[6*1+2].Pos = Vector3::Create(t, t, t);
		pVertices[6*1+3].UVW = Vector3::Create(t, -t, t); pVertices[6*1+3].Pos = Vector3::Create(t, -t, -t);
		pVertices[6*1+4].UVW = Vector3::Create(t, t, -t); pVertices[6*1+4].Pos = Vector3::Create(t, t, t);
		pVertices[6*1+5].UVW = Vector3::Create(t, -t, -t); pVertices[6*1+5].Pos = Vector3::Create(t, -t, t);

		// -Y
		pVertices[6*2+0].UVW = Vector3::Create(-t, -t, -t); pVertices[6*2+0].Pos = Vector3::Create(-t, -t, t);
		pVertices[6*2+1].UVW = Vector3::Create(-t, -t, t); pVertices[6*2+1].Pos = Vector3::Create(-t, -t, -t);
		pVertices[6*2+2].UVW = Vector3::Create(t, -t, t); pVertices[6*2+2].Pos = Vector3::Create(t, -t, -t);
		pVertices[6*2+3].UVW = Vector3::Create(-t, -t, -t); pVertices[6*2+3].Pos = Vector3::Create(-t, -t, t);
		pVertices[6*2+4].UVW = Vector3::Create(t, -t, t); pVertices[6*2+4].Pos = Vector3::Create(t, -t, -t);
		pVertices[6*2+5].UVW = Vector3::Create(t, -t, -t); pVertices[6*2+5].Pos = Vector3::Create(t, -t, t);

		// +Y
		pVertices[6*3+0].UVW = Vector3::Create(-t, t, t); pVertices[6*3+0].Pos = Vector3::Create(-t, t, -t);
		pVertices[6*3+1].UVW = Vector3::Create(-t, t, -t); pVertices[6*3+1].Pos = Vector3::Create(-t, t, t);
		pVertices[6*3+2].UVW = Vector3::Create(t, t, -t); pVertices[6*3+2].Pos = Vector3::Create(t, t, t);
		pVertices[6*3+3].UVW = Vector3::Create(-t, t, t); pVertices[6*3+3].Pos = Vector3::Create(-t, t, -t);
		pVertices[6*3+4].UVW = Vector3::Create(t, t, -t); pVertices[6*3+4].Pos = Vector3::Create(t, t, t);
		pVertices[6*3+5].UVW = Vector3::Create(t, t, t); pVertices[6*3+5].Pos = Vector3::Create(t, t, -t);

		// -Z
		pVertices[6*4+0].UVW = Vector3::Create(t, -t, -t); pVertices[6*4+0].Pos = Vector3::Create(-t, -t, -t);
		pVertices[6*4+1].UVW = Vector3::Create(t, t, -t); pVertices[6*4+1].Pos = Vector3::Create(-t, t, -t);
		pVertices[6*4+2].UVW = Vector3::Create(-t, t, -t); pVertices[6*4+2].Pos = Vector3::Create(t, t, -t);
		pVertices[6*4+3].UVW = Vector3::Create(t, -t, -t); pVertices[6*4+3].Pos = Vector3::Create(-t, -t, -t);
		pVertices[6*4+4].UVW = Vector3::Create(-t, t, -t); pVertices[6*4+4].Pos = Vector3::Create(t, t, -t);
		pVertices[6*4+5].UVW = Vector3::Create(-t, -t, -t); pVertices[6*4+5].Pos = Vector3::Create(t, -t, -t);

		// +Z
		pVertices[6*5+0].UVW = Vector3::Create(-t, -t, t); pVertices[6*5+0].Pos = Vector3::Create(t, -t, t);
		pVertices[6*5+1].UVW = Vector3::Create(-t, t, t); pVertices[6*5+1].Pos = Vector3::Create(t, t, t);
		pVertices[6*5+2].UVW = Vector3::Create(t, t, t); pVertices[6*5+2].Pos = Vector3::Create(-t, t, t);
		pVertices[6*5+3].UVW = Vector3::Create(-t, -t, t); pVertices[6*5+3].Pos = Vector3::Create(t, -t, t);
		pVertices[6*5+4].UVW = Vector3::Create(t, t, t); pVertices[6*5+4].Pos = Vector3::Create(-t, t, t);
		pVertices[6*5+5].UVW = Vector3::Create(t, -t, t); pVertices[6*5+5].Pos = Vector3::Create(-t, -t, t);

		pVertexBuffer->SetVertices(pVertices, 36);
		pVertexBuffer->SetRenderStateSetter(snew SkyBoxStateSetter());
		pVertexBuffer->SetDynamic(true);
		return pVertexBuffer;
	}

	//! creates a sky box vertex buffer when no cube map support
	VertexBuffer* GeometryProvider::CreateSkyBoxVertexBufferNoCubemap(Texture* pTexture)
	{
		VertexBuffer* pVertexBuffer = GraphicsDriver::Instance()->CreateSkyBoxVertexBufferNoCubemap(pTexture);
		pVertexBuffer->SetVertexFlag(Vertex3D::VF_Pos);
		pVertexBuffer->SetVertexFlag(Vertex3D::VF_UV);

		Vertex3D* pVertices = snew Vertex3D[36];
		f32 s = 1.0f;
		// matches texels with pixels - uses same method as the Irrlicht engine
		f32 onePixel = pTexture ? (1.0f / (pTexture->GetSize().X * 1.5f)) : 0.0f;
		f32 o = 0.0f + onePixel;
		f32 t = 1.0f - onePixel;

		// -X
		pVertices[6*0+0].Pos = Vector3::Create(-s, -s, -s);	pVertices[6*0+0].UV = Vector2(o, t);
		pVertices[6*0+1].Pos = Vector3::Create(-s, -s, s);		pVertices[6*0+1].UV = Vector2(o, o);
		pVertices[6*0+2].Pos = Vector3::Create(-s, s, s);		pVertices[6*0+2].UV = Vector2(t, o);
		pVertices[6*0+3].Pos = Vector3::Create(-s, -s, -s);	pVertices[6*0+3].UV = Vector2(o, t);
		pVertices[6*0+4].Pos = Vector3::Create(-s, s, s);		pVertices[6*0+4].UV = Vector2(t, o);
		pVertices[6*0+5].Pos = Vector3::Create(-s, s, -s);		pVertices[6*0+5].UV = Vector2(t, t);

		// +X
		pVertices[6*1+0].Pos = Vector3::Create(s, s, -s);		pVertices[6*1+0].UV = Vector2(o, t);
		pVertices[6*1+1].Pos = Vector3::Create(s, s, s);		pVertices[6*1+1].UV = Vector2(o, o);
		pVertices[6*1+2].Pos = Vector3::Create(s, -s, s);		pVertices[6*1+2].UV = Vector2(t, o);
		pVertices[6*1+3].Pos = Vector3::Create(s, s, -s);		pVertices[6*1+3].UV = Vector2(o, t);
		pVertices[6*1+4].Pos = Vector3::Create(s, -s, s);		pVertices[6*1+4].UV = Vector2(t, o);
		pVertices[6*1+5].Pos = Vector3::Create(s, -s, -s);		pVertices[6*1+5].UV = Vector2(t, t);

		// -Z
		pVertices[6*2+0].Pos = Vector3::Create(-s, s, -s);		pVertices[6*2+0].UV = Vector2(o, o);
		pVertices[6*2+1].Pos = Vector3::Create(s, s, -s);		pVertices[6*2+1].UV = Vector2(t, o);
		pVertices[6*2+2].Pos = Vector3::Create(-s, -s, -s);	pVertices[6*2+2].UV = Vector2(o, t);
		pVertices[6*2+3].Pos = Vector3::Create(-s, -s, -s);	pVertices[6*2+3].UV = Vector2(o, t);
		pVertices[6*2+4].Pos = Vector3::Create(s, s, -s);		pVertices[6*2+4].UV = Vector2(t, o);
		pVertices[6*2+5].Pos = Vector3::Create(s, -s, -s);		pVertices[6*2+5].UV = Vector2(t, t);

		// +Z
		pVertices[6*3+0].Pos = Vector3::Create(-s, s, s);		pVertices[6*3+0].UV = Vector2(o, t);
		pVertices[6*3+1].Pos = Vector3::Create(-s, -s, s);		pVertices[6*3+1].UV = Vector2(o, o);
		pVertices[6*3+2].Pos = Vector3::Create(s, -s, s);		pVertices[6*3+2].UV = Vector2(t, o);
		pVertices[6*3+3].Pos = Vector3::Create(-s, s, s);		pVertices[6*3+3].UV = Vector2(o, t);
		pVertices[6*3+4].Pos = Vector3::Create(s, -s, s);		pVertices[6*3+4].UV = Vector2(t, o);
		pVertices[6*3+5].Pos = Vector3::Create(s, s, s);		pVertices[6*3+5].UV = Vector2(t, t);

		// +Y
		pVertices[6*4+0].Pos = Vector3::Create(-s, s, -s);		pVertices[6*4+0].UV = Vector2(o, t);
		pVertices[6*4+1].Pos = Vector3::Create(-s, s, s);		pVertices[6*4+1].UV = Vector2(o, o);
		pVertices[6*4+2].Pos = Vector3::Create(s, s, s);		pVertices[6*4+2].UV = Vector2(t, o);
		pVertices[6*4+3].Pos = Vector3::Create(-s, s, -s);		pVertices[6*4+3].UV = Vector2(o, t);
		pVertices[6*4+4].Pos = Vector3::Create(s, s, s);		pVertices[6*4+4].UV = Vector2(t, o);
		pVertices[6*4+5].Pos = Vector3::Create(s, s, -s);		pVertices[6*4+5].UV = Vector2(t, t);

		// -Y
		pVertices[6*5+0].Pos = Vector3::Create(s, -s, -s);		pVertices[6*5+0].UV = Vector2(o, t);
		pVertices[6*5+1].Pos = Vector3::Create(s, -s, s);		pVertices[6*5+1].UV = Vector2(o, o);
		pVertices[6*5+2].Pos = Vector3::Create(-s, -s, s);		pVertices[6*5+2].UV = Vector2(t, o);
		pVertices[6*5+3].Pos = Vector3::Create(s, -s, -s);		pVertices[6*5+3].UV = Vector2(o, t);
		pVertices[6*5+4].Pos = Vector3::Create(-s, -s, s);		pVertices[6*5+4].UV = Vector2(t, o);
		pVertices[6*5+5].Pos = Vector3::Create(-s, -s, -s);	pVertices[6*5+5].UV = Vector2(t, t);

		pVertexBuffer->SetVertices(pVertices, 36);
		pVertexBuffer->SetRenderStateSetter(snew SkyBoxStateSetter());
		pVertexBuffer->SetDirty(false);
		pVertexBuffer->SetDynamic(true);
		return pVertexBuffer;
	}

	//! adds a characters to a text vertex buffer
	void GeometryProvider::AddCharacter(VertexBuffer* pVertexBuffer, Font* pFont, char c, const Vector2& vCharacterPos, u32 currentCharacter, const Color& color)
	{
		SHOOT_ASSERT(currentCharacter*6+6 <= pVertexBuffer->GetMaxVertices(), "TextVertexBuffer overflow");
		u32 index = (c - ' ');
		if(index > 111)
		{
			index = ('?' - ' '); // use ? for missing characters
		}
		u32 row = index / pFont->GetNumCharactersPerLine();
		u32 column = index % pFont->GetNumCharactersPerLine();

		Vector2 textureSize = pFont->GetTextureSize();
		Vector2 characterSize = pFont->GetCharacterSize();
		Vector2 uvSize(characterSize.X/textureSize.X, characterSize.Y/textureSize.Y);
		Vector2 uvStart(column*uvSize.X, row*uvSize.Y);
		Vector2 uvEnd = uvStart + uvSize;

		AABBox2D srcRect(uvStart, uvEnd);
		AABBox2D destRect(vCharacterPos, vCharacterPos+characterSize);
		Vertex3D* pVertices = pVertexBuffer->GetVertices();
		u32 i = currentCharacter;
		pVertices[i*6+0].UV = Vector2(srcRect.Min().X, srcRect.Min().Y); pVertices[i*6+0].Pos = Vector3::Create(destRect.Min().X, destRect.Min().Y, 0.0f);
		pVertices[i*6+1].UV = Vector2(srcRect.Max().X, srcRect.Min().Y); pVertices[i*6+1].Pos = Vector3::Create(destRect.Max().X, destRect.Min().Y, 0.0f);
		pVertices[i*6+2].UV = Vector2(srcRect.Min().X, srcRect.Max().Y); pVertices[i*6+2].Pos = Vector3::Create(destRect.Min().X, destRect.Max().Y, 0.0f);
		pVertices[i*6+3].UV = Vector2(srcRect.Min().X, srcRect.Max().Y); pVertices[i*6+3].Pos = Vector3::Create(destRect.Min().X, destRect.Max().Y, 0.0f);
		pVertices[i*6+4].UV = Vector2(srcRect.Max().X, srcRect.Min().Y); pVertices[i*6+4].Pos = Vector3::Create(destRect.Max().X, destRect.Min().Y, 0.0f);
		pVertices[i*6+5].UV = Vector2(srcRect.Max().X, srcRect.Max().Y); pVertices[i*6+5].Pos = Vector3::Create(destRect.Max().X, destRect.Max().Y, 0.0f);

		for(u32 j=0; j<6; ++j)
		{
			pVertices[i*6+j].color = color;
		}
	}
}
