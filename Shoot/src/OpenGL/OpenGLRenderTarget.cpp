/* 

Amine Rehioui
Created: August 15th 2010

*/

#include "Shoot.h"

#include "OpenGLRenderTarget.h"

#include "OpenGLExtensionHandler.h"

namespace shoot
{
	//! constructor
	OpenGLRenderTarget::OpenGLRenderTarget(const Vector2& vSize)
	{
		SetName("OpenGLRenderTarget");
		m_eFormat = TF_RGBA;
		m_vSize = vSize;

		GL_CHECK(glEnable(GL_TEXTURE_2D));

		// generate frame buffer
		OpenGLExtensionHandler* pExtHandler = OpenGLExtensionHandler::Instance();

		if(pExtHandler->HasExtension(OpenGLExtensionHandler::E_FBO))
		{
			// gen frame buffer
			pExtHandler->extGlGenFramebuffers(1, &m_FrameBufferID);
			pExtHandler->extGlBindFramebuffer(GL_FRAMEBUFFER_EXT, m_FrameBufferID);

			// generate color texture
			GL_CHECK(glGenTextures(1, &m_GLTextureID));

			// generate depth buffer
			pExtHandler->extGlGenRenderbuffers(1, &m_DepthBufferID);
			
			Resize(vSize);
			
			// bind buffers
			pExtHandler->extGlFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_GLTextureID, 0);		
			pExtHandler->extGlFramebufferRenderbuffer(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, m_DepthBufferID);

			// restore frame buffer
			pExtHandler->extGlBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
		}
		else
		{
			SHOOT_ASSERT(0, "OpenGLRenderTarget not implemented on this platform");
		}
	}

	//! destructor
	OpenGLRenderTarget::~OpenGLRenderTarget()
	{
		OpenGLExtensionHandler::Instance()->extGlDeleteFramebuffers(1, &m_FrameBufferID);	
		OpenGLExtensionHandler::Instance()->extGlDeleteRenderbuffers(1, &m_DepthBufferID);
	}

	//! resize the texture
	void OpenGLRenderTarget::Resize(const Vector2& size)
	{
		m_vSize = size;
		OpenGLExtensionHandler* pExtHandler = OpenGLExtensionHandler::Instance();

		if(pExtHandler->HasExtension(OpenGLExtensionHandler::E_FBO))
		{
			// set up color texture
			GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_GLTextureID));
			GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));			
			GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, u32(size.X), u32(size.Y), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0));

			// set up depth buffer			
			pExtHandler->extGlBindRenderbuffer(GL_RENDERBUFFER_EXT, m_DepthBufferID);
			pExtHandler->extGlRenderbufferStorage(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, u32(size.X), u32(size.Y));		
		}
		else
		{
			SHOOT_ASSERT(0, "OpenGLRenderTarget not implemented on this platform");
		}
	}
}

