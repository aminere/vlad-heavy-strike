/* 

Amine Rehioui
Created: August 12th 2010

*/

#include "Shoot.h"

#include "SkyBoxEntity.h"

#include "GeometryProvider.h"

namespace shoot
{
	DEFINE_OBJECT(SkyBoxEntity);

	//! constructor
	SkyBoxEntity::SkyBoxEntity()
	{
		m_bAlwaysActive = true;
	}

	//! serializes the entity to/from a PropertyStream
	void SkyBoxEntity::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);
		
		bool bChanged = stream.SerializeReference("CubeMap", &m_CubeMap);

		if(IsInitialized())
		{
			if(bChanged)
			{
				Init();
			}
		}		
	}

	//! called during the initialization of the entity
	void SkyBoxEntity::Init()
	{
		if(m_CubeMap.IsValid())
		{
			GraphicComponent* pGraphic = GetComponent<GraphicComponent>();
			if(!pGraphic)
			{
				pGraphic = snew GraphicComponent();
				pGraphic->SetRenderingPass(GraphicComponent::RP_SkyBox);
				AddComponent(pGraphic, true);
			}

			Material* pMaterial = pGraphic->GetMaterial();
			if(!pMaterial)
			{
				pMaterial = snew Material();
				pMaterial->SetFlag(Material::MF_DepthWrite, false);
				pGraphic->SetMaterial(pMaterial);
			}

			pMaterial->SetTexture(0, m_CubeMap->GetTexture());
			if(!pGraphic->GetVertexBuffer())
			{
				pGraphic->SetVertexBuffer(GeometryProvider::Instance()->GetSkyBoxVertexBuffer(pMaterial->GetTexture()));
			}
		}

		super::Init();
	}	

	//! sets the cube map
	void SkyBoxEntity::SetCubeMap(std::string strPath)
	{
		m_CubeMap = ResourceManager::Instance()->GetResource<CubeMapResource>(strPath.c_str());
		
		Init();
	}	
}


