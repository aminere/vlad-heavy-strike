/* 

Amine Rehioui
Created: July 30th 2013

*/

#include "Shoot.h"

#include "ShaderDX11.h"

#include "DirectX11Driver.h"

#include "File.h"

#ifndef SHOOT_FILE_SYSTEM
#include <d3dcompiler.h>
#endif // SHOOT_FILE_SYSTEM

namespace shoot
{
	//! constructor
	ShaderDX11::ShaderDX11()
	{
	}

	//! Load into video memory
	void ShaderDX11::GraphicLoad()
	{
		SHOOT_ASSERT(m_VS.Get() == NULL, "VertexShader already loaded");
		SHOOT_ASSERT(m_PS.Get() == NULL, "PixelShader already loaded");
		SHOOT_ASSERT(!m_pSource->m_VS.empty(), "VertexShader path is empty");
		SHOOT_ASSERT(!m_pSource->m_PS.empty(), "PixelShader path is empty");

		s32 VSSize = 0;
		s32 PSSize = 0;
		u8* pVSData = NULL;
		u8* pPSData = NULL;
		
#ifdef SHOOT_FILE_SYSTEM
		pVSData = GetCompiledHLSL(m_pSource->m_VS.c_str(), VSSize);
		pPSData = GetCompiledHLSL(m_pSource->m_PS.c_str(), PSSize);
#else
		COMReference<ID3DBlob> vsBlob = CompileHLSL(m_pSource->m_VS.c_str(), "VSMain", "vs_4_0_level_9_3");
		COMReference<ID3DBlob> psBlob = CompileHLSL(m_pSource->m_PS.c_str(), "PSMain", "ps_4_0_level_9_3");
		pVSData = static_cast<u8*>(vsBlob->GetBufferPointer());
		pPSData = static_cast<u8*>(psBlob->GetBufferPointer());
		VSSize = vsBlob->GetBufferSize();
		PSSize = psBlob->GetBufferSize();
#endif

		ID3D11Device1* pDevice = DirectX11Driver::Instance()->GetDevice();
		ID3D11VertexShader* pVS = NULL;
		DX_ASSERT(pDevice->CreateVertexShader(pVSData, VSSize, NULL, &pVS));
		m_VS = pVS;

		ID3D11PixelShader* pPS = NULL;
		DX_ASSERT(pDevice->CreatePixelShader(pPSData, PSSize, NULL, &pPS));
		m_PS = pPS;

		static const u32 numElements = 5;
		D3D11_INPUT_ELEMENT_DESC layout[numElements] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT,		  0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,	  0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "UVW", 0, DXGI_FORMAT_R32G32B32_FLOAT,	  0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		ID3D11InputLayout* pInputLayout = NULL;
		DX_ASSERT(pDevice->CreateInputLayout(layout, numElements, pVSData, VSSize, &pInputLayout));
		m_VertexLayout = pInputLayout;

#ifdef SHOOT_FILE_SYSTEM
		sdelete_array(pVSData);
		sdelete_array(pPSData);
#endif
	}

	//! Unload from video memory
	void ShaderDX11::GraphicUnload()
	{
		m_VS = NULL;
		m_PS = NULL;
		m_VertexLayout = NULL;
	}	

	//! starts rendering using this shader
	void ShaderDX11::Begin()
	{
		if(m_VS.Get() == NULL)
		{
			GraphicLoad();
		}

		ID3D11DeviceContext1* pContext = DirectX11Driver::Instance()->GetContext();
		pContext->IASetInputLayout(m_VertexLayout.Get());
		pContext->VSSetShader(m_VS.Get(), NULL, 0);
		pContext->PSSetShader(m_PS.Get(), NULL, 0);

		DirectX11Driver::Instance()->BindMaterialStates();

		//// get and set custom parameters
		//for(u32 i=0; i<m_pParams->GetSize(); ++i)
		//{
		//	if(ShaderParameter* pParameter = (*m_pParams)[i].Get())
		//	{
		//		u32 parameterLocation = 0;
		//		OpenGLExtensionHandler::Instance()->extGlGetUniformLocation(m_ProgramID, pParameter->GetName().c_str(), &parameterLocation);

		//		switch(pParameter->GetType())
		//		{
		//		case ShaderParameter::Type_Float:
		//			{
		//				f32 f = pParameter->GetFloat();
		//				OpenGLExtensionHandler::Instance()->extGlUniform1fv(parameterLocation, 1, &f);
		//			}					
		//			break;

		//		case ShaderParameter::Type_Color:					
		//			OpenGLExtensionHandler::Instance()->extGlUniform4fv(parameterLocation, 1, pParameter->GetColor().v);									
		//			break;

		//		case ShaderParameter::Type_Vec4:
		//			OpenGLExtensionHandler::Instance()->extGlUniform4fv(parameterLocation, 1, pParameter->GetVec4().v);
		//			break;
		//		}
		//	}
		//}
	}

	//! ends rendering using this shader
	void ShaderDX11::End()
	{
		// TODO unset VS/PS ?
	}

#ifndef SHOOT_FILE_SYSTEM
	//! compile HLSL code
	ID3DBlob* ShaderDX11::CompileHLSL(const char* strSource, const char* strEntryPoint, const char* strShaderModel)
	{
		ID3DBlob* pShaderBlob = NULL;
		COMReference<ID3DBlob> errorBlob;
		u32 flags = D3DCOMPILE_ENABLE_STRICTNESS;
		flags |= D3DCOMPILE_DEBUG;

		//const D3D_SHADER_MACRO defines[] = 
		//{
		//	"EXAMPLE_DEFINE", "1",
		//	NULL, NULL
		//};

		std::string source("data/");
		source = source + strSource;
		std::wstring wstr(source.begin(), source.end());
		ID3DBlob* pErrorBlob = NULL;
		HRESULT hr = D3DCompileFromFile(wstr.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, strEntryPoint, strShaderModel, flags, 0, &pShaderBlob, &pErrorBlob);
		errorBlob = pErrorBlob;
		if(FAILED(hr))
		{
			if(errorBlob.Get())
			{
				SHOOT_ASSERT(false, "Shader error (%s): %s", strSource, (char*)errorBlob->GetBufferPointer());
			}
			else
			{
				SHOOT_ASSERT(false, "Shader error (%s)", strSource);
			}
		}
		return pShaderBlob;
	}
#endif // SHOOT_FILE_SYSTEM

	//! returns compiled HLSL
	u8* ShaderDX11::GetCompiledHLSL(const char* strSource, s32& size)
	{
		File* pFile = File::Create(strSource, File::M_ReadBinary);
		pFile->Open();
		pFile->Read(&size, sizeof(s32));
		u8* pData = snew u8[size];
		pFile->Read(pData, size);
		pFile->Close();
		delete pFile;
		return pData;
	}
}


