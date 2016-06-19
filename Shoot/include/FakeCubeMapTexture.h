/* 

Amine Rehioui
Created: April 20th 2012

*/

#ifndef _FAKE_CUBEMAP_TEXTURE_H_INCLUDED_
#define _FAKE_CUBEMAP_TEXTURE_H_INCLUDED_

#include "Texture.h"

namespace shoot
{
	//! Fake CubeMap Texture class
	class FakeCubeMapTexture : public Texture
	{
	public:

		//! constructor
		FakeCubeMapTexture(const char* strPath[6]);

		//! constructor
		FakeCubeMapTexture();

		//! returns the size of the texture
		Vector2 GetSize() const { return m_Textures[0]->GetSize(); }

		//! returns the format of the texture
		E_Format GetFormat() const { return m_Textures[0]->GetFormat(); }

		//! returns the texture type
		virtual E_Type GetType() const { return T_2D; }

		//! resize the texture
		virtual void Resize(const Vector2& size) { SHOOT_ASSERT(false, "TODO: implement"); }

		//! returns the memory size in bytes
		virtual u32 GetMemorySize() { return m_Textures[0]->GetMemorySize()*6; }

		//! returns a texture
		Texture* GetTexture(u32 index) { return m_Textures[index]; }

	protected:

		Reference<Texture> m_Textures[6];
	};
}

#endif // _FAKE_CUBEMAP_TEXTURE_H_INCLUDED_

