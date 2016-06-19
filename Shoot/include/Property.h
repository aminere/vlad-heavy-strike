/* 

Amine Rehioui
Created: February 24th 2010

*/

#ifndef _PROPERTY_H_INCLUDED_
#define _PROPERTY_H_INCLUDED_

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Point.h"
#include "AABBox2D.h"
#include "AABBox3D.h"
#include "Size.h"
#include "Color.h"
#include "Range.h"
#include "Array.h"

namespace shoot
{
	// forwards
	class Object;

	//! Property types
	enum E_PropertyType
	{
		PT_Int,
		PT_UInt,
		PT_Float,
		PT_String,
		PT_Bool,
		PT_Vec2,
		PT_Vec3,
		PT_Vec4,
		PT_Point,
		PT_AABBox2D,
		PT_AABBox3D,
		PT_Size,		
		PT_Enum,
		PT_File,
		PT_Array,
		PT_Struct,		
		PT_Color,
		PT_Link,
		PT_Range,
		PT_Reference,
		PT_Event,
		PT_Count,
		PT_Undefined
	};

	// serialization operators
		
	//! operator to format Vector2 into a string stream
	std::stringstream& operator << (std::stringstream& ss, const Vector2& vec2D);
	
	//! operator to build Vector2 from a string stream
	std::stringstream& operator >> (std::stringstream& ss, Vector2& vec2D);
	
	//! operator to format Vector3 into a string stream
	std::stringstream& operator << (std::stringstream& ss, const Vector3& vec3D);
	
	//! operator to build Vector3 from a string stream
	std::stringstream& operator >> (std::stringstream& ss, Vector3& vec3D);

	//! operator to format Vector4 into a string stream
	std::stringstream& operator << (std::stringstream& ss, const Vector4& vec4D);
	
	//! operator to build Vector4 from a string stream
	std::stringstream& operator >> (std::stringstream& ss, Vector4& vec4D);
	
	//! operator to format Point into a string stream
	std::stringstream& operator << (std::stringstream& ss, const Point& point);
	
	//! operator to build Point from a string stream
	std::stringstream& operator >> (std::stringstream& ss, Point& point);	
	
	//! operator to format AABBox2D into a string stream
	std::stringstream& operator << (std::stringstream& ss, const AABBox2D& bbox2D);
	
	//! operator to build AABBox2D from a string stream
	std::stringstream& operator >> (std::stringstream& ss, AABBox2D& bbox2D);
	
	//! operator to format AABBox3D into a string stream
	std::stringstream& operator << (std::stringstream& ss, const AABBox3D& bbox3D);
	
	//! operator to build AABBox3D from a string stream
	std::stringstream& operator >> (std::stringstream& ss, AABBox3D& bbox3D);
	
	//! operator to format Size into a string stream
	std::stringstream& operator << (std::stringstream& ss, const Size& size);
	
	//! operator to build Size from a string stream
	std::stringstream& operator >> (std::stringstream& ss, Size& size);
	
	//! operator to format Color into a string stream
	std::stringstream& operator << (std::stringstream& ss, const Color& color);
	
	//! operator to build Color from a string stream
	std::stringstream& operator >> (std::stringstream& ss, Color& color);
	
	//! operator to format Size into a string stream
	std::stringstream& operator << (std::stringstream& ss, const Range& range);
	
	//! operator to build Size from a string stream
	std::stringstream& operator >> (std::stringstream& ss, Range& range);	
	
	//! Property interface
	/** A property is used to expose a variable to XML and to visual editors */
	class IProperty
	{
	public:
			
		//! Constructor
		IProperty()
		{
		}

		//! Destructor
		virtual ~IProperty()
		{
		}
			
		//! returns the string representation of this property		
		virtual const std::string GetString() const = 0;

		//! sets the data of this property from a string
		virtual void SetData(const std::string& strData) = 0;

		//! returns the data stored in this property
		virtual bool GetData(void* pData) const = 0;

		//! sets the data of this property from a void pointer
		virtual void SetData(const void* pData) = 0;

		//! returns the type of this property
		virtual E_PropertyType GetType() const = 0;

		//! copies this property
		virtual IProperty* Copy() const = 0;

		//! passes extra params to the property
		virtual void SetParams(const void* pParams) { }

		//! returns the type name of this property
		std::string GetClassName() const { return TypeNames[GetType()]; }

		//! sets the name of this property
		void SetName(const std::string& strName) { m_strName = strName; }

		//! returns the name of this property
		const std::string& GetName() const { return m_strName; }

		//! maximum size of the string representation of properties
		static const u32 MaxStringSize = 256;

		//! max type name size
		static const u32 MaxTypeNameSize = 8;

		//! property type names
		static const char TypeNames[PT_Count][MaxTypeNameSize];

		//! creates a property from a type enum
		static IProperty* CreateFromType(E_PropertyType eType);

		//! creates a property from a type name
		static IProperty* CreateFromTypeName(const std::string& strType);

	protected:

		std::string m_strName;
	};
	
	// Property class containing a data of type DataType
	template <class DataType>
	class Property : public IProperty
	{
	public:
			
		//! Constructor
		Property(E_PropertyType eType) : m_eType(eType)
		{
		}

		//! returns the string representation of this property			
		virtual const std::string GetString() const
		{
			std::stringstream ss(std::stringstream::in | std::stringstream::out);
			char buf[MaxStringSize];
			ss << m_Data;
			ss.get(buf, MaxStringSize);
			return std::string(buf);
		}

		//! sets the data of this property from a string		
		virtual void SetData(const std::string& strData)
		{
			std::stringstream ss(std::stringstream::in | std::stringstream::out);
			ss << strData;
			ss >> m_Data;
		}

		//! returns the data stored in this property
		virtual bool GetData(void* pData) const
		{ 
			bool bChanged = (*static_cast<DataType*>(pData) != m_Data);
			*static_cast<DataType*>(pData) = m_Data;
			return bChanged;
		}

		//! sets the data of this property from a void pointer
		virtual void SetData(const void* pData) { m_Data = *static_cast<const DataType*>(pData); }	

		//! returns the type of this property
		E_PropertyType GetType() const { return m_eType; }

		//! copies this property
		virtual IProperty* Copy() const
		{
			Property<DataType>* pCopy = snew Property<DataType>(m_eType);
			*pCopy = *this;
			return pCopy;
		}

	protected:

		DataType m_Data;
		E_PropertyType m_eType;
	};

	//! Integer property class
	class IntProperty : public Property<s32>
	{
	public:
		//! Constructor
		IntProperty() : Property<s32>(PT_Int)
		{
			m_Data = 0;		
		}
	};

	//! Unsigned Integer property class
	class UIntProperty : public Property<u32>
	{
	public:
		//! Constructor
		UIntProperty() : Property<u32>(PT_UInt)
		{
			m_Data = 0;		
		}

		//! returns the string representation of this property			
		virtual const std::string GetString() const
		{
			std::stringstream ss(std::stringstream::in | std::stringstream::out);
			char buf[MaxStringSize];
			ss << std::hex << m_Data;
			ss.get(buf, MaxStringSize);
			return std::string(buf);
		}

		//! sets the data of this property from a string		
		virtual void SetData(const std::string& strData)
		{
			std::stringstream ss(std::stringstream::in | std::stringstream::out);
			ss << std::hex << strData;
			ss >> m_Data;
		}
	};

	//! Float property class
	class FloatProperty : public Property<f32>
	{
	public:
		//! Constructor
		FloatProperty() : Property<f32>(PT_Float)
		{
			m_Data = 0.0f;
		}
	};

	//! String property class
	class StringProperty : public Property<std::string>
	{
	public:
		//! Constructor
		StringProperty() : Property<std::string>(PT_String)
		{			
		}		

		//! sets the data of this property from a string		
		void SetData(const std::string& strData) { m_Data = strData; }		
	};

	//! Bool property class
	class BoolProperty : public Property<bool>
	{
	public:
		//! Constructor
		BoolProperty() : Property<bool>(PT_Bool)
		{
			m_Data = false;
		}		
	};

	//! Vec2 property class
	class Vec2Property : public Property<Vector2>
	{
	public:
		//! Constructor
		Vec2Property() : Property<Vector2>(PT_Vec2)
		{			
		}		
	};

	//! Vec3 property class
	class Vec3Property : public Property<Vector3>
	{
	public:
		//! Constructor
		Vec3Property() : Property<Vector3>(PT_Vec3)
		{			
		}		
	};

	//! Vec4 property class
	class Vec4Property : public Property<Vector4>
	{
	public:
		//! Constructor
		Vec4Property() : Property<Vector4>(PT_Vec4)
		{			
		}
	};

	//! Point property class
	class PointProperty : public Property<Point>
	{
	public:
		//! Constructor
		PointProperty() : Property<Point>(PT_Point)
		{			
		}		
	};

	//! Axis-aligned 2D Bounding Box property class
	class AABBox2DProperty : public Property<AABBox2D>
	{
	public:
		//! Constructor
		AABBox2DProperty() : Property<AABBox2D>(PT_AABBox2D)
		{			
		}		
	};

	//! Axis-aligned 3D Bounding Box property class
	class AABBox3DProperty : public Property<AABBox3D>
	{
	public:
		//! Constructor
		AABBox3DProperty() : Property<AABBox3D>(PT_AABBox3D)
		{			
		}		
	};

	//! Size property class
	class SizeProperty : public Property<Size>
	{
	public:
		//! Constructor
		SizeProperty() : Property<Size>(PT_Size)
		{			
		}		
	};

	//! Macro to pass EnumProperty params
	#ifdef SHOOT_EDITOR
		#define ENUM_PARAMS2(literals, values) snew EnumProperty::Params(literals, values)
		#define ENUM_PARAMS1(literals) ENUM_PARAMS2(literals, 0)
	#else
		#define ENUM_PARAMS2(literals, values) 0
		#define ENUM_PARAMS1(literals) 0
	#endif // SHOOT_EDITOR

	//! Enum property class
	class EnumProperty : public Property<s32>
	{
	public:

		//! Constructor
		EnumProperty() : Property<s32>(PT_Enum), m_Params(NULL, NULL)
		{
			m_Data = 0;
		}

		//! params
		struct Params
		{
			Params(const char* const* pLiterals, const s32* pValues = NULL)
				: m_pLiterals(pLiterals)
				, m_pValues(pValues)
			{
			}

			const char* const* m_pLiterals;
			const s32* m_pValues;
		};

		//! passes extra params to the property
		void SetParams(const void* pParams) 
		{
			if(pParams)
			{
				m_Params = *static_cast<const Params*>(pParams);
			}
		}

		//! returns the enumeration literals
		const char* const* GetLiterals() const { return m_Params.m_pLiterals; }

		//! returns the enumeration values
		const s32* GetValues() const { return m_Params.m_pValues; }

	private:

		Params m_Params;
	};

	//! File property class
	class FileProperty : public StringProperty
	{
	public:
		//! Constructor
		FileProperty() : m_strFilter(NULL)
		{			
			m_eType = PT_File;
		}

		//! passes extra params to the property
		void SetParams(const void* pParams) 
		{ 
			m_strFilter = static_cast<const char*>(pParams);
		}

		const char* GetFilter() const { return m_strFilter; }

	private:

		const char* m_strFilter;
	};

	//! Array property class
	/** Contains an array of other properties */
	class ArrayProperty : public IProperty
	{
	public:
		
		//! Constructor
		ArrayProperty() : m_eSubType(PT_Undefined), m_MaxElems(-1), m_pArray(NULL)
		{
		}

		//! Destructor
		virtual ~ArrayProperty()
		{
			for(u32 i=0; i<m_aProperties.size(); ++i)
			{
				IProperty* pProperty = m_aProperties[i];
				delete pProperty;
			}
		}

		//! returns the string representation of this property		
		const std::string GetString() const { return ""; }

		//! sets the data of this property from a string
		void SetData(const std::string& strData) { }

		//! returns the data stored in this property
		bool GetData(void* pData) const { return false; }

		//! sets the data of this property from a void pointer
		virtual void SetData(const void* pData) { }

		//! returns the type of this property
		virtual E_PropertyType GetType() const { return PT_Array; }

		//! returns the properties
		std::vector<IProperty*>& GetProperties() { return m_aProperties; }

		//! returns the properties
		const std::vector<IProperty*>& GetProperties() const { return m_aProperties; }

		//! sets the subtype of this property
		void SetSubType(E_PropertyType eSubType) { m_eSubType = eSubType; }

		//! returns the subtype
		E_PropertyType GetSubType() const { return m_eSubType; } 

		//! sets the max number of elements
		void SetMaxElems(s32 maxElems) { m_MaxElems = maxElems; }

		//! returns the max number of elements
		s32 GetMaxElems() const { return m_MaxElems; }

		//! sets the array interface
		void SetArray(IArray* pArray) { m_pArray = pArray; }

		//! returns the array interface
		IArray* GetArray() { return m_pArray; }

		//! copies this property
		virtual IProperty* Copy() const
		{
			ArrayProperty* pCopy = snew ArrayProperty();
			pCopy->m_strName = m_strName;
			pCopy->m_eSubType = m_eSubType;
			pCopy->m_MaxElems = m_MaxElems;
			pCopy->m_pArray = m_pArray;
			for(u32 i=0; i<m_aProperties.size(); ++i)
			{
				IProperty* pPropertyCopy = m_aProperties[i]->Copy();
				pCopy->m_aProperties.push_back(pPropertyCopy);
			}
			return pCopy;
		}

	private:

		std::vector<IProperty*> m_aProperties;
		E_PropertyType m_eSubType;
		s32 m_MaxElems;
		IArray* m_pArray;
	};

	//! Struct property class
	/** represents a user defined structure that can be composed of any property type 
		Example use:

		struct MyStruct : public ISerializableStruct
		{
			s32 myInt;
			std::string myString;
			MyOtherStruct myOtherStruct;

			void Serialize(PropertyStream& stream)
			{
				stream.Serialize(PT_Int, "MyInt", &myInt);
				stream.Serialize(PT_String, "MyString", &myString);
				stream.Serialize(PT_Struct, "MyOtherStruct", &myOtherStruct);
			}
		};

		MyStruct myStruct;

		PropertyStream.Serialize(PT_Struct, "MyStruct", &myStruct);
	*/
	class StructProperty : public IProperty
	{
	public:
		
		//! Constructor
		StructProperty() : m_Stream(SM_Write)
		{
		}

		//! returns the string representation of this property		
		const std::string GetString() const { return ""; }

		//! sets the data of this property from a string
		void SetData(const std::string& strData) { }

		//! returns the data stored in this property
		bool GetData(void* pData) const { return false; }

		//! sets the data of this property from a void pointer
		virtual void SetData(const void* pData) { }

		//! returns the type of this property
		virtual E_PropertyType GetType() const { return PT_Struct; }

		//! returns the property stream 
		PropertyStream& GetStream() { return m_Stream; }

		//! copies this property
		virtual IProperty* Copy() const
		{
			StructProperty* pCopy = snew StructProperty();
			*pCopy = *this;	
			return pCopy;
		}

	private:

		PropertyStream m_Stream;
	};

	//! user defined structs must inherit from this to become serializable
	struct ISerializableStruct
	{
		//! destructor
		virtual ~ISerializableStruct()
		{
		}

		//! Reads/Writes struct properties from/to a stream
		virtual void Serialize(PropertyStream& stream) = 0;
	};

	//! Color property class
	class ColorProperty : public Property<Color>
	{
	public:
		//! Constructor
		ColorProperty() : Property<Color>(PT_Color)
		{					
		}		
	};

	//! Link interface
	class ILink
	{
	public:

		//! returns the Object ID
		virtual u32 GetObjectID() const = 0;

		//! sets the Object ID
		virtual void SetObjectID(u32 ID) = 0;

		//! returns the object class name
		virtual const char* GetClassName() const = 0;

		//! initializes the link
		void Init(Object* pParent);

	protected:

		//! sets the object pointer
		virtual void SetObject(Object* pObject) = 0;
	};

	//! Link property class
	class LinkProperty : public IProperty
	{
	public:
		//! Constructor
		LinkProperty() : m_pLink(NULL), m_ID(0)
		{
		}

		//! returns the string representation of this property		
		const std::string GetString() const
		{ 
			std::stringstream ss(std::stringstream::in | std::stringstream::out);
			char buf[MaxStringSize];
			ss << std::hex << m_ID;
			ss.get(buf, MaxStringSize);
			return std::string(buf);			
		}

		//! sets the data of this property from a string
		void SetData(const std::string& strData)
		{ 
			std::stringstream ss(std::stringstream::in | std::stringstream::out);
			ss << std::hex << strData;
			ss >> m_ID;
		}

		//! returns the data stored in this property
		bool GetData(void* pData) const
		{
			bool bChanged = (static_cast<ILink*>(pData)->GetObjectID() != m_ID);
			static_cast<ILink*>(pData)->SetObjectID(m_ID);
			return bChanged;
		}

		//! sets the data of this property from a void pointer
		void SetData(const void* pData)
		{
			m_pLink = static_cast<const ILink*>(pData);
			m_ID = m_pLink->GetObjectID();
		}

		//! returns the type of this property
		virtual E_PropertyType GetType() const { return PT_Link; }

		//! returns the link interface
		const ILink* GetLink() const { return m_pLink; }

		//! copies this property
		virtual IProperty* Copy() const
		{
			LinkProperty* pCopy = snew LinkProperty();
			*pCopy = *this;
			return pCopy;
		}

	private:

		const ILink* m_pLink;
		u32 m_ID;
	};

	//! Concrete link template
	template<class T>
	class Link : public ILink
	{
	public:

		//! constructor
		Link() : m_ID(0)
		{
		}

		//! returns the Object ID
		u32 GetObjectID() const { return m_ID; }

		//! sets the Object ID
		void SetObjectID(u32 ID) { m_ID = ID; }

		//! returns the object class name
		const char* GetClassName() const { return T::GetStaticClassName(); }

		//! returns the target object
		T* Get() { return m_Object; }

		//! returns the target object
		T* Get() const { return m_Object; }

		//! -> operator
		inline T* operator ->()
		{
			return m_Object;
		}

		//! const -> operator
		inline T* operator ->() const
		{
			return m_Object;
		}

		//! clears the link
		void Clear()
		{
			m_Object = NULL;
			m_ID = 0;
		}

		//! sets the object pointer
		void SetObject(Object* pObject)
		{
			m_Object = static_cast<T*>(pObject);
			m_ID = pObject ? m_Object->GetID() : 0;
		}

	private:

		Handle<T> m_Object;
		u32 m_ID;
	};

	//! Range property class
	class RangeProperty : public Property<Range>
	{
	public:
		//! Constructor
		RangeProperty() : Property<Range>(PT_Range)
		{			
		}		
	};

	// forwards
	class IReference;

	//! Reference property class
	class ReferenceProperty : public StructProperty
	{
	public:				

		//! constructor
		ReferenceProperty() : m_pRefInterface(NULL)
		{
		}
			
		//! returns the type of this property
		virtual E_PropertyType GetType() const { return PT_Reference; }

		//! sets the reference interface
		inline void SetRefInterface(IReference* pRefInterface) { m_pRefInterface = pRefInterface; }

		//! returns the reference interface
		inline IReference* GetRefInterface() { return m_pRefInterface; }

		//! returns the class name of the referenced object
		std::string GetClassName() const { return m_strClassName; }

		//! sets the base class name
		inline void SetClassName(std::string strClassName) { m_strClassName = strClassName; }

		//! sets the source template path
		inline void SetTemplatePath(const std::string& strPath) { m_strTemplatePath = strPath; }

		//! returns the source template
		inline const std::string& GetTemplatePath() const { return m_strTemplatePath; }

		//! copies this property
		virtual IProperty* Copy() const
		{
			ReferenceProperty* pCopy = snew ReferenceProperty();
			*pCopy = *this;
			return pCopy;
		}

	private:
				
		IReference* m_pRefInterface;
		std::string m_strClassName;
		std::string m_strTemplatePath;
	};

	//! Event property class
	class EventProperty : public StringProperty
	{
	public:
		//! Constructor
		EventProperty()
		{
			m_eType = PT_Event;
		}
	};
}

#endif // _PROPERTY_H_INCLUDED_

