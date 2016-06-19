/* 

Amine Rehioui
Created: April 21st 2010

*/

#ifndef _MATERIAL_H_INCLUDED_
#define _MATERIAL_H_INCLUDED_

#include "Color.h"
#include "Texture.h"
#include "Shader.h"

namespace shoot
{
	//! Material
	class Material : public Object
	{
	public:
		
		DECLARE_OBJECT(Material, Object);

		//! Material flags
		enum E_Flag
		{
			MF_Lighting		 = 0x1 << 0,
			MF_AlphaBlending = 0x1 << 1,
			MF_DepthTest	 = 0x1 << 2,
			MF_DepthWrite	 = 0x1 << 3,
		};

		//! Material creation info
		struct CreationInfo : public ISerializableStruct
		{
			//! constructor
			CreationInfo();

			//! Reads/Writes struct properties from/to a stream
			void Serialize(PropertyStream& stream);

			//! Initializes a material
			void Init(Material* pMaterial);

			//! sets a flag status
			inline void SetFlag(E_Flag eFlag, bool bEnabled) { bEnabled ? (m_Mask |= eFlag) : (m_Mask &= ~eFlag); }

			//! returns a flag status
			inline bool GetFlag(E_Flag eFlag) const { return (m_Mask & eFlag) != 0; }

			//! comparison operator
			bool operator == (const CreationInfo& other) const;

			//! TextureInfo
			struct TextureInfo : public ISerializableStruct
			{
				TextureInfo()
					: m_bMipMaps(true)
				{
				}

				//! Reads/Writes struct properties from/to a stream
				void Serialize(PropertyStream& stream)
				{
					stream.Serialize(PT_File, "Path", &m_Path);
					stream.Serialize(PT_Bool, "MipMaps", &m_bMipMaps);
				}

				std::string m_Path;
				bool m_bMipMaps;

				//! comparison operator
				bool operator == (const TextureInfo& other) const
				{
					return (m_Path == other.m_Path) && (m_bMipMaps == other.m_bMipMaps);
				}
			};

			Color m_Color;
			u32 m_Mask;
			u32 m_SrcBlendFactor;
			u32 m_DestBlendFactor;
			u32 m_CullMode;
			Array<TextureInfo> m_TextureInfos;
			Reference<Shader> m_Shader;
		};

		//! constructor
		Material();

		//! constructor
		Material(const CreationInfo& info);

		//! Reads/Writes struct properties from/to a stream
		virtual void Serialize(PropertyStream& stream);
		
		//! sets the color
		inline void SetColor(const Color& color) { m_CreationInfo.m_Color = color; }

		//! returns the color
		inline const Color& GetColor() const { return m_CreationInfo.m_Color; }

		//! returns a texture
		virtual Texture* GetTexture(u32 index = 0) { return (index < m_aTextures.size()) ? m_aTextures[index].Get() : NULL; }
		
		//! sets a texture
		void SetTexture(u32 index, Texture* pTexture);

		//! sets the shader
		inline void SetShader(Shader* pShader) { m_CreationInfo.m_Shader = pShader; }

		//! returns the shader
		inline const Shader* GetShader() const { return m_CreationInfo.m_Shader.Get(); }

		//! sets a flag status
		inline void SetFlag(E_Flag eFlag, bool bEnabled) { m_CreationInfo.SetFlag(eFlag, bEnabled); }

		//! sets the cull mode
		inline void SetCullMode(u32 cullMode) { m_CreationInfo.m_CullMode = cullMode; }

		//! returns the creation info
		const CreationInfo& GetCreationInfo() const { return m_CreationInfo; }

		//! start rendering with this material
		virtual void Begin();

		//! ends rendering with this material
		virtual void End();

	private:

		// properties
		CreationInfo m_CreationInfo;		
		
	protected:

		std::vector< Reference<Texture> > m_aTextures;
		static std::string m_strDefaultShader;
	};
}

#endif // _MATERIAL_H_INCLUDED_

