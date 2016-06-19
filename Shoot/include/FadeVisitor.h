/* 

Amine Rehioui
Created: October 30th 2013

*/

#ifndef _FADE_VISITOR_H_INCLUDED_
#define _FADE_VISITOR_H_INCLUDED_

#include "Visitor.h"

namespace shoot
{
	//! FadeVisitor class
	class FadeVisitor : public Visitor
	{
	public:

		DECLARE_OBJECT(FadeVisitor, Visitor);

		//! constructor
		FadeVisitor();

		//! destructor
		virtual ~FadeVisitor();

		//! Reads/Writes struct properties from/to a stream
		virtual void Serialize(PropertyStream& stream);		

		//! visits a particular entity
		virtual void Visit(Entity* pTarget);
	
		//! updates the visitor
		virtual void Update();

	protected:
		
		f32 m_fTimer;

		// properties
		Color m_SrcColor;
		Color m_DestColor;
		f32 m_fDuration;
	};
}

#endif // _FADE_VISITOR_H_INCLUDED_


