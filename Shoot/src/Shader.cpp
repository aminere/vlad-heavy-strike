/* 

Amine Rehioui
Created: September 4th 2012

*/

#include "Shoot.h"

#include "Shader.h"

#include "File.h"

namespace shoot
{
	DEFINE_OBJECT(Shader);

	//! constructor
	Shader::Shader()
	{
		m_pImpl = GraphicsDriver::Instance()->CreateShader();
	}

	//! destructor
	Shader::~Shader()
	{
		sdelete(m_pImpl);
	}

	//! serializes the object to/from a PropertyStream
	void Shader::Serialize(PropertyStream& stream)
	{
		stream.Serialize(PT_Struct, "GLSL", &m_GLSL);
		stream.Serialize(PT_Struct, "HLSL", &m_HLSL);
		stream.SerializeArray("Parameters", &m_Parameters, PT_Reference);

		if(stream.GetMode() == SM_Read)
		{
			m_pImpl->SetSource(GetSource());
			m_pImpl->SetParams(&m_Parameters);
		}
	}

	//! returns the source
	const ShaderImpl::Source* Shader::GetSource() const
	{
		if(GraphicsDriver::Instance()->GetType() == GraphicsDriver::DT_DirectX11)
		{
			return &m_HLSL;
		}
		else
		{
			return &m_GLSL;
		}
	}

	//! reads text from a file
	char* ShaderImpl::ReadFileText(const char* path)
	{
		File* pFile = File::Create(path, File::M_ReadBinary);
		pFile->Open();
		pFile->SetOffset(0, File::OT_End);
		u32 fileSize = pFile->GetOffset();		
		pFile->SetOffset(0, shoot::File::OT_Start);
		char* pText = snew char[fileSize+1];
		pFile->Read(pText, fileSize);
		pText[fileSize] = '\0';
		pFile->Close();
		sdelete(pFile);
		return pText;
	}
}

