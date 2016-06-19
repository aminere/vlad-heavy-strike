/* 

Amine Rehioui
Created: July 14th 2012

*/

#ifndef _REMOVE_ENTITY_VISITOR_H_INCLUDED_
#define _REMOVE_ENTITY_VISITOR_H_INCLUDED_

#include "Visitor.h"

namespace shoot
{
	//! RemoveEntityVisitor class
	class RemoveEntityVisitor : public Visitor
	{
	public:

		DECLARE_OBJECT(RemoveEntityVisitor, Visitor);

		//! visits a particular entity
		void Visit(Entity* pTarget)
		{
			pTarget->GetParent()->RemoveChild(pTarget);			
		}
	};
}

#endif // _REMOVE_ENTITY_VISITOR_H_INCLUDED_

