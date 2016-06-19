/* 

Amine Rehioui
Created: March 27h 2012

*/

#ifndef _MATERIAL_PROVIDER_H_INCLUDED_
#define _MATERIAL_PROVIDER_H_INCLUDED_

#include "Material.h"

namespace shoot
{
	//! Material provider
	class MaterialProvider : public Singleton<MaterialProvider>
	{
		DECLARE_SINGLETON(MaterialProvider);

	public:

		//! gets a material using creation info. Attempts to match with an existing instance.
		Material* GetMaterial(const Material::CreationInfo& info);

	private:

		// material pool
		std::list< Handle<Material> > m_lMaterials;
	};
}

#endif // _MATERIAL_PROVIDER_H_INCLUDED_


