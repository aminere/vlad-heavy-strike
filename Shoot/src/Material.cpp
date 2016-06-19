/* 

Amine Rehioui
Created: July 11th 2010

*/

#include "Shoot.h"

#include "Material.h"

namespace shoot
{
	DEFINE_OBJECT(Material);

	// statics
	std::string Material::m_strDefaultShader = "common/Shaders/test.xml";

	//! blending factor literals
	const char* g_strBlendFactors[] =
	{
		"Zero",
		"One",		
		"SrcColor",
		"InvSrcColor",
		"SrcAlpha",
		"InvSrcAlpha",
		"DestAlpha",
		"InvDestAlpha",
		"DestColor",
		"InvDestColor",
		"SrcAlphaSaturate",
		0
	};

	//! cull mode literals
	const char* g_strCullModes[] =
	{
		"CM_ClockWise",
		"CM_CounterClockWise",
		"CM_None",
		0
	};

	//! constructor
	Material::CreationInfo::CreationInfo()
		: m_Color(1.0f, 1.0f, 1.0f)
		, m_Mask(0)
		, m_SrcBlendFactor(GraphicsDriver::BF_SrcAlpha)
		, m_DestBlendFactor(GraphicsDriver::BF_InvSrcAlpha)
		, m_CullMode(GraphicsDriver::CM_CounterClockWise)
	{
		SetFlag(MF_DepthTest, true);
		SetFlag(MF_DepthWrite, true);
	}

	//! Reads/Writes struct properties from/to a stream
	void Material::CreationInfo::Serialize(PropertyStream& stream)
	{
		bool bLighting = (m_Mask & MF_Lighting) != 0;
		if(stream.Serialize(PT_Bool, "Lighting", &bLighting)) SetFlag(MF_Lighting, bLighting);

		bool bAlphaBlending = (m_Mask & MF_AlphaBlending) != 0;
		if(stream.Serialize(PT_Bool, "AlphaBlending", &bAlphaBlending)) SetFlag(MF_AlphaBlending, bAlphaBlending);

		bool bDepthTest = (m_Mask & MF_DepthTest) != 0;
		if(stream.Serialize(PT_Bool, "DepthTest", &bDepthTest)) SetFlag(MF_DepthTest, bDepthTest);

		bool bDepthWrite = (m_Mask & MF_DepthWrite) != 0;
		if(stream.Serialize(PT_Bool, "DepthWrite", &bDepthWrite)) SetFlag(MF_DepthWrite, bDepthWrite);

		stream.Serialize(PT_Enum, "SrcBlendFactor", &m_SrcBlendFactor, ENUM_PARAMS1(g_strBlendFactors));

		stream.Serialize(PT_Enum, "DestBlendFactor", &m_DestBlendFactor, ENUM_PARAMS1(g_strBlendFactors));		

		stream.Serialize(PT_Enum, "CullMode", &m_CullMode, ENUM_PARAMS1(g_strCullModes));		

		stream.Serialize(PT_Color, "Color", &m_Color);
		stream.Serialize(PT_Float, "Alpha", &m_Color.A);

		stream.SerializeArray("Textures", &m_TextureInfos, PT_Struct);
		
		if((GraphicsDriver::Instance()->GetType() == GraphicsDriver::DT_DirectX11)
		&& (stream.GetMode() == SM_Write)
		&& m_Shader.IsValid()
		&& m_Shader->GetTemplatePath() == m_strDefaultShader)
		{
			// do not save default shader in DX11
		}
		else
		{
			stream.SerializeReference("Shader", &m_Shader);
		}		
	}

	//! Initializes a material
	void Material::CreationInfo::Init(Material* pMaterial)
	{
		for(u32 i=0; i<m_TextureInfos.GetSize(); ++i)
		{
			bool newIndex = (i >= pMaterial->m_aTextures.size());
			Texture* pTexture = newIndex ? NULL : pMaterial->m_aTextures[i].Get();
			std::string texturePath = pTexture ? pTexture->GetTemplatePath() : "";
			bool bTextureChanged = newIndex || (texturePath != m_TextureInfos[i].m_Path);
			if(bTextureChanged)
			{
				Texture* pNewTexture = (m_TextureInfos[i].m_Path.length() > 0) ? ResourceManager::Instance()->GetResource<Texture>(m_TextureInfos[i].m_Path.c_str()) : NULL;
				if(pNewTexture)
				{
					pNewTexture->SetHasMipMaps(m_TextureInfos[i].m_bMipMaps);
				}

				if(i < pMaterial->m_aTextures.size())
				{
					pMaterial->m_aTextures[i].SetObject(pNewTexture);
				}
				else
				{
					pMaterial->m_aTextures.push_back(Reference<Texture>(pNewTexture));
				}
			}
		}

		if(GraphicsDriver::Instance()->GetType() == GraphicsDriver::DT_DirectX11)
		{
			// force default shader if none selected
			if(!m_Shader.IsValid())
			{
				m_Shader = static_cast<Shader*>(ObjectManager::Instance()->Get(m_strDefaultShader.c_str(), true));
			}
		}

		// release unused textures
		for(s32 i=m_TextureInfos.GetSize(); i<s32(pMaterial->m_aTextures.size()); ++i)
		{
			pMaterial->m_aTextures[i].SetObject(NULL);
			pMaterial->m_aTextures.erase(pMaterial->m_aTextures.begin()+i);
			i--;
		}
	}

	//! comparison operator
	bool Material::CreationInfo::operator == (const CreationInfo& other) const
	{
		return (m_Mask == other.m_Mask
			 && m_Color == other.m_Color
			 && m_SrcBlendFactor == other.m_SrcBlendFactor
			 && m_DestBlendFactor == other.m_DestBlendFactor
			 && m_TextureInfos == other.m_TextureInfos
#ifndef DX11 // temporary, because shaders are auto instanciated in Material() - consider doing it in CreationInfo()
			 && m_Shader == other.m_Shader
#endif
			 && m_CullMode == other.m_CullMode);
	}

	//! constructor
	Material::Material()
	{
		if(GraphicsDriver::Instance()->GetType() == GraphicsDriver::DT_DirectX11)
		{
			m_CreationInfo.m_Shader = static_cast<Shader*>(ObjectManager::Instance()->Get(m_strDefaultShader.c_str(), true));
		}
	}

	//! constructor
	Material::Material(const CreationInfo& info)
	{
		m_CreationInfo = info;
		m_CreationInfo.Init(this);
	}

	//! Reads/Writes struct properties from/to a stream
	void Material::Serialize(PropertyStream& stream)
	{			
		super::Serialize(stream);
		
		if(stream.Serialize(PT_Struct, "CreationInfo", &m_CreationInfo))
		{
			m_CreationInfo.Init(this);
		}
	}

	//! sets a texture
	void Material::SetTexture(u32 index, Texture* pTexture)
	{
		std::string path = pTexture->GetTemplatePath();
		if(index < m_aTextures.size())
		{
			m_aTextures[index].SetObject(pTexture);
			m_CreationInfo.m_TextureInfos[index].m_Path = path;
			m_CreationInfo.m_TextureInfos[index].m_bMipMaps = pTexture->GetHasMipMaps();
		}
		else
		{
			m_aTextures.push_back(Reference<Texture>(pTexture));
			CreationInfo::TextureInfo* pInfo = snew CreationInfo::TextureInfo();
			pInfo->m_Path = path;
			pInfo->m_bMipMaps = pTexture->GetHasMipMaps();
			m_CreationInfo.m_TextureInfos.Add(pInfo);
		}
	}

	//! start rendering with this material
	void Material::Begin()
	{
		GraphicsDriver* pDriver = GraphicsDriver::Instance();
		
		Texture* pTexture = m_aTextures.size() ? m_aTextures[0].Get() : NULL;
		if(pTexture)
		{
			pTexture->Begin(0);
		}
		else
		{
			pDriver->SetRenderState(GraphicsDriver::RS_2DTextureMapping, false);
		}

		bool alphaBlending = m_CreationInfo.GetFlag(MF_AlphaBlending);
		pDriver->SetRenderState(GraphicsDriver::RS_AlphaBlending, alphaBlending);
		if(alphaBlending)
		{
			pDriver->SetBlendFunc(GraphicsDriver::E_BlendFactor(m_CreationInfo.m_SrcBlendFactor), GraphicsDriver::E_BlendFactor(m_CreationInfo.m_DestBlendFactor));
		}

		pDriver->SetCullMode(GraphicsDriver::E_CullMode(m_CreationInfo.m_CullMode));

		pDriver->SetRenderState(GraphicsDriver::RS_DepthTesting, m_CreationInfo.GetFlag(MF_DepthTest));
		pDriver->SetRenderState(GraphicsDriver::RS_DepthWriting, m_CreationInfo.GetFlag(MF_DepthWrite));

		if(Shader* pShader = m_CreationInfo.m_Shader.Get())
		{
			pShader->Begin();
		}
	}

	//! ends rendering with this material
	void Material::End()
	{
		if(Texture* pTexture = m_aTextures.size() ? m_aTextures[0].Get() : NULL)
		{
			pTexture->End();
		}

		if(Shader* pShader = m_CreationInfo.m_Shader.Get())
		{
			pShader->End();
		}
	}
}

