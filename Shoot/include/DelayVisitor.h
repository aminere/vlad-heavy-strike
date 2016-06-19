/* 

Amine Rehioui
Created: April 10th 2011

*/

#ifndef _DELAY_VISITOR_H_INCLUDED_
#define _DELAY_VISITOR_H_INCLUDED_

#include "Visitor.h"

namespace shoot
{
	//! DelayVisitor class
	class DelayVisitor : public Visitor
	{
	public:

		DECLARE_OBJECT(DelayVisitor, Visitor);
		
		//! constructor
		DelayVisitor();

		//! destructor
		virtual ~DelayVisitor();

		//! Reads/Writes struct properties from/to a stream
		virtual void Serialize(PropertyStream& stream);		

		//! visits a particular entity
		virtual void Visit(Entity* pTarget);

		//! updates the visitor		
		virtual void Update();

		//! sets the delay duration
		inline void SetDuration(f32 fDuration) { m_fDuration = fDuration; }

	protected:		

		f32 m_fCurrentTime;

		// properties				
		f32 m_fDuration;
	};
}

#endif // _DELAY_VISITOR_H_INCLUDED_

