/* 

Amine Rehioui
Created: January 23rd 2013

*/

#ifndef _OPENGL_SHADOW_MAP_H_INCLUDED_
#define _OPENGL_SHADOW_MAP_H_INCLUDED_

#include "OpenGLTexture.h"

namespace shoot
{
	//! OpenGL shadow map
	class OpenGLShadowMap : public OpenGLTexture
	{
		typedef OpenGLTexture super;

	public:

		//! constructor
		OpenGLShadowMap(const Vector2& vSize);

		//! destructor
		virtual ~OpenGLShadowMap();

		//! returns the texture type
		E_Type GetType() const { return T_ShadowMap; }

		//! returns the frame buffer ID
		u32 GetFrameBufferID() const { return m_FrameBufferID; }

	private:

		u32 m_FrameBufferID;
		u32 m_DepthBufferID;
	};
}

#endif // _OPENGL_SHADOW_MAP_H_INCLUDED_

