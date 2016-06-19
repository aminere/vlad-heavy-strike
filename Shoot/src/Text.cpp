/* 

Amine Rehioui
Created: April 21th 2010

*/

#include "Shoot.h"

#include "Text.h"

#include "MaterialProvider.h"

#include "GeometryProvider.h"

#include "TextVisitor.h"

namespace shoot
{
	DEFINE_OBJECT(Text);

	// Alignement literals
	const char* const g_strAlignementLiterals[] =
	{
		"Left",
		"Center",
		"Right",
		0
	};

	//! constructor
	Text::Text()
		: m_bDirty(true)
		, m_DesiredVertexCount(-1)
		// properties
		, m_Size(512, 32)
		, m_eAlignement(A_Left)
		, m_LineSpacing(32)
	{
	}

	//! serializes the entity to/from a PropertyStream
	void Text::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);

		std::string strText = m_strText;
		stream.Serialize(PT_String, "Text", &strText);
		stream.Serialize(PT_Size, "Size", &m_Size);
		stream.Serialize(PT_Enum, "Alignement", &m_eAlignement, ENUM_PARAMS1(g_strAlignementLiterals));
		stream.Serialize(PT_UInt, "LineSpacing", &m_LineSpacing);
		stream.SerializeReference("Font", &m_Font);
		stream.Serialize(PT_Color, "Color", &m_Color);

		if(IsInitialized() 
		&& (stream.GetMode() == SM_Read)
		&& m_Font.IsValid())
		{
			UpdateGeometry(strText, m_Color);
		}

		m_strText = strText;
	}

	//! called during the initialization of the entity
	void Text::Init()
	{
		SHOOT_ASSERT(m_Font.IsValid(), "Creating text with no font");

		// if no material exists, create it
		Material::CreationInfo materialInfo;
		Material::CreationInfo::TextureInfo* pInfo = snew Material::CreationInfo::TextureInfo();
		pInfo->m_Path = m_Font->GetTexturePath();
		pInfo->m_bMipMaps = false;
		materialInfo.m_TextureInfos.Add(pInfo);
		materialInfo.SetFlag(Material::MF_AlphaBlending, true);
		Material* pMaterial = MaterialProvider::Instance()->GetMaterial(materialInfo);

		if(!GetComponent<GraphicComponent>())
		{
			AddComponent(snew GraphicComponent(), true);
			GetComponent<GraphicComponent>()->SetRenderingPass(GraphicComponent::RP_2D);
		}
		GetComponent<GraphicComponent>()->SetMaterial(pMaterial);		
		
		SHOOT_ASSERT(m_Font->GetTextureSize() == pMaterial->GetTexture()->GetSize(), "Font size mismatch");

		UpdateGeometry(m_strText, m_Color);

		super::Init();
	}	

	//! registers the entity and its children for rendering
	
	void Text::RegisterForRendering()
	{
		if(!m_strText.empty())
		{
			PreRender();
			EntityRenderer::Instance()->RegisterEntity(this);
		}
	}	

	//! Get the local axis-aligned bounding box
	AABBox2D Text::GetBoundingBox() const
	{
		AABBox2D box(Vector2(0.0f, 0.0f),
					 Vector2(f32(m_Size.Width), f32(m_Size.Height)));
		return box;
	}

	//! Reloads the image given an image path
	void Text::SetFont(std::string strPath)
	{
		m_Font = ResourceManager::Instance()->GetResource<Font>(strPath.c_str());

		Init();
	}

	//! sets the text
	void Text::SetText(const char* strText)
	{ 
		if(m_strText != strText)
		{
			m_strText = strText;
			m_bDirty = true;
		}		
	}

	//! sets the color
	void Text::SetColor(const Color& color)
	{
		m_Color = color;
		m_bDirty = true;
	}

	//! pre-rendering
	void Text::PreRender()
	{
		if(m_bDirty)
		{
			UpdateGeometry(m_strText, m_Color);
			m_bDirty = false;
		}

		VertexBuffer* pVB = GetComponent<GraphicComponent>()->GetVertexBuffer();
		pVB->SetNumVertices(m_DesiredVertexCount >= 0 ? m_DesiredVertexCount : pVB->GetMaxVertices());
	}

	//! updates the geometry
	void Text::UpdateGeometry(const std::string& text, const Color& color)
	{
		GraphicComponent* pGraphic = GetComponent<GraphicComponent>();
		VertexBuffer* pVertexBuffer = pGraphic->GetVertexBuffer();

		if(pVertexBuffer)
		{
			GeometryProvider::Instance()->UpdateTextVertexBuffer(pVertexBuffer, m_Font, text, m_Size, m_eAlignement, m_LineSpacing, color);
			pVertexBuffer->SetDirty(true);
		}
		else
		{
			pGraphic->SetVertexBuffer(GeometryProvider::Instance()->CreateTextVertexBuffer(m_Font, text, m_Size, m_eAlignement, m_LineSpacing, color));
		}
	}
}

