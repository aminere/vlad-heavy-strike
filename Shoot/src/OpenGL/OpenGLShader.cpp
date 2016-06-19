/* 

Amine Rehioui
Created: September 4th 2012

*/

#include "Shoot.h"

#include "OpenGLShader.h"

#include "OpenGLExtensionHandler.h"

namespace shoot
{
	//! constructor
	OpenGLShader::OpenGLShader()
		: m_VertexShaderID(0)
		, m_PixelShaderID(0)
		, m_ProgramID(0)
		, m_WorldMatrixLocation(0)
	{
	}

	//! Load into video memory
	void OpenGLShader::GraphicLoad()
	{
		SHOOT_ASSERT(m_ProgramID == 0, "Shader already loaded");
		if(!m_pSource->m_VS.empty())
		{
			OpenGLExtensionHandler::Instance()->extGlCreateShader(OpenGLExtensionHandler::ST_Vertex, &m_VertexShaderID);
			char* vertexShaderSource = ReadFileText(m_pSource->m_VS.c_str());
			OpenGLExtensionHandler::Instance()->extGlShaderSource(m_VertexShaderID, 1, (const char**)&vertexShaderSource, NULL);			
			OpenGLExtensionHandler::Instance()->extGlCompileShader(m_VertexShaderID);
			OpenGLExtensionHandler::Instance()->extGlCheckCompileStatus(m_VertexShaderID);
			sdelete_array(vertexShaderSource);
		}

		if(!m_pSource->m_PS.empty())
		{
			OpenGLExtensionHandler::Instance()->extGlCreateShader(OpenGLExtensionHandler::ST_Fragment, &m_PixelShaderID);
			char* pixelShaderSource = ReadFileText(m_pSource->m_PS.c_str());
			OpenGLExtensionHandler::Instance()->extGlShaderSource(m_PixelShaderID, 1, (const char**)&pixelShaderSource, NULL);
			OpenGLExtensionHandler::Instance()->extGlCompileShader(m_PixelShaderID);
			OpenGLExtensionHandler::Instance()->extGlCheckCompileStatus(m_PixelShaderID);
			sdelete_array(pixelShaderSource);
		}

		if(m_VertexShaderID && m_PixelShaderID)
		{
			OpenGLExtensionHandler::Instance()->extGlCreateProgram(&m_ProgramID);
			OpenGLExtensionHandler::Instance()->extGlAttachShader(m_ProgramID, m_VertexShaderID);
			OpenGLExtensionHandler::Instance()->extGlAttachShader(m_ProgramID, m_PixelShaderID);
			OpenGLExtensionHandler::Instance()->extGlLinkProgram(m_ProgramID);
			OpenGLExtensionHandler::Instance()->extGlCheckLinkStatus(m_ProgramID);
		}
	}

	//! Unload from video memory
	void OpenGLShader::GraphicUnload()
	{
		if(m_ProgramID != 0)
		{
			OpenGLExtensionHandler::Instance()->extGlDeleteShader(m_VertexShaderID);
			OpenGLExtensionHandler::Instance()->extGlDeleteShader(m_PixelShaderID);
			OpenGLExtensionHandler::Instance()->extGlDeleteProgram(m_ProgramID);

			m_VertexShaderID = 0;
			m_PixelShaderID = 0;
			m_ProgramID = 0;
		}		
	}	

	//! starts rendering using this shader
	void OpenGLShader::Begin()
	{
		if(m_ProgramID == 0)
		{
			GraphicLoad();
		}

		OpenGLExtensionHandler::Instance()->extGlUseProgram(m_ProgramID);

		// get standard parameters location
		OpenGLExtensionHandler::Instance()->extGlGetUniformLocation(m_ProgramID, "WorldMatrix", &m_WorldMatrixLocation);

		// get and set custom parameters
		for(u32 i=0; i<m_pParams->GetSize(); ++i)
		{
			if(ShaderParameter* pParameter = (*m_pParams)[i].Get())
			{
				u32 parameterLocation = 0;
				OpenGLExtensionHandler::Instance()->extGlGetUniformLocation(m_ProgramID, pParameter->GetName().c_str(), &parameterLocation);

				switch(pParameter->GetType())
				{
				case ShaderParameter::Type_Float:
					{
						f32 f = pParameter->GetFloat();
						OpenGLExtensionHandler::Instance()->extGlUniform1fv(parameterLocation, 1, &f);
					}					
					break;

				case ShaderParameter::Type_Color:					
					OpenGLExtensionHandler::Instance()->extGlUniform4fv(parameterLocation, 1, pParameter->GetColor().v);									
					break;

				case ShaderParameter::Type_Vec4:
					OpenGLExtensionHandler::Instance()->extGlUniform4fv(parameterLocation, 1, pParameter->GetVec4().v);
					break;
				}
			}
		}
	}

	//! ends rendering using this shader
	void OpenGLShader::End()
	{
		OpenGLExtensionHandler::Instance()->extGlUseProgram(0);
	}
}


