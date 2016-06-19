/* 

Amine Rehioui
Created: July 28th 2013

*/

#ifndef _TEXTURE_DX11_H_INCLUDED_
#define _TEXTURE_DX11_H_INCLUDED_

#include "Texture.h"

namespace shoot
{
	//! TextureDX11 class
	class TextureDX11 : public Texture
	{
	public:

		//! constructor
		TextureDX11();

		//! constructor
		TextureDX11(const char* strPath);

		//! destructor
		virtual ~TextureDX11();

		//! Load from disk
		void ResourceInit();

		//! Unload disk data
		void ResourceUninit();

		//! Load into video memory
		void GraphicLoad();

		//! Unload from video memory
		void GraphicUnload();

		//! returns the size of the texture
		Vector2 GetSize() const { return m_vSize; }

		//! returns the hardware size of the texture
		Vector2 GetHardwareSize() const { return GetSize(); }

		//! returns the format of the texture
		E_Format GetFormat() const { return m_eFormat; }

		//! returns the texture type
		virtual E_Type GetType() const { return T_2D; }

		//! resize the texture
		virtual void Resize(const Vector2& size) { SHOOT_ASSERT(false, "TODO: implement"); }

		//! returns the memory size in bytes
		virtual u32 GetMemorySize() { return m_MemorySize; }

		//! start rendering with this texture
		void Begin(u32 stage);

		//! ends rendering with this texture
		void End() { }

	protected:
				
		COMReference<ID3D11Texture2D> m_Texture;
		COMReference<ID3D11ShaderResourceView> m_TextureRV;

		Vector2 m_vSize;
		E_Format m_eFormat;
		u32 m_MemorySize;
		void* m_pData;
		u32 m_bpp;
		std::string m_strPath;
	};
}

#endif // _TEXTURE_DX11_H_INCLUDED_

