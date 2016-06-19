/* 

Amine Rehioui
Created: June 10th 2013

*/

#ifndef _CONVERSATION_VISITOR_H_INCLUDED_
#define _CONVERSATION_VISITOR_H_INCLUDED_

#include "Visitor.h"
#include "ConversationManager.h"

namespace shoot
{
	//! ConversationVisitor
	class ConversationVisitor : public Visitor
	{
	public:

		DECLARE_OBJECT(ConversationVisitor, Visitor);

		//! Reads/Writes struct properties from/to a stream
		virtual void Serialize(PropertyStream& stream);		

		//! visits a particular entity
		virtual void Visit(Entity* pTarget);

	protected:

		// properties
		Array<ConversationManager::Phrase> m_Phrases;		
	};
}

#endif // _CONVERSATION_VISITOR_H_INCLUDED_


