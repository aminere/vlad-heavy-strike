/* 

Amine Rehioui
Created: July 8th 2010

*/

#include "Shoot.h"

#include "OpenGLCubeMapTexture.h"

#include "TextureLoader.h"

#if defined(USE_GLES_1_1) || SHOOT_PLATFORM == SHOOT_PLATFORM_ANDROID
#define GL_TEXTURE_CUBE_MAP_ARB 0x8513
#define GL_TEXTURE_CUBE_MAP 0x8513
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB 0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB 0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB 0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB 0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB 0x851A
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB 0x8519
#endif

namespace shoot
{
	DEFINE_OBJECT(OpenGLCubeMapTexture);

	//! constructor
	OpenGLCubeMapTexture::OpenGLCubeMapTexture(const char* strPath[6])
	: m_GLTextureID(0)
	, m_MemorySize(0)
	, m_eFormat(TF_Unknown)
	, m_bpp(2)
	{
		for(u32 i=0; i<6; ++i)
		{
			m_pData[i] = 0;
			m_strPath[i] = strPath[i];
		}

		m_bMipMaps = false;
		ResourceInit();
	}

	//! constructor
	OpenGLCubeMapTexture::OpenGLCubeMapTexture()
	{
		SHOOT_ASSERT(false, "Invalid Constructor");
	}

	//! destructor
	OpenGLCubeMapTexture::~OpenGLCubeMapTexture()
	{
		ResourceUninit();		
	}

	//! Load from disk
	void OpenGLCubeMapTexture::ResourceInit()
	{
		if(!m_pData[0])
		{
			for(u32 i=0; i<6; ++i)
			{
				// TODO make sure size and format is consistent for all 6 textures
				s32 width, height, channels;				
				m_pData[i] = TextureLoader::Load(m_strPath[i].c_str(), width, height, channels, m_bpp);
				m_vSize.Set(f32(width), f32(height));

				switch(channels)
				{
				case 3:	m_eFormat = TF_RGB;	break;

				case 4:
				default:
					SHOOT_ASSERT(0, "OpenGLCubeMapTexture has unsupported format");
				}
			}
		}
	}

	//! Unload disk data
	void OpenGLCubeMapTexture::ResourceUninit()
	{
		GraphicUnload();

		for(u32 i=0; i<6; ++i)
		{
			u16* pData = static_cast<u16*>(m_pData[i]);
			sdelete_array(pData);
			m_pData[i] = NULL;
		}
	}

	//! Load into video memory
	void OpenGLCubeMapTexture::GraphicLoad()
	{
		SHOOT_ASSERT(m_GLTextureID == 0, "CubeMapTexture already loaded");
		SHOOT_ASSERT(m_bpp == 2 || m_bpp == 3, "Invalid cubemap texture format");

		GL_CHECK(glEnable(GL_TEXTURE_CUBE_MAP_ARB));		
		GL_CHECK(glGenTextures(1, &m_GLTextureID));
		GL_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, m_GLTextureID));
		GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		//GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

		GLenum aTargets[6] = 
		{           
			GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB,
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB,
			GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB,
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB,
			GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB
		};

		// Load textures into video memory
		for(u32 i=0; i<6; ++i)
		{
			u16* pData = static_cast<u16*>(m_pData[i]);
			GL_CHECK(glTexImage2D(aTargets[i], 0, GL_RGB, s32(m_vSize.X), s32(m_vSize.Y), 0, GL_RGB, (m_bpp == 2) ? GL_UNSIGNED_SHORT_5_6_5 : GL_UNSIGNED_BYTE, pData));
			sdelete_array(pData);
			m_pData[i] = NULL;
		}

		m_MemorySize = s32(m_vSize.X*m_vSize.Y*m_bpp)*6;
		GL_CHECK(glDisable(GL_TEXTURE_CUBE_MAP_ARB));
	}

	//! Unload from video memory
	void OpenGLCubeMapTexture::GraphicUnload()
	{
		if(m_GLTextureID != 0)
		{
			GL_CHECK(glEnable(GL_TEXTURE_CUBE_MAP_ARB));
			GL_CHECK(glDeleteTextures(1, &m_GLTextureID));
			GL_CHECK(glDisable(GL_TEXTURE_CUBE_MAP_ARB));
			m_GLTextureID = 0;
			m_MemorySize = 0;
		}
	}

	//! start rendering with this texture
	void OpenGLCubeMapTexture::Begin(u32 stage)
	{
		if(m_GLTextureID == 0)
		{
			GraphicLoad();
		}

		GL_CHECK(glEnable(GL_TEXTURE_CUBE_MAP_ARB));
		GL_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, m_GLTextureID));
	}

	//! ends rendering with this texture
	void OpenGLCubeMapTexture::End()
	{
		GL_CHECK(glDisable(GL_TEXTURE_CUBE_MAP_ARB)); 
	}
}

