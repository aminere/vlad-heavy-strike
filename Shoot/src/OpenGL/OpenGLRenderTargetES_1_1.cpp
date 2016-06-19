/* 
 
 Amine Rehioui
 Created: August 16th 2010
 
 */

#include "Shoot.h"

#include "OpenGLRenderTargetES_1_1.h"

#include "OpenGLExtensionHandler.h"

namespace shoot
{
	//! constructor
	OpenGLRenderTargetES_1_1::OpenGLRenderTargetES_1_1(const Vector2& vSize)
	{
		m_eFormat = TF_RGBA;
		m_vSize = vSize;
		
		GL_CHECK(glEnable(GL_TEXTURE_2D));
		
		// generate frame buffer
		OpenGLExtensionHandler* pExtHandler = OpenGLExtensionHandler::Instance();
		
		if(pExtHandler->HasExtension(OpenGLExtensionHandler::E_FBO))
		{
			// gen frame buffer
			pExtHandler->extGlGenFramebuffers(1, &m_FrameBufferID);
			pExtHandler->extGlBindFramebuffer(GL_FRAMEBUFFER_OES, m_FrameBufferID);
			
			// generate color texture
			GL_CHECK(glGenTextures(1, &m_GLTextureID));

			// generate depth buffer
			pExtHandler->extGlGenRenderbuffers(1, &m_DepthBufferID);
			
			Resize(vSize);

			// bind buffers	
			pExtHandler->extGlFramebufferTexture2D(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_TEXTURE_2D, m_GLTextureID, 0);		
			pExtHandler->extGlFramebufferRenderbuffer(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, m_DepthBufferID);

			// restore frame buffer
			pExtHandler->extGlBindFramebuffer(GL_FRAMEBUFFER_OES, 0);
		}
		else
		{
			SHOOT_ASSERT(0, "OpenGLRenderTarget not implemented on this platform");
		}
	}

	//! destructor
	OpenGLRenderTargetES_1_1::~OpenGLRenderTargetES_1_1()
	{
		OpenGLExtensionHandler::Instance()->extGlDeleteFramebuffers(1, &m_FrameBufferID);	
		OpenGLExtensionHandler::Instance()->extGlDeleteRenderbuffers(1, &m_DepthBufferID);
	}

	//! resize the texture
	void OpenGLRenderTargetES_1_1::Resize(const Vector2& size)
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
			pExtHandler->extGlBindRenderbuffer(GL_RENDERBUFFER_OES, m_DepthBufferID);
			pExtHandler->extGlRenderbufferStorage(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, u32(size.X), u32(size.Y));		
		}
		else
		{
			SHOOT_ASSERT(0, "OpenGLRenderTarget not implemented on this platform");
		}
	}
}

