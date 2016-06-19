/* 

Amine Rehioui
Created: March 27h 2012

*/

#include "Shoot.h"

#include "MaterialProvider.h"



namespace shoot
{
	//! constructor
	MaterialProvider::MaterialProvider()
	{
	}

	//! destructor
	MaterialProvider::~MaterialProvider()
	{
	}

	//! gets a material using creation info. Attempts to match with an existing instance.
	Material* MaterialProvider::GetMaterial(const Material::CreationInfo& info)
	{
		for(std::list< Handle<Material> >::iterator it = m_lMaterials.begin(); it != m_lMaterials.end(); )
		{
			if((*it).IsValid())
			{
				if((*it)->GetCreationInfo() == info)
				{
					return (*it);
				}
				++it;
			}
			else
			{
				it = m_lMaterials.erase(it);
			}
		}
		Material* pMaterial = snew Material(info);
		m_lMaterials.push_back(Handle<Material>(pMaterial));
		return pMaterial;
	}
}

