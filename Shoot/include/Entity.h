/* 

Amine Rehioui
Created: February 22th 2010

*/

#ifndef _ENTITY_H_INCLUDED_
#define _ENTITY_H_INCLUDED_

namespace tinyxml2
{	
	// forwards
	class XMLDocument;
}

namespace shoot
{	
	// forwards
	class Context;

	//! Base class for all game entities	
	class Entity : public Object
	{
	public:

		DECLARE_OBJECT(Entity, Object);

		//! Constructor
		Entity();

		//! Destructor
		virtual ~Entity();
	
		//! serializes the entity to/from a PropertyStream
		virtual void Serialize(PropertyStream& stream);

		//! called during the initialization of the entity
		virtual void Init();

		//! called during the update of the entity
		virtual void Update() { }
			
		//! registers the entity and its children for rendering
		virtual void RegisterForRendering() { }

		//! returns the number of children 
		inline u32 GetChildCount() const { return m_aChildren.size(); }

		//! returns a pointer to a child
		/** \param index: index of the wanted child */
		inline Entity* GetChild(int index) const { return m_aChildren[index]; }

		//! returns a child using its ID
		/** \param ID: ID of the wanted child */
		Entity* GetChildByID(u32 ID) const;

		//! returns a child using its name
		/** \param strName: name of the wanted child */
		Entity* GetChildByName(const std::string& strName, bool bRecursive = true) const;

		//! returns the first child by type
		Entity* GetChildByType(const char* strTypeName) const;

		//! returns true if the child is found
		bool HasChild(Entity* pChild) const;

		//! returns children by type
		void GetChildrenByType(const char* strTypeName, std::vector< Handle<Entity> >& aChildren) const;

		//! adds a child to the entity
		virtual void AddChild(Entity* pChild, bool bImmediate = false);

		//! inserts a child into a specific index
		virtual void InsertChild(Entity* pChild, u32 index, bool bImmediate = false);

		//! removes a child from this entity
		virtual void RemoveChild(Entity* pChild, bool bImmediate = false);
			
		//! returns the parent of this entity
		inline Entity* GetParent() const { return m_pParent ? m_pParent : m_pPendingParent; }

		//! returns the first ancestor with a certain type
		template <class T>
		T* GetAncestor() const
		{
			if(m_pParent)
			{
				if(T* pParent = DYNAMIC_CAST(m_pParent, T))
				{
					return pParent;
				}
				else
				{
					return m_pParent->GetAncestor<T>();
				}
			}
			else
			{
				return NULL;
			}
		}

		//! returns the root context
		Context* GetRootContext() const;

		//! returns the root entity
		Entity* GetRoot() const;

		//! returns the number of components 
		inline u32 GetComponentCount() const { return m_aComponents.size(); }

		//! returns a pointer to a component
		/** \param index: index of the wanted component */
		inline Component* GetComponent(int index) const { return m_aComponents[index]; }

		//! returns a pointer to a component		
		Component* GetComponent(const char* strName) const;

		//! gets a component by type
		Component* GetComponentByType(const char* strTypeName) const;

		//! gets a component by type
		template <class T> T* GetComponent() { return static_cast<T*>(GetComponentByType(T::GetStaticClassName())); }

		//! adds a component to the entity
		virtual void AddComponent(Component* pComponent, bool bImmediate = false);

		//! removes a component from this entity
		virtual void RemoveComponent(Component* pComponent, bool bImmediate = false);

		//! gets components by type
		template <class T>
		void GetComponents(std::vector<T*>& components)
		{
			for(u32 i=0; i<m_aComponents.size(); ++i)
			{
				if(T* pComponent = DYNAMIC_CAST(m_aComponents[i].Get(), T))
				{
					components.push_back(pComponent);
				}
			}
		}

		//! returns if this entity is enabled or not.
		/** Disabled entities are not updated */
		bool IsEnabled() const { return m_bEnabled; }

		//! enables / disables this entity
		void SetEnabled(bool bEnabled) { m_bEnabled = bEnabled; }

		//! returns the visibility status of this entity
		bool IsVisible() const { return m_bVisible; }

		//! sets the visibility status of this entity
		void SetVisible(bool bVisible) { m_bVisible = bVisible; }

		//! Loads the entity and its hierarchy from an XML
		void LoadFromXML(const char* strPath, Entity* pParent = NULL);

		//! Loads the object from XML
		virtual void LoadFromXML(tinyxml2::XMLElement* pRoot);

		//! returns the initialization status
		bool IsInitialized() const { return m_bInitialized; }

		//! Saves the object to XML
		virtual void SaveToXML(const char* strPath);

		//! recursive write to XML
		void WriteEntity(tinyxml2::XMLElement* pXMLElement, Entity* pEntity);

		//! set savable to XML flag
		void SetSavableToXML(bool bSavable) { m_bSavableToXML = bSavable; }

		//! gets savable to XML flag
		bool GetSavableToXML() const { return m_bSavableToXML; }

		//! returns the overriden properties stream
		PropertyStream& GetOverridenProperties() { return m_OverridenProperties; }

		//! clears children and components
		void Clear();

	private:

		//! sets the parent of this entity
		void SetParent(Entity* pParent);

		//! recursive read from XML
		void ReadEntity(tinyxml2::XMLElement* pXMLElement, Entity* pParent);
	
		std::vector< Reference<Entity> > m_aChildren;
		std::vector< Reference<Component> > m_aComponents;

		Entity* m_pParent;
		Entity* m_pPendingParent;
		
	protected:

		bool m_bInitialized;
		bool m_bSavableToXML;

		// properties
		bool m_bEnabled;
		bool m_bVisible;

		PropertyStream m_OverridenProperties;
	};
}

#endif // _ENTITY_H_INCLUDED_

