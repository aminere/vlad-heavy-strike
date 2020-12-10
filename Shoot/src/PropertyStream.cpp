/* 

Amine Rehioui
Created: February 24th 2010

*/

#include "Shoot.h"

#include "PropertyStream.h"

#include "tinyxml2.h"

#include "MaterialProvider.h"

namespace shoot
{
	//! Destructor
	PropertyStream::~PropertyStream()
	{
		Clear();	
	}

	//! reads/write from a property from this stream
	/** \return: in read mode, returns true if the property changed, false if not
				 in write mode, always returns false.
	*/
	bool PropertyStream::Serialize(u32 eType, const std::string& strName, void* pValue, const void *pParams /*= NULL*/)
	{
		SHOOT_ASSERT(eType != PT_Array, "Use SerializeArray for serializing arrays");
		SHOOT_ASSERT(eType != PT_Reference, "Use SerializeReference for serializing references");

		bool bChanged = false;
		switch(m_eMode)
		{
		case SM_Read:

			// check if this property was marked for update
			if(m_strPropertyToUpdate.length() == 0
			|| m_strPropertyToUpdate == strName)
			{
				IProperty* pProperty = GetProperty(strName);
				if(pProperty)
				{
					if(pProperty->GetType() == PT_Struct)
					{
						StructProperty* pStructProperty = static_cast<StructProperty*>(pProperty);
						pStructProperty->GetStream().SetMode(SM_Read);
						ISerializableStruct* pStruct = static_cast<ISerializableStruct*>(pValue);		
						pStruct->Serialize(pStructProperty->GetStream());
					}
					else
					{
						if(pProperty->GetType() == PT_Link)
						{
							if(m_pIDMap_OriginalToCopy)
							{
								Link<Object> link;
								pProperty->GetData(&link);
								if(m_pIDMap_OriginalToCopy->find(link.GetObjectID()) != m_pIDMap_OriginalToCopy->end())
								{
									static_cast<ILink*>(pValue)->SetObjectID((*m_pIDMap_OriginalToCopy)[link.GetObjectID()]);									
									bChanged = true;
									goto end;
								}
							}
						}
						
						bChanged = pProperty->GetData(pValue);
						goto end;
					}

					bChanged =  true;
					goto end;
				}
			}
			break;

		case SM_Write:
			{
				IProperty* pProperty = IProperty::CreateFromType(E_PropertyType(eType));
				pProperty->SetName(strName);
				pProperty->SetParams(pParams);

				if(pProperty->GetType() == PT_Struct)
				{
					StructProperty* pStructProperty = static_cast<StructProperty*>(pProperty);
					pStructProperty->GetStream().SetMode(SM_Write);
					ISerializableStruct* pStruct = static_cast<ISerializableStruct*>(pValue);		
					pStruct->Serialize(pStructProperty->GetStream());
				}
				else
				{
					pProperty->SetData(pValue);	
				}

				m_aProperties.push_back(pProperty);
			}
			break;

		default:
			SHOOT_ASSERT(false, "Invalid E_SerializationMode");
		}

		end:
		if(eType == PT_Enum)
		{
			EnumProperty::Params* _pParams = static_cast<EnumProperty::Params*>(const_cast<void*>(pParams));
			sdelete(_pParams);
		}

		return bChanged;
	}

	//! returns the property with the specified name
	IProperty* PropertyStream::GetProperty(const std::string& strName) const
	{
		for(u32 i=0; i<m_aProperties.size(); ++i)
		{
			IProperty* pProperty = m_aProperties[i];
			if(pProperty->GetName() == strName)
			{
				return pProperty;
			}
		}
		return 0;
	}

	//! adds a property to the stream
	void PropertyStream::AddProperty(IProperty* pProperty)
	{
		std::vector<IProperty*>::iterator it = std::find(m_aProperties.begin(), m_aProperties.end(), pProperty);
		SHOOT_ASSERT(it == m_aProperties.end(), "Trying to add a property twice");
		m_aProperties.push_back(pProperty);
	}

	//! removes a propety from the stream
	void PropertyStream::RemoveProperty(IProperty* pProperty)
	{
		std::vector<IProperty*>::iterator it = std::find(m_aProperties.begin(), m_aProperties.end(), pProperty);
		SHOOT_ASSERT(it != m_aProperties.end(), "Trying to remove unexisting property");
		m_aProperties.erase(it);
		sdelete(pProperty);
	}

	//! fills this stream from xml element
	void PropertyStream::ReadFromXML(tinyxml2::XMLElement* pXMLElement, ArrayProperty* pParentArrayProperty /*= NULL*/)	
	{		
		tinyxml2::XMLElement* pChild = pXMLElement->FirstChildElement();
		while(pChild)
		{
			// get the property type name
			std::string strPropertyTypeName = pChild->Value();
			IProperty* pProperty = IProperty::CreateFromTypeName(strPropertyTypeName);

			// get the property name
			const char* strPropertyName = pChild->Attribute("Name");
			if(strPropertyName)
			{
				pProperty->SetName(std::string(strPropertyName));
			}

			switch(pProperty->GetType())
			{
			case PT_Array: 
				ReadFromXML(pChild, static_cast<ArrayProperty*>(pProperty));
				break;

			case PT_Reference:
				{
					ReferenceProperty* pRefProperty = static_cast<ReferenceProperty*>(pProperty);

					// read the class names
					const char* strClassName = pChild->Attribute("ClassName");
					const char* strTemplatePath = pChild->Attribute("TemplatePath");
					SHOOT_ASSERT(strClassName || strTemplatePath, "property missing both ClassName and TemplatePath");
					if(strClassName)
					{
						pRefProperty->SetClassName(strClassName);
						pRefProperty->GetStream().ReadFromXML(pChild);	
					}
					else
					{
						pRefProperty->SetTemplatePath(strTemplatePath);
					}
				}				
				break;

			case PT_Struct:
				{
					StructProperty* pStructProperty = static_cast<StructProperty*>(pProperty);
					pStructProperty->GetStream().ReadFromXML(pChild);
				}
				break;

			default:
				{
					// get the property value
					const char* pStrPropertyValue = pChild->Attribute("Value");
					SHOOT_ASSERT(pStrPropertyValue, "property missing Value attribute");					
					pProperty->SetData(std::string(pStrPropertyValue));
				}
			}
			
			// add the property to the stream
			if(pParentArrayProperty)
			{				
				if(pParentArrayProperty->GetSubType() == PT_Undefined)
				{
					pParentArrayProperty->SetSubType(pProperty->GetType());
				}
				else
				{
					SHOOT_ASSERT(pParentArrayProperty->GetSubType() == pProperty->GetType(), "Can't have multiple types in an array property");
				}
				pParentArrayProperty->GetProperties().push_back(pProperty);
			}			
			else
			{
				m_aProperties.push_back(pProperty);
			}

			pChild = pChild->NextSiblingElement();
		}
	}

	// writes this stream to xml element
	void PropertyStream::WriteToXML(tinyxml2::XMLElement* pXMLElement)	
	{
		for(u32 i=0; i<m_aProperties.size(); ++i)
		{
			WritePropertyToXML(pXMLElement, m_aProperties[i]);			
		}		
	}	

	//! reads/writes an array property from/to this stream
	/** \return: in read mode, returns true if the property changed, false if not
				 in write mode, always returns false.
	*/
	bool PropertyStream::SerializeArray(const std::string& strName, IArray* pArray, u32 eSubType, s32 maxElems /*= -1*/)
	{
		SHOOT_ASSERT(eSubType != PT_Array, "Arrays of arrays not supported. Encapsulate your inner array into a struct to get around this.");

		switch(m_eMode)
		{
		case SM_Read:

			// check if this property was marked for update
			if(m_strPropertyToUpdate.length() == 0
			|| m_strPropertyToUpdate == strName)
			{
				ArrayProperty* pProperty = static_cast<ArrayProperty*>(GetProperty(strName));
				if(pProperty)
				{
					// update existing elements					
					for(auto i=0; i<pArray->GetSize() && i<pProperty->GetProperties().size(); ++i)
					{
						IProperty* pSubProperty = pProperty->GetProperties()[i];
						SHOOT_ASSERT(pSubProperty->GetType() == eSubType, "Actual sub property type differs from expected type");
						void* pElement = pArray->GetPtr(i);
						FillArrayElement(pElement, pSubProperty);						
					}

					// attempt to add new elements
					for(auto i=pArray->GetSize(); i<pProperty->GetProperties().size(); ++i)
					{
						IProperty* pSubProperty = pProperty->GetProperties()[i];
						SHOOT_ASSERT(pSubProperty->GetType() == eSubType, "Actual sub property type differs from expected type");
						if(maxElems < 0
						|| pArray->GetSize() < u32(maxElems))
						{
							void* pElement = pArray->Alloc();
							FillArrayElement(pElement, pSubProperty);							
							pArray->Add(pElement);
						}
						else
						{
							SHOOT_WARNING(0, "SerializeArray can't add more elements to this array");
						}
					}

					// attempt to remove elements
					for(auto i=pProperty->GetProperties().size(); i<pArray->GetSize(); ++i)
					{
						pArray->Delete(i);
						--i;
					}

					return true;
				}
			}
			break;

		case SM_Write:
			{
				ArrayProperty* pProperty = static_cast<ArrayProperty*>(IProperty::CreateFromType(PT_Array));
				pProperty->SetName(strName);
				pProperty->SetSubType(E_PropertyType(eSubType));
				pProperty->SetMaxElems(maxElems);
				pProperty->SetArray(pArray);

				for(u32 i=0; i<pArray->GetSize(); ++i)
				{
					IProperty* pSubProperty = IProperty::CreateFromType(E_PropertyType(eSubType));
					void* pElement = pArray->GetPtr(i);

					if(eSubType == PT_Struct)
					{
						StructProperty* pStructProperty = static_cast<StructProperty*>(pSubProperty);						
						ISerializableStruct* pStruct = static_cast<ISerializableStruct*>(pElement);
						pStruct->Serialize(pStructProperty->GetStream());
					}
					else if(eSubType == PT_Reference)
					{
						IReference* pReference = static_cast<IReference*>(pElement);
						ReferenceProperty* pRefProperty = static_cast<ReferenceProperty*>(pSubProperty);
						FillPropertyFromReference(pRefProperty, pReference);						
					}
					else
					{
						pSubProperty->SetData(pElement);
					}									

					pProperty->GetProperties().push_back(pSubProperty);
				}

				m_aProperties.push_back(pProperty);
			}
			break;		
		}

		return false;
	}

	//! reads/writes a reference property from/to this stream
	/** \return: in read mode, returns true if the property changed, false if not.
				 in write mode, always returns false 
	*/
	bool PropertyStream::SerializeReference(const std::string& strName, IReference* pReference)
	{
		switch(m_eMode)
		{
		case SM_Read:
			// check if this property was marked for update
			if(m_strPropertyToUpdate.length() == 0
			|| m_strPropertyToUpdate == strName)
			{
				IProperty* pProperty = GetProperty(strName);
				if(pProperty && pProperty->GetType() == PT_Reference)
				{
					return FillReferenceFromProperty(pReference, static_cast<ReferenceProperty*>(pProperty));					
				}
			}
			break;

		case SM_Write:
			{
				ReferenceProperty* pProperty = static_cast<ReferenceProperty*>(IProperty::CreateFromType(PT_Reference));
				pProperty->SetName(strName);
				FillPropertyFromReference(pProperty, pReference);
				m_aProperties.push_back(pProperty);
			}
			break;
		}
		return false;
	}

	//! explicitly clear this stream
	void PropertyStream::Clear()
	{
		for(u32 i=0; i<m_aProperties.size(); ++i)
		{
			IProperty* pProperty = m_aProperties[i];
			delete pProperty;
		}
		m_aProperties.clear();
	}

	//! writes a property to XML
	void PropertyStream::WritePropertyToXML(tinyxml2::XMLElement* pXMLElement, IProperty* pProperty, bool bWriteName /*= true*/)
	{		
		tinyxml2::XMLElement* pElement = pXMLElement->GetDocument()->NewElement(pProperty->GetClassName().c_str());

		if(bWriteName)
		{
			pElement->SetAttribute("Name", pProperty->GetName().c_str());
		}
		
		switch(pProperty->GetType())
		{
		case PT_Array:
			{				
				tinyxml2::XMLElement* pXMLArray = pXMLElement->InsertEndChild(pElement)->ToElement();				
				ArrayProperty* pArrayProperty = static_cast<ArrayProperty*>(pProperty);
				for(u32 i=0; i<pArrayProperty->GetProperties().size(); ++i)
				{
					IProperty* pSubProperty = pArrayProperty->GetProperties()[i];
					WritePropertyToXML(pXMLArray, pSubProperty, false); // don't write the name for array sub properties
				}	
			}
			break;

		case PT_Reference:
			{
				ReferenceProperty* pRefProperty = static_cast<ReferenceProperty*>(pProperty);
				if(pRefProperty->GetTemplatePath().empty())
				{
					pElement->SetAttribute("ClassName", pRefProperty->GetClassName().c_str());
					tinyxml2::XMLElement* pXMLRef = pXMLElement->InsertEndChild(pElement)->ToElement();				
					pRefProperty->GetStream().WriteToXML(pXMLRef);
				}
				else
				{
					pElement->SetAttribute("TemplatePath", pRefProperty->GetTemplatePath().c_str());
					pXMLElement->InsertEndChild(pElement);
				}				
			}
			break;

		case PT_Struct:
			{
				tinyxml2::XMLElement* pXMLStruct = pXMLElement->InsertEndChild(pElement)->ToElement();
				StructProperty* pStructProperty = static_cast<StructProperty*>(pProperty);
				pStructProperty->GetStream().WriteToXML(pXMLStruct);
			}
			break;

		default:
			{
				pElement->SetAttribute("Value", pProperty->GetString().c_str());
				pXMLElement->InsertEndChild(pElement);
			}
		}		
	}

	//! fills a user reference from a ReferenceProperty
	bool PropertyStream::FillReferenceFromProperty(IReference* pReference, ReferenceProperty* pProperty)
	{
		bool bFromTemplate = !pProperty->GetTemplatePath().empty();
		if(pProperty->GetStream().GetNumProperties() > 0 || bFromTemplate)
		{
			Object* pObject = pProperty->GetRefInterface() ? pProperty->GetRefInterface()->Get() : NULL;
			if(!pObject || m_bUsedForObjectCopy)
			{
				if(bFromTemplate)
				{
					if(!pObject)
					{
						std::string strTemplatePath = pProperty->GetTemplatePath();
						pObject = ObjectManager::Instance()->Get(strTemplatePath.c_str(), true);
					}
				}
				else
				{
					// hack because materials must only be created through MaterialProvider
					// TODO find a better way
					if(std::string(pProperty->GetClassName()) == Material::GetStaticClassName())
					{
						Material::CreationInfo materialInfo;
						E_SerializationMode eMode = pProperty->GetStream().GetMode();
						pProperty->GetStream().SetMode(SM_Read);
						pProperty->GetStream().Serialize(PT_Struct, "CreationInfo", &materialInfo);
						pProperty->GetStream().SetMode(eMode);
						pObject = MaterialProvider::Instance()->GetMaterial(materialInfo);
					}
					else
					{
						pObject = ObjectFactory::Instance()->Create(pProperty->GetClassName());
					}					
				}
			}
			
			bool bChanged = (pReference->Get() != pObject);
			pReference->SetObject(pObject);
			if(pProperty->GetStream().GetNumProperties() > 0)
			{
				pProperty->GetStream().SetMode(SM_Read);
				pProperty->GetStream().SetIDMap_OriginalToCopy(m_pIDMap_OriginalToCopy);
				pProperty->GetStream().m_bUsedForObjectCopy = m_bUsedForObjectCopy;

				u32 ID = pObject->GetID();
				pObject->Serialize(pProperty->GetStream());

				if(m_bUsedForObjectCopy)
				{
					// use the ID from the new instance
					pObject->SetID(ID);
				}
			}
			return bChanged;
		}
		return false;
	}

	//! fills a ReferenceProperty from a user reference
	void PropertyStream::FillPropertyFromReference(ReferenceProperty* pProperty, IReference* pReference)
	{
		pProperty->SetRefInterface(pReference);
		Object* pObject = pReference->Get();
		if(pObject)
		{
			std::string strTemplatePath = pObject->GetTemplatePath();
			if(!strTemplatePath.empty())
			{
				pProperty->SetTemplatePath(strTemplatePath);
			}
			else
			{
				pProperty->SetClassName(pObject->GetClassName());
			}

			pProperty->GetStream().SetMode(SM_Write);
			pObject->Serialize(pProperty->GetStream());
		}						
		else
		{
			pProperty->SetClassName(pReference->GetClassName());
			pProperty->SetTemplatePath("");
		}
	}

	//! fills an array element from a property
	void PropertyStream::FillArrayElement(void* pElement, IProperty* pProperty)
	{
		if(pProperty->GetType() == PT_Struct)
		{
			StructProperty* pStructProperty = static_cast<StructProperty*>(pProperty);
			pStructProperty->GetStream().SetMode(SM_Read);
			ISerializableStruct* pStruct = static_cast<ISerializableStruct*>(pElement);		
			pStruct->Serialize(pStructProperty->GetStream());
		}
		else if(pProperty->GetType() == PT_Reference)
		{
			IReference* pReference = static_cast<IReference*>(pElement);
			ReferenceProperty* pRefProperty = static_cast<ReferenceProperty*>(pProperty);
			FillReferenceFromProperty(pReference, pRefProperty);								
		}
		else
		{
			pProperty->GetData(pElement);
		}
	}

	//! assignment operator
	PropertyStream& PropertyStream::operator = (const PropertyStream& other)
	{
		Clear();
		for(u32 i=0; i<other.GetNumProperties(); ++i)
		{
			IProperty* pPropertyCopy = other.GetProperty(i)->Copy();
			m_aProperties.push_back(pPropertyCopy);
		}
		return *this;
	}
}

