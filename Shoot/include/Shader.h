/* 

Amine Rehioui
Created: September 4th 2012

*/

#ifndef _SHADER_H_INCLUDED_
#define _SHADER_H_INCLUDED_

#include "ShaderParameter.h"

#include "Utils.h"

namespace shoot
{
	//! ShaderImpl interface
	class ShaderImpl : public GraphicObject
	{
	public:

		//! constructor
		ShaderImpl()
			: m_pSource(NULL)
			, m_pParams(NULL)
		{
		}

		//! destructor
		virtual ~ShaderImpl()
		{
		}

		//! starts rendering using this shader
		virtual void Begin() = 0;

		//! ends rendering using this shader
		virtual void End() = 0;

		//! Source shader code
		struct Source : public ISerializableStruct
		{
			void Serialize(PropertyStream& stream)
			{
				stream.Serialize(PT_File, "VertexShader", &m_VS);
				stream.Serialize(PT_File, "PixelShader", &m_PS);
			}

			std::string m_VS;
			std::string m_PS;
		};

		//! sets the source shader code
		inline void SetSource(const Source* pSource) { m_pSource = pSource; }

		//! sets the shader parameters
		inline void SetParams(Array< Reference<ShaderParameter> >* pParams) { m_pParams = pParams; }

		//! reads text from a file
		static char* ReadFileText(const char* path);

	protected:

		const Source* m_pSource;
		Array< Reference<ShaderParameter> >* m_pParams;
	};

	//! NULL ShaderImpl
	class NULLShaderImpl : public ShaderImpl
	{
		//! starts rendering using this shader
		void Begin() { }

		//! ends rendering using this shader
		void End() { }
	};

	//! base Shader class
	class Shader : public Resource
	{
		DECLARE_OBJECT(Shader, Resource);

	public:

		//! constructor
		Shader();

		//! destructor
		virtual ~Shader();

		//! serializes the object to/from a PropertyStream
		void Serialize(PropertyStream& stream);

		//! starts rendering using this shader
		inline void Begin()
		{
			m_pImpl->Begin();
		}

		//! ends rendering using this shader
		inline void End()
		{
			m_pImpl->End();
		}

		//! Unload disk data
		void ResourceUninit() {	m_pImpl->GraphicUnload(); }

		//! returns true if this resource is using the specified data
		bool IsUsingData(const char* strPath) const
		{
			return Utils::Equals(GetSource()->m_VS.c_str(), strPath) || Utils::Equals(GetSource()->m_PS.c_str(), strPath);
		}

		//! return the ShaderImpl
		const ShaderImpl* GetImpl() const { return m_pImpl; }

	protected:

		//! returns the source
		const ShaderImpl::Source* GetSource() const;

		// properties
		ShaderImpl::Source m_GLSL;
		ShaderImpl::Source m_HLSL;
		Array< Reference<ShaderParameter> > m_Parameters;

		ShaderImpl* m_pImpl;
	};
}

#endif // _SHADER_H_INCLUDED_

