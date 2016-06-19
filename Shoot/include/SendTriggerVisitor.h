/* 

Amine Rehioui
Created: July 14th 2012

*/

#ifndef _SEND_TRIGGER_VISITOR_H_INCLUDED_
#define _SEND_TRIGGER_VISITOR_H_INCLUDED_

#include "Visitor.h"

namespace shoot
{
	//! SendTriggerVisitor class
	class SendTriggerVisitor : public Visitor
	{
	public:

		DECLARE_OBJECT(SendTriggerVisitor, Visitor);

		//! Reads/Writes struct properties from/to a stream
		virtual void Serialize(PropertyStream& stream);		

		//! visits a particular entity
		void Visit(Entity* pTarget);

	protected:

		// properties
		std::string m_TriggerName;
	};	
}

#endif // _SEND_TRIGGER_VISITOR_H_INCLUDED_

