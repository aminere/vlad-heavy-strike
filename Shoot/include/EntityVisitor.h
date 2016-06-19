/* 

Amine Rehioui
Created: November 3rd 2013

*/

#ifndef _ENTITY_VISITOR_H_INCLUDED_
#define _ENTITY_VISITOR_H_INCLUDED_

#include "Visitor.h"

namespace shoot
{
	//! EntityVisitor class
	class EntityVisitor : public Visitor
	{
	public:

		DECLARE_OBJECT(EntityVisitor, Visitor);

		//! constructor
		EntityVisitor();

		//! Reads/Writes struct properties from/to a stream
		void Serialize(PropertyStream& stream);		

		//! visits a particular entity
		virtual void Visit(Entity* pTarget);
	
	protected:
		
		// properties
		bool m_bEntityEnabled;
		bool m_bEntityVisible;
		bool m_bCollisionEnabled;
	};
}

#endif // _ENTITY_VISITOR_H_INCLUDED_


