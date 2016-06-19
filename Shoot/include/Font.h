/* 

Amine Rehioui
Created: April 21st 2010

*/

#ifndef _FONT_H_INCLUDED_
#define _FONT_H_INCLUDED_

namespace shoot
{
	//! Bitmap font class
	class Font : public Resource
	{
		DECLARE_OBJECT(Font, Resource);

	public:

		//! serializes the font from a stream
		void Serialize(PropertyStream& stream);

		//! returns the texture
		const std::string& GetTexturePath() const { return m_TexturePath; }

		//! returns the character size
		inline const Vector2& GetCharacterSize() const { return m_vCharacterSize; }

		//! returns the texture size
		inline const Vector2& GetTextureSize() const { return m_TextureSize; }

		//! returns the number of characters per line
		inline s32 GetNumCharactersPerLine() const { return m_NumCharactersPerLine; }

	private:

		// properties
		std::string m_TexturePath;
		Vector2 m_vCharacterSize;
		Vector2 m_TextureSize;
		s32 m_NumCharactersPerLine;
	};
}

#endif // _FONT_H_INCLUDED_

