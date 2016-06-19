/* 

Amine Rehioui
Created: July 10th 2010

*/

#include "Shoot.h"

#include "ResourceLoader.h"

#include "TextureLoader.h"

#include "Shader.h"

#include "tinyxml2.h"

#include "Utils.h"

#include "Mutex.h"

namespace shoot
{
	//! Constructor
	ResourceManager::ResourceManager()
		: m_ResourceReloadDelay(0.0f)
	{
		m_pMutex = Mutex::Create();
	}

	//! Destructor
	ResourceManager::~ResourceManager()
	{
		sdelete(m_pMutex);
	}

	//! returns a texture from a texture path
	Resource* ResourceManager::GetResource(const char* strPath, u32 typeID)
	{
		// check if resource has already been created		
		{
			//Mutex::_Lock lock(m_pMutex);
			ResourceMap::iterator it = m_ResourceMap.find(std::string(strPath));
			if(it != m_ResourceMap.end())
			{
				Resource* pResource = (*it).second;				
				return pResource;
			}
		}		

		// pick appropriate loader depending on file extension
		std::string extension = Utils::GetFileExtension(strPath);
		ResourceLoader* pLoader = NULL;

		if(extension == "png")// || extension == "jpg" || extension == "tga" || extension == "bmp")
		{
			//! LocalTextureLoader
			class LocalTextureLoader : public ResourceLoader
			{
			public:
				Resource* Load(const char* strPath)
				{
					return GraphicsDriver::Instance()->CreateTexture(strPath);
				}
			};
			pLoader = snew LocalTextureLoader();
		}		
		else if(extension == "xml")
		{
			//! ObjectLoader
			class ObjectLoader : public ResourceLoader
			{
			public:
				Resource* Load(const char* strPath)
				{
					Object* pObject = ObjectManager::Instance()->Get(strPath, true);
					SHOOT_ASSERT(pObject->IsA(Resource::TypeID), "Object is not of type Resource");
					return static_cast<Resource*>(pObject);
				}
			};
			
			pLoader = snew ObjectLoader();
		}

		SHOOT_ASSERT(pLoader, "Unsupported Resource Format (%s)", strPath);
		if(pLoader)
		{
			Resource* pResource = pLoader->Load(strPath);
			
			ObjectManager::Instance()->SetTemplatePath(pResource, strPath);
			{
				//Mutex::_Lock lock(m_pMutex);
				m_ResourceMap[strPath] = pResource;
			}	

			delete pLoader;	
			return pResource;
		}
		else
		{
			return NULL;
		}
	}

	//! request a resource from a path
	void ResourceManager::RequestReloadResource(const char* strPath)
	{
		Log.Print("RequestReloadResource: %s\n", strPath);
		if(std::find(m_ResourcesToReload.begin(), m_ResourcesToReload.end(), strPath) == m_ResourcesToReload.end())
		{
			m_ResourcesToReload.push_back(strPath);
			m_ResourceReloadDelay = 2.0f;
		}
	}

	//! updates the resource manager
	void ResourceManager::Update()
	{
		if(m_ResourceReloadDelay > 0.0f)
		{
			m_ResourceReloadDelay -= g_fDeltaTime;
		}
		else
		{
			for(u32 i=0; i<m_ResourcesToReload.size(); ++i)
			{
				ReloadResource(m_ResourcesToReload[i].c_str());
			}
			m_ResourcesToReload.clear();
		}		
	}

	//! reload all resources
	void ResourceManager::ReloadResources()
	{
		for(ResourceMap::iterator it = m_ResourceMap.begin(); it != m_ResourceMap.end(); ++it)
		{
			(*it).second->ResourceInit();
		}
	}

	//! called when a resource is created through ObjectManager::LoadObject
	void ResourceManager::OnResourceCreated(Resource* pResource)
	{
		std::string strPath = pResource->GetTemplatePath();
		ResourceMap::iterator it = m_ResourceMap.find(strPath);
		if(it != m_ResourceMap.end())
		{
			SHOOT_ASSERT(it->second == pResource, "Can't have multiple resource instances having the same resource path");
		}
		m_ResourceMap[strPath] = pResource;
	}

	//! called when a resource is destroyed
	void ResourceManager::OnResourceDestroyed(Resource* pResource)
	{
		std::string strPath = pResource->GetTemplatePath();
		ResourceMap::iterator it = m_ResourceMap.find(strPath);
		if(it != m_ResourceMap.end())
		{
			m_ResourceMap.erase(it);
		}
	}

	//! reloads a resource from a path
	void ResourceManager::ReloadResource(const char* strPath)
	{
		Resource* pResource = NULL;

		for(ResourceMap::iterator it = m_ResourceMap.begin(); it != m_ResourceMap.end(); ++it)
		{
			if((*it).first == strPath
			|| (*it).second->IsUsingData(strPath))
			{
				pResource = (*it).second;
				break;
			}
		}

		if(pResource)
		{
			Log << "ReloadResource " << strPath << "...\n";

			pResource->ResourceUninit();

			// redo serialization
			if(Utils::GetFileExtension(strPath) == "xml")
			{
				tinyxml2::XMLDocument document;
				SHOOT_ASSERT(!document.LoadFile(strPath), "Could not load file");
				tinyxml2::XMLElement* pRoot = document.FirstChildElement();		
				PropertyStream stream(SM_Read);
				stream.ReadFromXML(pRoot);
				pResource->Serialize(stream);
			}

			pResource->ResourceInit();
		}		
	}
}

