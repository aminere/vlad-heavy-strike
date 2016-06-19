/* 

Amine Rehioui
Created: September 4th 2012

*/

#ifndef _OPENGL_SHADER_H_INCLUDED_
#define _OPENGL_SHADER_H_INCLUDED_

#include "Shader.h"

namespace shoot
{
	//! OpenGLShader class
	class OpenGLShader : public ShaderImpl
	{
	public:

		//! constructor
		OpenGLShader();

		//! Load into video memory
		void GraphicLoad();

		//! Unload from video memory
		void GraphicUnload();

		//! starts rendering using this shader
		void Begin();

		//! ends rendering using this shader
		void End();

		//! returns the world matrix location
		u32 GetWorldMatrixLocation() const { return m_WorldMatrixLocation; }

	private:

		u32 m_VertexShaderID;
		u32 m_PixelShaderID;
		u32 m_ProgramID;

		// standard shader parameters
		u32 m_WorldMatrixLocation;
	};
}

#endif // _OPENGL_SHADER_H_INCLUDED_

