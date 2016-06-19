/* 

Amine Rehioui
Created: June 1st 2013

*/

#ifndef _TEXT_VISITOR_H_INCLUDED_
#define _TEXT_VISITOR_H_INCLUDED_

#include "Visitor.h"

namespace shoot
{
	// forwards 
	class Text;

	//! TextVisitor class
	class TextVisitor : public Visitor
	{
	public:

		DECLARE_OBJECT(TextVisitor, Visitor);

		//! constructor
		TextVisitor();

		//! destructor
		virtual ~TextVisitor();

		//! Reads/Writes struct properties from/to a stream
		virtual void Serialize(PropertyStream& stream);		

		//! visits a particular entity
		virtual void Visit(Entity* pTarget);

		//! updates the visitor
		virtual void Update();

		//! Leaves the visited entity
		void Leave();

	protected:

		Text* m_pText;
		f32 m_fCurrentTime;

		// properties
		f32 m_fDuration;
	};
}

#endif // _TEXT_VISITOR_H_INCLUDED_


