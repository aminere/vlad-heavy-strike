/* 

Amine Rehioui
Created: February 24th 2010

*/

#ifndef _PROPERTY_STREAM_H_INCLUDED_
#define _PROPERTY_STREAM_H_INCLUDED_

// forwards
namespace tinyxml2
{	
	class XMLElement;
	class XMLDocument;
}

namespace shoot
{
	//! specifies if the PropertyStream is to be read from / written to
	enum E_SerializationMode
	{
		SM_Read,
		SM_Write
	};

	class IProperty;
	class IArray;
	class IReference;
	class ArrayProperty;
	class ReferenceProperty;

	//! Class to encapsulate properties and communicate with XML
	class PropertyStream
	{
	public:		

		//! Constructor
		PropertyStream(E_SerializationMode eMode) 
			: m_eMode(eMode)
			, m_bUsedForObjectCopy(false)
			, m_pIDMap_OriginalToCopy(NULL)
		{
		}

		//! Destructor
		~PropertyStream();		

		//! reads/write a property from/to this stream
		/** \return: in read mode, returns true if the property changed, false if not.
					 in write mode, always returns false 
		*/
		bool Serialize(u32 eType, const std::string& strName, void* pValue, const void *pParams = NULL);

		//! reads/writes an array property from/to this stream
		/** \return: in read mode, returns true if the property changed, false if not.
					 in write mode, always returns false 
		*/
		bool SerializeArray(const std::string& strName, IArray* pArray, u32 eSubType, s32 maxElems = -1);

		//! reads/writes a reference property from/to this stream
		/** \return: in read mode, returns true if the property changed, false if not.
					 in write mode, always returns false 
		*/
		bool SerializeReference(const std::string& strName, IReference* pReference);

		//! returns the property with the specified name
		IProperty* GetProperty(const std::string& strName) const;

		//! returns the property at the specified index
		IProperty* GetProperty(u32 index) const { return m_aProperties[index]; }

		//! adds a property to the stream
		void AddProperty(IProperty* pProperty);

		//! removes a propety from the stream
		void RemoveProperty(IProperty* pProperty);

		//! returns the number of properties stored in this stream
		size_t GetNumProperties() const { return m_aProperties.size(); }

		//! fills this stream from an xml element
		void ReadFromXML(tinyxml2::XMLElement* pXMLElement, ArrayProperty* pParentArrayProperty = NULL);

		//! writes this stream to an xml element
		void WriteToXML(tinyxml2::XMLElement* pXMLElement);

		//! changes the serialization mode
		void SetMode(E_SerializationMode eMode) { m_eMode = eMode; }

		//! returns the serialization mode
		E_SerializationMode GetMode() const { return m_eMode; }

		//! marks one property for update
		void SetPropertyToUpdate(const std::string& strName) { m_strPropertyToUpdate = strName; }

		//! explicitly clear this stream
		void Clear();

		//! fills a user reference from a ReferenceProperty
		bool FillReferenceFromProperty(IReference* pReference, ReferenceProperty* pProperty);

		//! fills a ReferenceProperty from a user reference
		void FillPropertyFromReference(ReferenceProperty* pProperty, IReference* pReference);

		//! assignment operator
		PropertyStream& operator = (const PropertyStream& other);

		//! returns the properties array
		std::vector<IProperty*>& GetProperties() { return m_aProperties; }

		//! set this flag when the stream is used for object copy
		inline void SetUsedForObjectCopy(bool bSet) { m_bUsedForObjectCopy = bSet; }

		//! set this flag when the stream is used for object copy
		inline void SetIDMap_OriginalToCopy(std::map<u32, u32>* pMap) { m_pIDMap_OriginalToCopy = pMap; }

	private:

		//! writes a property to XML
		void WritePropertyToXML(tinyxml2::XMLElement* pXMLElement, IProperty* pProperty, bool bWriteName = true);

		//! fills an array element from a property
		void FillArrayElement(void* pElement, IProperty* pProperty);

		std::vector<IProperty*> m_aProperties;

		E_SerializationMode m_eMode;

		std::string m_strPropertyToUpdate;

		bool m_bUsedForObjectCopy;
		std::map<u32, u32>* m_pIDMap_OriginalToCopy;
	};
}

#endif // _PROPERTY_STREAM_H_INCLUDED_