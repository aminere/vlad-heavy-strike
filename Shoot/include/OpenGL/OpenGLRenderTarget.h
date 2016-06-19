/* 

Amine Rehioui
Created: August 15th 2010

*/

#ifndef _OPENGL_RENDER_TARGET_H_INCLUDED_
#define _OPENGL_RENDER_TARGET_H_INCLUDED_

#include "OpenGLTexture.h"

namespace shoot
{
	//! OpenGL render target texture
	class OpenGLRenderTarget : public OpenGLTexture
	{
		typedef OpenGLTexture super;

	public:

		//! constructor
		OpenGLRenderTarget(const Vector2& vSize);

		//! destructor
		virtual ~OpenGLRenderTarget();

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

#endif // _OPENGL_RENDER_TARGET_H_INCLUDED_

