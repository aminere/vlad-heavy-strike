/* 

Amine Rehioui
Created: February 22th 2010

*/

#include "Shoot.h"

#include "tinyxml2.h"

#include "EntityOperation.h"

namespace shoot
{
	DEFINE_OBJECT(Entity);

	//! Constructor
	Entity::Entity() 
		: m_pParent(NULL)
		, m_pPendingParent(NULL)
		, m_bInitialized(false)
		, m_bSavableToXML(false)
		, m_OverridenProperties(SM_Read)
		// properties
		, m_bEnabled(true)
		, m_bVisible(true)
	{			
	}		

	//! Destructor
	Entity::~Entity()
	{
		m_bInitialized = false;
		Clear();
	}

	//! serializes the entity to/from a PropertyStream
	void Entity::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);	

		stream.Serialize(PT_Bool, "Visible", &m_bVisible);		
		stream.Serialize(PT_Bool, "Enabled", &m_bEnabled);
	}

	//! called during the initialization of the entity
	void Entity::Init()
	{
		for(u32 i=0; i<m_aChildren.size(); ++i)
		{
			m_aChildren[i]->Init();
		}

		for(u32 i=0; i<m_aComponents.size(); ++i)
		{
			m_aComponents[i]->Init();
		}

		m_bInitialized = true;
	}

	//! adds a child to the entity
	void Entity::AddChild(Entity* pChild, bool bImmediate /*= false*/)
	{
		if(bImmediate)
		{
			Reference<Entity> child(pChild);

			if(pChild->m_pParent)
			{
				pChild->m_pParent->RemoveChild(pChild, true);
			}

			m_aChildren.push_back(child);
			pChild->SetParent(this);
		}
		else
		{
			pChild->m_pPendingParent = this;
			EntityOperationInfo info(EO_AddChild, this, pChild);
			Engine::Instance()->AddEntityOperation(info);
		}
	}

	//! inserts a child into a specific index
	void Entity::InsertChild(Entity* pChild, u32 index, bool bImmediate /*= false*/)
	{	
		if(bImmediate)
		{
			Reference<Entity> child(pChild);

			if(pChild->m_pParent)
			{
				pChild->m_pParent->RemoveChild(pChild, true);				
			}

			m_aChildren.insert(m_aChildren.begin()+index, child);			
			pChild->SetParent(this);
		}
		else
		{
			pChild->m_pPendingParent = this;
			EntityOperationInfo info(EO_InsertChild, this, pChild, index);
			Engine::Instance()->AddEntityOperation(info);
		}
	}

	//! removes a child from this entity
	void Entity::RemoveChild(Entity* pChild, bool bImmediate /*= true*/)
	{				
		if(bImmediate)
		{
			Reference<Entity> child(pChild);
			std::vector< Reference<Entity> >::iterator it = std::find(m_aChildren.begin(), m_aChildren.end(), child);
			SHOOT_ASSERT(it != m_aChildren.end(), "Trying to remove unexisting child");

			pChild->SetParent(NULL);
			m_aChildren.erase(it);
		}
		else
		{
			EntityOperationInfo info(EO_RemoveChild, this, pChild);
			Engine::Instance()->AddEntityOperation(info);
		}
	}	

	//! returns a child using its ID
	/** \param ID: ID of the wanted child */
	Entity* Entity::GetChildByID(u32 ID) const
	{
		for(u32 i=0; i<m_aChildren.size(); ++i)
		{
			Entity* pChild = m_aChildren[i];
			if(pChild->GetID() == ID)
			{
				return pChild;
			}
		}

		for(u32 i=0; i<m_aChildren.size(); ++i)
		{
			Entity* pGrandChild = m_aChildren[i]->GetChildByID(ID);
			if(pGrandChild)
			{
				return pGrandChild;
			}
		}

		return NULL;
	}

	//! returns a child using its name
	/** \param strName: name of the wanted child */
	Entity* Entity::GetChildByName(const std::string& strName, bool bRecursive /*= true*/) const
	{
		for(u32 i=0; i<m_aChildren.size(); ++i)
		{
			Entity* pChild = m_aChildren[i];
			if(pChild->GetName() == strName)
			{
				return pChild;
			}
		}

		if(bRecursive)
		{
			for(u32 i=0; i<m_aChildren.size(); ++i)
			{
				Entity* pGrandChild = m_aChildren[i]->GetChildByName(strName, true);
				if(pGrandChild)
				{
					return pGrandChild;
				}
			}
		}

		return NULL;
	}

	//! returns the first child by type
	Entity* Entity::GetChildByType(const char* strTypeName) const
	{
		std::string typeName(strTypeName);

		for(u32 i=0; i<m_aChildren.size(); ++i)
		{
			Entity* pChild = m_aChildren[i];
			if(ObjectFactory::Instance()->IsDerived(pChild->GetClassName(), typeName))
			{
				return pChild;
			}
		}

		for(u32 i=0; i<m_aChildren.size(); ++i)
		{
			Entity* pGrandChild = m_aChildren[i]->GetChildByType(strTypeName);
			if(pGrandChild)
			{
				return pGrandChild;
			}
		}

		return NULL;
	}

	//! returns true if the child is found
	bool Entity::HasChild(Entity* pChild) const
	{
		for(u32 i=0; i<m_aChildren.size(); ++i)
		{
			if(m_aChildren[i].Get() == pChild
			|| m_aChildren[i]->HasChild(pChild))
			{
				return true;
			}
		}

		return false;
	}

	//! returns children by type name
	void Entity::GetChildrenByType(const char* strTypeName, std::vector< Handle<Entity> >& aChildren) const
	{
		std::string typeName(strTypeName);

		for(u32 i=0; i<m_aChildren.size(); ++i)
		{
			Entity* pChild = m_aChildren[i];
			if(ObjectFactory::Instance()->IsDerived(pChild->GetClassName(), typeName))
			{
				aChildren.push_back(Handle<Entity>(pChild));
			}
		}

		// check grand children
		for(u32 i=0; i<m_aChildren.size(); ++i)
		{
			m_aChildren[i]->GetChildrenByType(strTypeName, aChildren);
		}
	}

	//! returns the root context
	Context* Entity::GetRootContext() const
	{
		return DYNAMIC_CAST(GetRoot(), Context);
	}

	//! returns the root entity
	Entity* Entity::GetRoot() const
	{
		if(GetParent() == NULL || IsA(Context::TypeID))
		{
			return const_cast<Entity*>(this);
		}
		else
		{
			return GetParent()->GetRoot();
		}
	}

	//! returns a pointer to a component		
	Component* Entity::GetComponent(const char* strName) const
	{
		for(u32 i=0; i<m_aComponents.size(); ++i)
		{
			if(m_aComponents[i]->GetName() == strName)
			{
				return m_aComponents[i];
			}
		}
		return NULL;
	}

	//! returns a pointer to a component by type	
	Component* Entity::GetComponentByType(const char* strTypeName) const
	{
		for(u32 i=0; i<m_aComponents.size(); ++i)
		{
			if(ObjectFactory::Instance()->IsDerived(m_aComponents[i]->GetClassName(), strTypeName))
			{
				return m_aComponents[i];
			}
		}
		return NULL;
	}

	//! adds a component to the entity
	void Entity::AddComponent(Component* pComponent, bool bImmediate /*= true*/)
	{
		if(bImmediate)
		{
			Reference<Component> component(pComponent);
			std::vector< Reference<Component> >::iterator it = std::find(m_aComponents.begin(), m_aComponents.end(), component);
			SHOOT_ASSERT(it == m_aComponents.end(), "Trying to add component twice");
			m_aComponents.push_back(component);
			pComponent->SetParent(this);
		}
		else
		{
			EntityOperationInfo info(EO_AddComponent, this, pComponent);
			Engine::Instance()->AddEntityOperation(info);
		}
	}

	//! removes a component from this entity
	void Entity::RemoveComponent(Component* pComponent, bool bImmediate /*= true*/)
	{		
		if(bImmediate)
		{
			Reference<Component> component(pComponent);
			std::vector< Reference<Component> >::iterator it = std::find(m_aComponents.begin(), m_aComponents.end(), component);
			SHOOT_ASSERT(it != m_aComponents.end(), "Trying to remove unexisting component");
			m_aComponents.erase(it);
		}
		else
		{
			EntityOperationInfo info(EO_RemoveComponent, this, pComponent);
			Engine::Instance()->AddEntityOperation(info);
		}
	}

	//! Loads the entity and its hierarchy from an XML
	void Entity::LoadFromXML(const char* strPath, Entity* pParent /*= NULL*/)
	{
		Clear();
		tinyxml2::XMLDocument document;

		if(!document.LoadFile(strPath))
		{
			ReadEntity(document.FirstChildElement(), pParent);	
		}
		else
		{
			SHOOT_ASSERT(0, "Could not load file");
		}
	}

	//! Loads the object from XML
	void Entity::LoadFromXML(tinyxml2::XMLElement* pRoot)
	{
		Clear();
		ReadEntity(pRoot, NULL);
	}

	//! Saves the entity and its hierarchy to an XML
	void Entity::SaveToXML(const char* strPath)
	{
		tinyxml2::XMLDocument doc;
		tinyxml2::XMLElement* pRoot = doc.NewElement(GetClassName());

		// if saving an entity, clear the template path to make a regular save
		ObjectManager::Instance()->SetTemplatePath(this, "");

		// write the entity
		WriteEntity(pRoot, this);

		// set the template path
		ObjectManager::Instance()->SetTemplatePath(this, strPath);
		
		doc.LinkEndChild(pRoot);
		doc.SaveFile(strPath);
	}

	//! recursive write to XML
	void Entity::WriteEntity(tinyxml2::XMLElement* pXMLElement, Entity* pEntity)
	{
		// write properties
		PropertyStream stream(SM_Write);
		pEntity->Serialize(stream);

		std::string templatePath = pEntity->GetTemplatePath();
		bool bIsTemplateInstance = !templatePath.empty();
		if(bIsTemplateInstance)
		{
			// set template attribute
			pXMLElement->SetAttribute("TemplatePath", templatePath.c_str());

			// for template entities only write overriden properties and the instance ID
			for(u32 i=0; i<stream.GetNumProperties(); ++i)
			{
				IProperty* pProperty = stream.GetProperty(i);
				if(pProperty->GetName() != "ID"
				&& !pEntity->m_OverridenProperties.GetProperty(pProperty->GetName()))
				{
					stream.RemoveProperty(pProperty);
					--i;
				}
			}
		}

		if(stream.GetNumProperties() > 0)
		{
			tinyxml2::XMLElement* pXMLProperties = pXMLElement->GetDocument()->NewElement("Properties");
			stream.WriteToXML(pXMLProperties);
			pXMLElement->LinkEndChild(pXMLProperties);
		}	

		// for now component and children definition/overriding is disabled for template instances
		if(!bIsTemplateInstance)
		{
			// write components
			auto numComponents = pEntity->GetComponentCount();
			if(numComponents > 0)
			{
				tinyxml2::XMLElement* pXMLComponents = pXMLElement->GetDocument()->NewElement("Components");
				PropertyStream stream(SM_Write);
				for(auto i=0; i<numComponents; ++i)
				{
					stream.SerializeReference("", &pEntity->m_aComponents[i]);
				}
				stream.WriteToXML(pXMLComponents);
				pXMLElement->LinkEndChild(pXMLComponents);
			}

			// write children
			for(u32 i=0; i<pEntity->GetChildCount(); ++i)
			{
				Entity* pChild = pEntity->GetChild(i);
				if(pChild->m_bSavableToXML)
				{
					std::string strClassName = pChild->GetClassName();
					tinyxml2::XMLElement* pXMLChild = pXMLElement->GetDocument()->NewElement(strClassName.c_str());
					WriteEntity(pXMLChild, pChild);
					pXMLElement->LinkEndChild(pXMLChild);
				}				
			}
		}
	}

	//! recursive read from XML
	void Entity::ReadEntity(tinyxml2::XMLElement* pXMLElement, Entity* pParent)
	{
		Entity* pEntity = NULL;
		const char* strClassName = pXMLElement->Value();
		const char* strTemplatePath = pXMLElement->Attribute("TemplatePath");

		if(pParent)
		{
			// create entity
			Object* pObject = ObjectFactory::Instance()->Create(strClassName);			
			pEntity = DYNAMIC_CAST(pObject, Entity);
			SHOOT_ASSERT(pEntity, "Cannot create an Entity from Object");
		}
		else
		{
			SHOOT_ASSERT(std::string(GetClassName()) == strClassName, "Trying to load and entity into an entity of incompatible type");
			pEntity = this;
		}		

		pEntity->m_bSavableToXML = true;

		if(strTemplatePath)
		{
			ObjectManager::Instance()->SetTemplatePath(pEntity, strTemplatePath);
			pEntity->LoadFromXML(strTemplatePath);
		}
		
		// read properties
		tinyxml2::XMLElement* pXMLProperties = pXMLElement->FirstChildElement();
		if(pXMLProperties)
		{
			SHOOT_ASSERT(std::string(pXMLProperties->Value()) == std::string("Properties"), "properties tag not found"); 
			PropertyStream stream(SM_Read);
			stream.ReadFromXML(pXMLProperties);

			// if this is a template instance, mark the properties as overriden
			if(strTemplatePath)
			{
				for(u32 i=0; i<stream.GetNumProperties(); ++i)
				{
					if(stream.GetProperty(i)->GetName() != "ID")
					{
						pEntity->m_OverridenProperties.AddProperty(stream.GetProperty(i)->Copy());
					}
				}
			}
			
			pEntity->Serialize(stream);
		}
		else
		{
			SHOOT_ASSERT(strTemplatePath, "properties tag not found on an entity that is not a template instance");
		}

		if(pParent)
		{
			// Add entity after it has been serialized to ensure proper event handling inside of AddChild()
			pParent->AddChild(pEntity, true);
		}

		// read components
		tinyxml2::XMLElement* pXMLChild = pXMLProperties ? pXMLProperties->NextSiblingElement() : NULL;
		if(pXMLChild)
		{
			const char* strChildName = pXMLChild->Value();
			if(std::string(strChildName) == "Components")
			{
				SHOOT_ASSERT(!strTemplatePath, "Entity template instance is not supposed to define components");
				PropertyStream stream(SM_Read);
				stream.ReadFromXML(pXMLChild);
				for(u32 i=0; i<stream.GetNumProperties(); ++i)
				{
					IProperty* pProperty = stream.GetProperty(i);
					SHOOT_ASSERT(pProperty->GetType() == PT_Reference, "Invalid Reference to Component");
					ReferenceProperty* pRefProperty = static_cast<ReferenceProperty*>(pProperty);
					Reference<Component> component;
					stream.FillReferenceFromProperty(&component, pRefProperty);

					Component* pComponent = component.Get();
					if(!pRefProperty->GetTemplatePath().empty())
					{
						// must add a unique component instance per entity, because components can't have multiple parents.
						pComponent = static_cast<Component*>(pComponent->Copy());
					}
					
					pEntity->AddComponent(pComponent, true);
				}
				pXMLChild = pXMLChild->NextSiblingElement();
			}
		}

		// read children		
		while(pXMLChild)
		{
			SHOOT_ASSERT(!strTemplatePath, "Entity template instance is not supposed to define children");
			ReadEntity(pXMLChild, pEntity);
			pXMLChild = pXMLChild->NextSiblingElement();
		}
	}

	//! clears children and components
	void Entity::Clear()
	{
		m_aComponents.clear();
		m_aChildren.clear();
	}

	//! sets the parent of this entity
	void Entity::SetParent(Entity* pParent) 
	{ 
		m_pParent = pParent;
		m_pPendingParent = NULL;
	}
}

