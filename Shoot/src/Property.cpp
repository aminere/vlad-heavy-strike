/* 

Amine Rehioui
Created: February 24th 2010

*/

#include "Shoot.h"

namespace shoot
{
	// property typenames
	const char IProperty::TypeNames[PT_Count][MaxTypeNameSize] =
	{
		"int", // PT_Int
		"uint", // PT_UInt
		"float", // PT_Float
		"string", // PT_String
		"bool", // PT_Bool
		"vec2D", // PT_Vec2
		"vec3D", // PT_Vec3
		"vec4D", // PT_Vec4D
		"point", // PT_Point
		"aabb2D", // PT_AABBox2D
		"aabb3D", // PT_AABBox3D
		"size", // PT_Size		
		"enum", // PT_Enum
		"file", // PT_File
		"array", // PT_Array
		"struct", // PT_Struct
		"color", // PT_Color
		"link", // PT_Link
		"range", // PT_Range
		"ref", // PT_Reference
		"event", // PT_Event
	};

	//! creates a property from a type enum-
	IProperty* IProperty::CreateFromType(E_PropertyType eType)
	{
		switch(eType)
		{
		case PT_Int:	return snew IntProperty();
		case PT_UInt:	return snew UIntProperty();
		case PT_Float:	return snew FloatProperty();
		case PT_String: return snew StringProperty();
		case PT_Bool:	return snew BoolProperty();
		case PT_Vec2:	return snew Vec2Property();
		case PT_Vec3:	return snew Vec3Property();
		case PT_Vec4:	return snew Vec4Property();
		case PT_Point:	return snew PointProperty();
		case PT_AABBox2D: return snew AABBox2DProperty();
		case PT_AABBox3D: return snew AABBox3DProperty();
		case PT_Size:	return snew SizeProperty();		
		case PT_Enum:	return snew EnumProperty();
		case PT_File:	return snew FileProperty();
		case PT_Array: return snew ArrayProperty();
		case PT_Struct:	return snew StructProperty();
		case PT_Color:	return snew ColorProperty();
		case PT_Link:	return snew LinkProperty();
		case PT_Range:	return snew RangeProperty();
		case PT_Reference: return snew ReferenceProperty();
		case PT_Event: return snew EventProperty();
	
		default:
			SHOOT_ASSERT(0, "Invalid E_PropertyType");
			return NULL;
		}
	}

	//! creates a property from a type name
	IProperty* IProperty::CreateFromTypeName(const std::string& strType)
	{
		for(u32 i=0; i<PT_Count; ++i)
		{
			if(strType == TypeNames[i])
			{
				return CreateFromType((E_PropertyType)i);
			}
		}
		SHOOT_ASSERT(0, "Invalid property type name");
		return 0;
	}

	//! operator to format Vector2 into a string stream
	std::stringstream& operator << (std::stringstream& ss, const Vector2& vec2D)
	{
		ss << vec2D.X << " " << vec2D.Y;
		return ss;
	}

	//! operator to build Vector2 from a string stream
	std::stringstream& operator >> (std::stringstream& ss, Vector2& vec2D)
	{
		ss >> vec2D.X >> vec2D.Y;
		return ss;
	}

	//! operator to format Vector3 into a string stream
	std::stringstream& operator << (std::stringstream& ss, const Vector3& vec3D)
	{
		ss << vec3D.X << " " << vec3D.Y << " " << vec3D.Z;
		return ss;
	}

	//! operator to build Vector3 from a string stream
	std::stringstream& operator >> (std::stringstream& ss, Vector3& vec3D)
	{
		ss >> vec3D.X >> vec3D.Y >> vec3D.Z;
		return ss;
	}

	//! operator to format Vector4 into a string stream
	std::stringstream& operator << (std::stringstream& ss, const Vector4& vec4D)
	{
		ss << vec4D.X << " " << vec4D.Y << " " << vec4D.Z << " " << vec4D.W;
		return ss;
	}

	//! operator to build Vector4 from a string stream
	std::stringstream& operator >> (std::stringstream& ss, Vector4& vec4D)
	{
		ss >> vec4D.X >> vec4D.Y >> vec4D.Z >> vec4D.W;
		return ss;
	}


	//! operator to format Point into a string stream
	std::stringstream& operator << (std::stringstream& ss, const Point& point)
	{
		ss << point.X << " " << point.Y;
		return ss;
	}

	//! operator to build Point from a string stream
	std::stringstream& operator >> (std::stringstream& ss, Point& point)
	{
		ss >> point.X >> point.Y;
		return ss;
	}

	//! operator to format AABBox2D into a string stream
	std::stringstream& operator << (std::stringstream& ss, const AABBox2D& bbox2D)
	{
		// TODO: find out why this doesn't work: ss << bbox2D.Min() << " " << bbox2D.Max();
		ss << bbox2D.Min() << " ";
		ss << bbox2D.Max();
		return ss;
	}

	//! operator to build AABBox2D from a string stream
	std::stringstream& operator >> (std::stringstream& ss, AABBox2D& bbox2D)
	{
		Vector2 vMin, vMax;
		ss >> vMin >> vMax;
		bbox2D.Set(vMin, vMax);
		return ss;
	}

	//! operator to format AABBox3D into a string stream
	std::stringstream& operator << (std::stringstream& ss, const AABBox3D& bbox3D)
	{
		// TODO: find out why this doesn't work: ss << bbox3D.Min() << " " << bbox3D.Max();
		ss << bbox3D.Min() << " ";
		ss << bbox3D.Max();
		return ss;
	}

	//! operator to build AABBox3D from a string stream
	std::stringstream& operator >> (std::stringstream& ss, AABBox3D& bbox3D)
	{
		Vector3 vMin, vMax;
		ss >> vMin >> vMax;
		bbox3D.Set(vMin, vMax);
		return ss;
	}

	//! operator to format Size into a string stream
	std::stringstream& operator << (std::stringstream& ss, const Size& size)
	{
		ss << size.Width << " " << size.Height;
		return ss;
	}

	//! operator to build Size from a string stream
	std::stringstream& operator >> (std::stringstream& ss, Size& size)
	{
		ss >> size.Width >> size.Height;
		return ss;
	}	
	
	//! operator to format Color into a string stream
	std::stringstream& operator << (std::stringstream& ss, const Color& color)
	{
		// TODO: implement/get RGBA dialog editor in wxWidgets
		ss << color.R << " " << color.G << " " << color.B; // << " " << color.A; 
		return ss;
	}

	//! operator to build Color from a string stream
	std::stringstream& operator >> (std::stringstream& ss, Color& color)
	{
		// TODO: implement/get RGBA dialog editor in wxWidgets
		ss >> color.R >> color.G >> color.B; // >> color.A;
		return ss;
	}

	//! operator to format Size into a string stream
	std::stringstream& operator << (std::stringstream& ss, const Range& range)
	{
		ss << range.Min << " " << range.Max;
		return ss;
	}

	//! operator to build Size from a string stream
	std::stringstream& operator >> (std::stringstream& ss, Range& range)
	{
		ss >> range.Min >> range.Max;
		return ss;
	}

	//! initializes the link
	void ILink::Init(Object* pParent)
	{
		if(GetObjectID())
		{
			SHOOT_ASSERT(pParent->IsA(Entity::TypeID), "Link parent not of type Entity");
			Object* pObject = static_cast<Entity*>(pParent)->GetRoot()->GetChildByID(GetObjectID());
			SHOOT_WARNING(pObject, "Unresolved object Link");
			SetObject(pObject);
		}
		else
		{
			SetObject(NULL);
		}
	}
}

