/* 

Amine Rehioui
Created: January 23rd 2013

*/

#include "Shoot.h"

#include "OpenGLShadowMap.h"

#include "OpenGLExtensionHandler.h"

namespace shoot
{
	//! constructor
	OpenGLShadowMap::OpenGLShadowMap(const Vector2& vSize)
	{
		SetName("OpenGLShadowMap");
		m_eFormat = TF_RGBA;
		m_vSize = vSize;

		GL_CHECK(glEnable(GL_TEXTURE_2D));

		// generate frame buffer
		OpenGLExtensionHandler* pExtHandler = OpenGLExtensionHandler::Instance();

		if(pExtHandler->HasExtension(OpenGLExtensionHandler::E_FBO))
		{
			// gen frame buffer
			pExtHandler->extGlGenFramebuffers(1, &m_FrameBufferID);
			// TODO iPhone proof
			//pExtHandler->extGlBindFramebuffer(GL_FRAMEBUFFER_EXT, m_FrameBufferID);

			// generate and set depth texture
			GL_CHECK(glGenTextures(1, &m_GLTextureID));
			GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_GLTextureID));
			GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			// Remove artefact on the edges of the shadowmap
			// TODO iPhone proof
			//GL_CHECK(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP));
			//GL_CHECK(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP));
			//GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, u32(vSize.X), u32(vSize.Y), 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0));			
			
			// disable color rendering
			// TODO iPhone proof
			//glDrawBuffer(GL_NONE);
			
			// bind buffers
			// TODO iPhone proof
			//pExtHandler->extGlFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, m_GLTextureID, 0);
			//SHOOT_ASSERT(pExtHandler->extGlCheckFramebufferStatus(GL_FRAMEBUFFER_EXT) == GL_FRAMEBUFFER_COMPLETE_EXT, "ShadowMap error");			

			// restore frame buffer
			// TODO iPhone proof
			//pExtHandler->extGlBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
		}
		else
		{
			SHOOT_ASSERT(0, "OpenGLRenderTarget not implemented on this platform");
		}
	}

	//! destructor
	OpenGLShadowMap::~OpenGLShadowMap()
	{
		OpenGLExtensionHandler::Instance()->extGlDeleteFramebuffers(1, &m_FrameBufferID);
	}	
}

