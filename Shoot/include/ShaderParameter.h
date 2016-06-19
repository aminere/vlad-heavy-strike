/* 

Amine Rehioui
Created: September 4th 2012

*/

#ifndef _SHADER_PARAMETER_H_INCLUDED_
#define _SHADER_PARAMETER_H_INCLUDED_

#include "Color.h"
#include "Vector4.h"

namespace shoot
{
	//! base ShaderParameter class
	class ShaderParameter : public Object
	{
		DECLARE_OBJECT(ShaderParameter, Object);

	public:

		//! parameter type
		enum E_Type
		{
			Type_Float,
			Type_Color,
			Type_Vec4,
			Type_None
		};

		//! serializes the object to/from a PropertyStream
		virtual void Serialize(PropertyStream& stream)
		{
			stream.Serialize(PT_String, "Name", &m_Name);
		}

		//! returns the parameter name
		std::string GetName() const { return m_Name; }

		//! returns the parameter type
		virtual E_Type GetType() const { return Type_None; }

		//! returns the parameter as Float
		virtual f32 GetFloat() const { return 0.0f; }

		//! returns the parameter as Color
		virtual Color GetColor() const { return Color(); }

		//! returns the parameter as Vector4
		virtual Vector4 GetVec4() const { return Vector4::Zero; }

	private:

		// propeties
		std::string m_Name;
	};

	//! FloatShaderParameter
	class FloatShaderParameter : public ShaderParameter
	{
		DECLARE_OBJECT(FloatShaderParameter, ShaderParameter);

	public:		

		//! constructor
		FloatShaderParameter()
			: m_Value(0.0f)
		{
		}

		//! serializes the object to/from a PropertyStream
		virtual void Serialize(PropertyStream& stream)
		{
			super::Serialize(stream);
			stream.Serialize(PT_Float, "Value", &m_Value);
		}

		//! returns the parameter type
		virtual E_Type GetType() const { return Type_Float; }

		//! returns the parameter as Float
		virtual f32 GetFloat() const { return m_Value; }

	private:

		f32 m_Value;
	};

	//! ColorShaderParameter
	class ColorShaderParameter : public ShaderParameter
	{
		DECLARE_OBJECT(ColorShaderParameter, ShaderParameter);

	public:
		
		//! serializes the object to/from a PropertyStream
		virtual void Serialize(PropertyStream& stream)
		{
			super::Serialize(stream);
			stream.Serialize(PT_Color, "Value", &m_Value);
		}

		//! returns the parameter type
		virtual E_Type GetType() const { return Type_Color; }

		//! returns the parameter as Float
		virtual Color GetColor() const { return m_Value; }

	private:

		Color m_Value;
	};

	//! Vec4ShaderParameter
	class Vec4ShaderParameter : public ShaderParameter
	{
		DECLARE_OBJECT(Vec4ShaderParameter, ShaderParameter);

	public:
		
		//! serializes the object to/from a PropertyStream
		virtual void Serialize(PropertyStream& stream)
		{
			super::Serialize(stream);
			stream.Serialize(PT_Vec4, "Value", &m_Value);
		}

		//! returns the parameter type
		virtual E_Type GetType() const { return Type_Vec4; }

		//! returns the parameter as Float
		virtual Vector4 GetVec4() const { return m_Value; }

	private:

		Vector4 m_Value;
	};
}
#endif // _SHADER_PARAMETER_H_INCLUDED_

