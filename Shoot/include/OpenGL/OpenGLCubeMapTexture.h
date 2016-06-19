/* 

Amine Rehioui
Created: April 2nd 2012

*/

#ifndef _OPENGL_CUBEMAP_TEXTURE_H_INCLUDED_
#define _OPENGL_CUBEMAP_TEXTURE_H_INCLUDED_

#include "Texture.h"
#include "Utils.h"

namespace shoot
{
	//! OpenGL CubeMap Texture class
	class OpenGLCubeMapTexture : public Texture
	{
		DECLARE_OBJECT(OpenGLCubeMapTexture, Texture);

	public:

		//! constructor
		OpenGLCubeMapTexture(const char* strPath[6]);

		//! constructor
		OpenGLCubeMapTexture();

		//! destructor
		virtual ~OpenGLCubeMapTexture();

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
		void End();

		//! returns true if this resource is using the specified data
		virtual bool IsUsingData(const char* strPath) const
		{
			for(u32 i=0; i<6; ++i)
			{
				if(Utils::Equals(m_strPath[i].c_str(), strPath))
				{
					return true;
				}
			}
			return false;
		}

	protected:

		std::string m_strPath[6];

		u32 m_GLTextureID;
		Vector2 m_vSize;
		E_Format m_eFormat;
		u32 m_MemorySize;
		void* m_pData[6];
		u32 m_bpp;
	};
}

#endif // _OPENGL_CUBEMAP_TEXTURE_H_INCLUDED_

