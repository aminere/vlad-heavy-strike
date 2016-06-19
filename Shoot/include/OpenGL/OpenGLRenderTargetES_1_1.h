/* 
 
 Amine Rehioui
 Created: August 16th 2010
 
 */

#ifndef _OPENGL_RENDER_TARGET_ES_1_1_H_INCLUDED_
#define _OPENGL_RENDER_TARGET_ES_1_1_H_INCLUDED_

#include "OpenGLTexture.h"

namespace shoot
{
	//! OpenGL ES 1.1 render target texture
	class OpenGLRenderTargetES_1_1 : public OpenGLTexture
	{
		typedef OpenGLTexture super;
		
	public:
		
		//! constructor
		OpenGLRenderTargetES_1_1(const Vector2& vSize);

		//! destryctor
		virtual ~OpenGLRenderTargetES_1_1();
				
		//! returns the texture type
		E_Type GetType() const { return T_RenderTarget; }
		
		//! returns the frame buffer ID
		u32 GetFrameBufferID() const { return m_FrameBufferID; }

		//! resize the texture
		void Resize(const Vector2& size);

	private:
		
		u32 m_FrameBufferID;
		u32 m_DepthBufferID;
	};
}

#endif // _OPENGL_RENDER_TARGET_ES_1_1_H_INCLUDED_

