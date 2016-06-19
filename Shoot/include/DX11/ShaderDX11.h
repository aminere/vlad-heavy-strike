/* 

Amine Rehioui
Created: July 30th 2013

*/

#ifndef _SHADER_DX11_H_INCLUDED_
#define _SHADER_DX11_H_INCLUDED_

#include "Shader.h"

namespace shoot
{
	//! ShaderDX11 class
	class ShaderDX11 : public ShaderImpl
	{
	public:

		//! constructor
		ShaderDX11();

		//! Load into video memory
		void GraphicLoad();

		//! Unload from video memory
		void GraphicUnload();

		//! starts rendering using this shader
		void Begin();

		//! ends rendering using this shader
		void End();

#ifndef SHOOT_FILE_SYSTEM
		//! compile HLSL code
		static ID3DBlob* CompileHLSL(const char* strSource, const char* strEntryPoint, const char* strShaderModel);
#endif // SHOOT_FILE_SYSTEM

	private:

		//! returns compiled HLSL
		u8* GetCompiledHLSL(const char* strSource, s32& size);

		COMReference<ID3D11InputLayout> m_VertexLayout;
		COMReference<ID3D11VertexShader> m_VS;
		COMReference<ID3D11PixelShader> m_PS;
	};
}

#endif // _SHADER_DX11_H_INCLUDED_

