/* 

Amine Rehioui
Created: July 8th 2010

*/

#ifndef _OPENGL_TEXTURE_H_INCLUDED_
#define _OPENGL_TEXTURE_H_INCLUDED_

#include "Texture.h"

namespace shoot
{
	//! OpenGL Texture class
	class OpenGLTexture : public Texture
	{
	public:

		//! constructor
		OpenGLTexture();

		//! constructor
		OpenGLTexture(const char* strPath);

		//! destructor
		virtual ~OpenGLTexture();

		//! Load from disk
		void ResourceInit();

		//! Unload disk data
		void ResourceUninit();

		//! Load into video memory
		void GraphicLoad();

		//! Unload from video memory
		void GraphicUnload();

		//! returns loaded status
		bool IsLoaded() { return m_GLTextureID != 0; }

		//! returns the size of the texture
		Vector2 GetSize() const { return m_vSize; }

		//! returns the hardware size of the texture
		Vector2 GetHardwareSize() const { return m_vHardwareSize; }

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

		u32 m_GLTextureID;
		Vector2 m_vSize;
		Vector2 m_vHardwareSize;
		E_Format m_eFormat;
		u32 m_MemorySize;
		void* m_pData;
		u32 m_bpp;
		std::string m_strPath;
	};
}

#endif // _OPENGL_TEXTURE_H_INCLUDED_

