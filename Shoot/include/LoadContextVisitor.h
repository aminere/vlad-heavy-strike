/* 

Amine Rehioui
Created: June 16th 2013

*/

#ifndef _LOAD_CONTEXT_VISITOR_H_INCLUDED_
#define _LOAD_CONTEXT_VISITOR_H_INCLUDED_

#include "Visitor.h"

namespace shoot
{
	//! LoadContextVisitor
	class LoadContextVisitor : public Visitor
	{
	public:

		DECLARE_OBJECT(LoadContextVisitor, Visitor);
		
		//! Reads/Writes struct properties from/to a stream
		void Serialize(PropertyStream& stream);		

		//! visits a particular entity
		void Visit(Entity* pTarget);

	protected:

		// properties
		std::string m_ContextStack;		
	};
}

#endif // _LOAD_CONTEXT_VISITOR_H_INCLUDED_


