/* 

Amine Rehioui
Created: November 19th 2011

*/

#ifndef _RESOURCE_MANAGER_H_INCLUDED_
#define _RESOURCE_MANAGER_H_INCLUDED_

#include "Resource.h"

namespace shoot
{
	//! Resource manager class
	class ResourceManager : public Singleton<ResourceManager>
	{
		DECLARE_SINGLETON(ResourceManager);

	public:

		//! returns a resource from a path
		template <class T>
		T* GetResource(const char* strPath)
		{
			Resource* pBaseResource = GetResource(strPath, T::TypeID);
			if(pBaseResource)
			{
				T* pResource = DYNAMIC_CAST(pBaseResource, T);
				SHOOT_ASSERT(pResource, "Trying to load a resource of type '%s' into an instance of type '%s'", pBaseResource->GetClassName(), T::GetStaticClassName());
				return pResource;
			}
			else
			{
				return NULL;
			}
		}

		//! request a resource from a path
		void RequestReloadResource(const char* strPath);

		//! updates the resource manager
		void Update();

		//! reload all resources
		void ReloadResources();

		//! called when a resource is created through ObjectManager::LoadObject
		void OnResourceCreated(Resource* pResource);

		//! called when a resource is destroyed
		void OnResourceDestroyed(Resource* pResource);
		
	private:

		//! returns a resource from a path
		Resource* GetResource(const char* strPath, u32 typeID);

		//! reloads a resource from a path
		void ReloadResource(const char* strPath);

		typedef std::map<std::string, Resource*> ResourceMap;

		ResourceMap m_ResourceMap;
		std::vector<std::string> m_ResourcesToReload;
		f32 m_ResourceReloadDelay;
		Mutex* m_pMutex;
	};
}

#endif // _RESOURCE_MANAGER_H_INCLUDED_

