/* 

Amine Rehioui
Created: April 2nd 2011

*/

#include "Shoot.h"

#include "tinyxml2.h"

namespace shoot
{
	//! static variables initialization
	u32 Object::m_TypeCount = 0;

	DEFINE_OBJECT(Object);

	//! Constructor
	Object::Object()
		: m_ID(0)
	{
		ObjectManager::Instance()->RegisterObject(this);
	}

	//! Destructor
	Object::~Object()
	{
		ObjectManager::Instance()->UnregisterObject(this);
	}

	//! serializes the object to/from a PropertyStream
	void Object::Serialize(PropertyStream& stream)
	{
		stream.Serialize(PT_UInt, "ID", &m_ID);
		stream.Serialize(PT_String, "Name", &m_strName);
	}

	//! Loads the object from XML
	void Object::LoadFromXML(tinyxml2::XMLElement* pRoot)
	{
		PropertyStream stream(SM_Read);
		stream.ReadFromXML(pRoot);
		Serialize(stream);
	}

	//! returns the object's template path
	const std::string& Object::GetTemplatePath() const
	{
		return ObjectManager::Instance()->GetTemplatePath(const_cast<Object*>(this));
	}

	//! create a copy of the object
	Object* Object::Copy() const
	{
		return ObjectManager::Instance()->CopyObject(const_cast<Object*>(this));
	}

	//! Saves the object to XML
	void Object::SaveToXML(const char* strPath)
	{
		tinyxml2::XMLDocument doc;
		tinyxml2::XMLElement* pRoot = doc.NewElement(GetClassName());

		PropertyStream stream(SM_Write);
		Serialize(stream);

		stream.WriteToXML(pRoot);		
		doc.LinkEndChild(pRoot);
		doc.SaveFile(strPath);

		ObjectManager::Instance()->SetTemplatePath(this, strPath);
	}
}
