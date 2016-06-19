/* 

Amine Rehioui
Created: March 21th 2010

*/

#include "Shoot.h"

#include "Image.h"

#include "GeometryProvider.h"

namespace shoot
{
	DEFINE_OBJECT(Image);

	//! Constructor
	Image::Image()
		: m_bTextureTransformDirty(true)
	{
	}	

	//! serializes the entity to/from a PropertyStream
	void Image::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);

		m_bTextureTransformDirty = true;
	}

	//! called during the initialization of the entity
	void Image::Init()
	{
		super::Init();

		UpdateGeometry();		
	}

	//! returns the texture transformation matrix
	Matrix44 Image::GetTextureTransformationMatrix()
	{
		if(m_bTextureTransformDirty)
		{
			m_TextureTransform = Matrix44::Identity;
			m_TextureTransform.SetScale(Vector3::Create(m_bHFlip ? -1.0f : 1.0f, m_bVFlip ? -1.0f : 1.0f, 1.0f));
			m_bTextureTransformDirty = false;
		}
		return m_TextureTransform;
	}

	//! updates the geometry
	void Image::UpdateGeometry()
	{
		if(GraphicComponent* pGraphic = GetComponent<GraphicComponent>())
		{
			Texture* pTexture = pGraphic->GetMaterial() ? pGraphic->GetMaterial()->GetTexture() : NULL;
			if(pTexture)
			{
				m_BoundingBox.Set(Vector2(0.0f, 0.0f), pTexture->GetSize());
				Vector2 vUV(pTexture->GetSize().X/pTexture->GetHardwareSize().X, pTexture->GetSize().Y/pTexture->GetHardwareSize().Y);
				pGraphic->SetVertexBuffer(GeometryProvider::Instance()->Get2DQuad(m_BoundingBox.Size(), vUV));

				// force no mipmaps
				pTexture->SetHasMipMaps(false);
				pGraphic->GetMaterial()->SetTexture(0, pTexture);
			}
		}

		m_bTextureTransformDirty = true;
	}
}

