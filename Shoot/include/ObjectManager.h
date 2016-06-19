/* 

Amine Rehioui
Created: April 2nd 2011

*/

#ifndef _OBJECT_MANAGER_H_INCLUDED_
#define _OBJECT_MANAGER_H_INCLUDED_

namespace shoot
{
	// forwards
	class Mutex;

	//! Object manager class
	class ObjectManager : public Singleton<ObjectManager>
	{
		DECLARE_SINGLETON(ObjectManager);

	public:

		//! registers an object within the manager and assigns a random ID to it		
		void RegisterObject(Object* pObject);		

		//! removes an object from the manager
		void UnregisterObject(Object* pObject);
		
		//! returns an object from an XML
		Object* Get(const char* strPath, bool bTryGetExistingInstance = false);

		//! finds an object from an XML path, returns NULL if not found
		Object* FindObject(const char* strPath) const;

		//! create a copy of the object
		Object* CopyObject(Object* pObject);

		//! registers a reference to an object
		void RegisterReference(Object* pObject);

		//! unregisters a reference (to an object)
		void UnregisterReference(Object* pObject);

		//! registers a handle to an object
		void RegisterHandle(Object* pObject, IHandle* pHandle);

		//! unregisters a handle (to an object)
		void UnregisterHandle(Object* pObject, IHandle* pHandle);

		//! Object Handler signature
		typedef void ObjectHandler(void* pListener, Object* pObject);

		//! ObjectListener
		struct ObjectListener
		{
			ObjectListener(void* _pListener, ObjectHandler* _pHandler)
				: pListener(_pListener)
				, pHandler(_pHandler)
			{
			}

			void* pListener;
			ObjectHandler* pHandler;
		};

		//! object container
		struct ObjectContainer
		{
			ObjectContainer() 
				: RefCount(-1)
			{
			}

			s32 RefCount;
			std::string strPath;
			std::list<IHandle*> lHandles;

			bool operator == (const ObjectContainer& other) const
			{
				return (RefCount == other.RefCount) && (strPath == other.strPath);
			}

			bool operator != (const ObjectContainer& other) const
			{
				return !operator == (other);
			}
		};

		//! returns an object container from an object pointer
		ObjectContainer* GetContainer(Object* pObject);

		//! returns an object container from an object pointer
		ObjectContainer* GetContainer(const Object* pObject);

		//! sets an object template path
		void SetTemplatePath(Object* pObject, const std::string& path);

		//! gets an object template path
		const std::string& GetTemplatePath(Object* pObject) const;

		typedef std::map<Object*, ObjectContainer> ObjectMap;

	private:

		//! load an object from a path
		Object* LoadObject(const char* strPath);

		//! create a copy of the object
		Object* InternalCopyObject(Object* pObject, std::map<u32, u32>& IDMap_OriginalToCopy);
		
		//! updates an object ref count
		s32 UpdateRefCount(Object* pObject, s32 amount);

		ObjectMap m_ObjectMap;
		Mutex* m_pMutex;

#ifdef SHOOT_EDITOR
	public:
		ObjectMap& GetObjectMap() { return m_ObjectMap; }
#endif // SHOOT_EDITOR
	};
}

#endif // _OBJECT_MANAGER_H_INCLUDED_

