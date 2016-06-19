/* 

Amine Rehioui
Created: July 8th 2010

*/

#include "Shoot.h"

#include "OpenGLTexture.h"

#include "TextureLoader.h"

#include "OpenGLExtensionHandler.h"

namespace shoot
{
	//! constructor
	OpenGLTexture::OpenGLTexture()
		: m_pData(NULL)
		, m_bpp(2)
		, m_GLTextureID(0)
		, m_MemorySize(0)
		, m_eFormat(TF_Unknown)
	{
	}

	//! constructor
	OpenGLTexture::OpenGLTexture(const char* strPath)
		: Texture()
		, m_pData(NULL)
		, m_bpp(2)
		, m_GLTextureID(0)
		, m_MemorySize(0)
		, m_eFormat(TF_Unknown)		
		, m_strPath(strPath)
	{
		ResourceInit();
	}

	//! destructor
	OpenGLTexture::~OpenGLTexture()
	{
		ResourceUninit();
	}

	//! Load from disk
	void OpenGLTexture::ResourceInit()
	{		
		if(m_pData)
		{
			SHOOT_WARNING(false, "Texture '%s' disk data already loaded", m_strPath.c_str());
			return;
		}

		s32 width, height, channels;
		if(m_pData = TextureLoader::Load(m_strPath.c_str(), width, height, channels, m_bpp))
		{
			m_vSize.Set(f32(width), f32(height));

			if(!OpenGLExtensionHandler::Instance()->HasExtension(OpenGLExtensionHandler::E_RectangleTexture))
			{
				m_vHardwareSize.Set(f32(Math::GetNextPow2(width)), f32(Math::GetNextPow2(height)));
			}
			else
			{
				m_vHardwareSize = m_vSize;
			}

			switch(channels)
			{
			case 3:	m_eFormat = TF_RGB;	break;
			case 4:	m_eFormat = TF_RGBA; break;			
			default: SHOOT_ASSERT(0, "Texture '%s' has unsupported format", m_strPath.c_str());
			}
		}
	}

	//! Unload disk data
	void OpenGLTexture::ResourceUninit()
	{
		GraphicUnload();

		u8* pData = static_cast<u8*>(m_pData);
		sdelete_array(pData);
		m_pData = NULL;
	}

	//! Load into video memory
	void OpenGLTexture::GraphicLoad()
	{
		if(m_GLTextureID)
		{
			SHOOT_WARNING(false, "Texture '%s' video data already loaded", m_strPath.c_str());
			return;
		}

		if(!m_pData)
		{
			SHOOT_ASSERT(false, "Texture '%s' disk data not available", m_strPath.c_str());
			return;
		}
		
		GL_CHECK(glEnable(GL_TEXTURE_2D));
		GL_CHECK(glGenTextures(1, &m_GLTextureID));
		GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_GLTextureID)); // 2d texture (x and y size)
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		if(m_bMipMaps)
		{
			GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST)); 
			GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE));
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}

		Size size((s32)m_vSize.X, (s32)m_vSize.Y);
		Size hwSize((s32)m_vHardwareSize.X, (s32)m_vHardwareSize.Y);
		if((size != hwSize))
		{
			u8* pOldData = static_cast<u8*>(m_pData);
			u8* pNewData = snew u8[hwSize.Width * hwSize.Height * m_bpp];
			memset(pNewData, 0, hwSize.Width * hwSize.Height * m_bpp);
			for(s32 row=0; row<size.Height; ++row)
			{
				memcpy(pNewData+row*hwSize.Width*m_bpp, pOldData+row*size.Width*m_bpp, size.Width*m_bpp);
			}
			sdelete_array(pOldData);
			m_pData = pNewData;
		}

		// Load texture into video memory
		switch(m_eFormat)
		{
		case TF_RGB:
			GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, hwSize.Width, hwSize.Height, 0, GL_RGB, (m_bpp == 2) ? GL_UNSIGNED_SHORT_5_6_5 : GL_UNSIGNED_BYTE, m_pData));
			break;

		case TF_RGBA:
			GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, hwSize.Width, hwSize.Height, 0, GL_RGBA, (m_bpp == 2) ? GL_UNSIGNED_SHORT_4_4_4_4 : GL_UNSIGNED_BYTE, m_pData));
			break;
		}

		m_MemorySize = hwSize.Width*hwSize.Height*m_bpp;
		
		u8* pData = static_cast<u8*>(m_pData);
		sdelete_array(pData);
		m_pData = NULL;
	}

	//! Unload from video memory
	void OpenGLTexture::GraphicUnload()
	{
		if(m_GLTextureID)
		{
			GL_CHECK(glDeleteTextures(1, &m_GLTextureID));
			m_GLTextureID = 0;
			m_MemorySize = 0;
		}
	}	

	//! start rendering with this texture
	void OpenGLTexture::Begin(u32 stage)
	{
		if(m_GLTextureID == 0)
		{
			GraphicLoad();
		}

		GL_CHECK(glEnable(GL_TEXTURE_2D));
		GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_GLTextureID));		
	}
}

