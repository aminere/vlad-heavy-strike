/* 

Amine Rehioui
Created: April 2nd 2011

*/

#include "Shoot.h"

#include "tinyxml2.h"

#include "Mutex.h"

namespace shoot
{
	//! Constructor
	ObjectManager::ObjectManager() 
	{
		m_pMutex = Mutex::Create();
	}

	//! Destructor
	ObjectManager::~ObjectManager()
	{
		SHOOT_ASSERT(m_ObjectMap.empty(), "Object Leaks Detected");
		sdelete(m_pMutex);
	}

	//! registers an object within the manager and assigns a random ID to it		
	void ObjectManager::RegisterObject(Object* pObject)
	{
		SHOOT_ASSERT(m_ObjectMap.find(pObject) == m_ObjectMap.end(), "Trying to register an object twice");
		m_ObjectMap[pObject].RefCount = 0;

		u32 ID = Random::NextU32();
		pObject->SetID(ID);
	}

	//! removes an object from the manager
	void ObjectManager::UnregisterObject(Object* pObject)
	{
		ObjectContainer& container = m_ObjectMap[pObject];		
		SHOOT_ASSERT(container.RefCount != -1, "Trying to delete an unregistered object");		
		SHOOT_ASSERT(container.RefCount == 0, "Trying to delete an object that is still referenced");
		
		for(std::list<IHandle*>::iterator it = container.lHandles.begin(); it != container.lHandles.end(); ++it)
		{
			(*it)->ClearByObjectManager();
		}
		
		m_ObjectMap.erase(pObject);
	}

	//! returns an object from an XML
	Object* ObjectManager::Get(const char* strPath, bool bTryGetExistingInstance /*= false*/)
	{
		if(bTryGetExistingInstance)
		{
			if(Object* pObject = FindObject(strPath))
			{
				return pObject;
			}
		}

		return LoadObject(strPath);
	}

	//! finds an object from an XML path, returns NULL if not found
	Object* ObjectManager::FindObject(const char* strPath) const
	{
		for(ObjectMap::const_iterator it = m_ObjectMap.begin(); it != m_ObjectMap.end(); ++it)
		{
			if((*it).second.strPath == strPath)
			{
				return (*it).first;
			}
		}

		return NULL;
	}	

	//! load an object from a path
	Object* ObjectManager::LoadObject(const char* strPath)
	{
		tinyxml2::XMLDocument document;
		if(document.LoadFile(strPath) == tinyxml2::XML_SUCCESS)
		{
			tinyxml2::XMLElement* pRoot = document.FirstChildElement();	
			const char* strClassName = pRoot->Value();

			Object* pObject = ObjectFactory::Instance()->Create(strClassName);
			pObject->LoadFromXML(pRoot);

			SetTemplatePath(pObject, strPath);

			if(Resource* pResource = DYNAMIC_CAST(pObject, Resource))
			{
				ResourceManager::Instance()->OnResourceCreated(pResource);
			}

			return pObject;
		}
		return NULL;
	}

	//! registers a reference to an object
	void ObjectManager::RegisterReference(Object* pObject)
	{
		UpdateRefCount(pObject, 1);
	}

	//! unregisters a reference (to an object)
	void ObjectManager::UnregisterReference(Object* pObject)
	{
		s32 refCount = UpdateRefCount(pObject, -1);		
		SHOOT_ASSERT(refCount >= 0, "Negative RefCount detected");
		if(refCount == 0)
		{
			sdelete(pObject);
		}
	}

	//! registers a handle to an object
	void ObjectManager::RegisterHandle(Object* pObject, IHandle* pHandle)
	{
		//Mutex::_Lock lock(m_pMutex);
		ObjectContainer* pContainer = GetContainer(pObject);
		SHOOT_ASSERT(std::find(pContainer->lHandles.begin(), pContainer->lHandles.end(), pHandle) == pContainer->lHandles.end(), "Duplicate Object Handle detected");
		pContainer->lHandles.push_back(pHandle);		
	}

	//! unregisters a handle (to an object)
	void ObjectManager::UnregisterHandle(Object* pObject, IHandle* pHandle)
	{
		//Mutex::_Lock lock(m_pMutex);
		ObjectContainer* pContainer = GetContainer(pObject);
		std::list<IHandle*>::iterator it = std::find(pContainer->lHandles.begin(), pContainer->lHandles.end(), pHandle);
		SHOOT_ASSERT(it != pContainer->lHandles.end(), "Object '%s' Handle not found", pObject->GetClassName());
		pContainer->lHandles.erase(it);		
	}

	//! returns an object container from an object pointer
	ObjectManager::ObjectContainer* ObjectManager::GetContainer(Object* pObject)
	{
		ObjectMap::iterator it = m_ObjectMap.find(pObject);
		SHOOT_ASSERT(it != m_ObjectMap.end(), "Objet '%s' container not found", pObject->GetClassName());
		return &(it->second);
	}

	//! returns an object container from an object pointer
	ObjectManager::ObjectContainer* ObjectManager::GetContainer(const Object* pObject)
	{
		return GetContainer(const_cast<Object*>(pObject));
	}

	//! sets an object template path
	void ObjectManager::SetTemplatePath(Object* pObject, const std::string& path)
	{
		//Mutex::_Lock lock(m_pMutex);
		GetContainer(pObject)->strPath = path;
	}

	//! gets an object template path
	const std::string& ObjectManager::GetTemplatePath(Object* pObject) const
	{		
		//Mutex::_Lock lock(m_pMutex);
		const std::string& path = const_cast<ObjectManager*>(this)->GetContainer(pObject)->strPath;
		return path;
	}

	//! updates an object ref count
	s32 ObjectManager::UpdateRefCount(Object* pObject, s32 amount)
	{		
		//Mutex::_Lock lock(m_pMutex);
		s32& refCount = GetContainer(pObject)->RefCount;
		refCount += amount;
		return refCount;		
	}

	//! create a copy of the object
	Object* ObjectManager::CopyObject(Object* pObject)
	{
		std::map<u32, u32> IDMap_OriginalToCopy;
		return InternalCopyObject(pObject, IDMap_OriginalToCopy);
	}

	//! create a copy of the object
	Object* ObjectManager::InternalCopyObject(Object* pObject, std::map<u32, u32>& IDMap_OriginalToCopy)
	{
		Object* pCopy = ObjectFactory::Instance()->Create(pObject->GetClassName());

		// copy all the properties from the source object
		u32 ID = pCopy->GetID();
		PropertyStream stream(SM_Write);
		pObject->Serialize(stream);
		stream.SetMode(SM_Read);
		stream.SetUsedForObjectCopy(true);
		stream.SetIDMap_OriginalToCopy(&IDMap_OriginalToCopy);
		pCopy->Serialize(stream);
		IDMap_OriginalToCopy[pCopy->GetID()] = ID;
		pCopy->SetID(ID);

		// if the object is an entity, copy children and components
		if(Entity* pEntity = DYNAMIC_CAST(pObject, Entity))
		{
			Entity* pEntityCopy = DYNAMIC_CAST(pCopy, Entity);

#ifdef SHOOT_EDITOR
			pEntityCopy->SetSavableToXML(pEntity->GetSavableToXML());

			// copy overriden property statuses
			for(u32 i=0; i<stream.GetNumProperties(); ++i)
			{
				if(pEntity->GetOverridenProperties().GetProperty(stream.GetProperty(i)->GetName()))
				{
					pEntityCopy->GetOverridenProperties().AddProperty(stream.GetProperty(i)->Copy());
				}
			}
#endif // SHOOT_EDITOR

			// copy children
			auto numChildren = pEntity->GetChildCount();
			for(size_t i=0; i<numChildren; ++i)
			{
				Object* pChild = pEntity->GetChild(i);
				Object* pChildCopy = InternalCopyObject(pChild, IDMap_OriginalToCopy);
				pEntityCopy->AddChild(DYNAMIC_CAST(pChildCopy, Entity), true);
			}

			// copy components
			auto numComponents = pEntity->GetComponentCount();
			for(size_t i=0; i<numComponents; ++i)
			{
				Object* pComponent = pEntity->GetComponent(i);
				Object* pComponentCopy = InternalCopyObject(pComponent, IDMap_OriginalToCopy);				
				pEntityCopy->AddComponent(static_cast<Component*>(pComponentCopy), true);
			}
		}

		// copy container path
		{
			//Mutex::_Lock lock(m_pMutex);
			ObjectContainer* pDestContainer = GetContainer(pCopy);
			ObjectContainer* pSrcContainer = GetContainer(pObject);
			pDestContainer->strPath = pSrcContainer->strPath;
		}

		return pCopy;
	}
}

