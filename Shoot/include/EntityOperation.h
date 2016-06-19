/* 

Amine Rehioui
Created: September 15th 2012

*/

#ifndef _ENTITY_OPERATION_H_INCLUDED_
#define _ENTITY_OPERATION_H_INCLUDED_

namespace shoot
{
	//! entity operation type
	enum E_EntityOperation
	{
		EO_AddChild,
		EO_AddComponent,
		EO_InsertChild,
		EO_RemoveChild,		
		EO_RemoveComponent,
	};

	//! entity operation info		
	struct EntityOperationInfo
	{
		//! constructor
		EntityOperationInfo(E_EntityOperation operation, Entity* pEntity, Object* pObject, s32 index = -1)
			: m_Operation(operation)
			, m_Entity(pEntity)
			, m_Object(pObject)
			, m_Index(index)
		{
		}

		E_EntityOperation m_Operation;
		Reference<Entity> m_Entity;
		Reference<Object> m_Object;
		s32 m_Index;

		//! == operator
		bool operator == (const EntityOperationInfo& other)
		{
			return m_Operation == other.m_Operation
				&& m_Entity == other.m_Entity
				&& m_Object == other.m_Object
				&& m_Index == other.m_Index;
		}
	};
}

#endif // _ENTITY_OPERATION_H_INCLUDED_

