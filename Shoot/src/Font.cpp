/* 

Amine Rehioui
Created: April 21st 2010

*/

#include "Shoot.h"

#include "Font.h"

namespace shoot
{
	DEFINE_OBJECT(Font);

	//! serializes the font from a stream
	void Font::Serialize(PropertyStream& stream)
	{
		stream.Serialize(PT_String, "Path", &m_TexturePath);
		stream.Serialize(PT_Vec2, "CharacterSize", &m_vCharacterSize);
		stream.Serialize(PT_Vec2, "TextureSize", &m_TextureSize);		
		stream.Serialize(PT_Int, "CharactersPerLine", &m_NumCharactersPerLine);		
	}
}

