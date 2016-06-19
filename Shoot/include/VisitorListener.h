/* 

Amine Rehioui
Created: April 2nd 2011

*/

#ifndef _VISITOR_LISTENER_H_INCLUDED_
#define _VISITOR_LISTENER_H_INCLUDED_

namespace shoot
{
	// forwards
	class Visitor;

	//! Visitor listener interface
	class VisitorListener
	{
	public:

		//! called when a visitor finished its job
		virtual void OnVisitorFinished(Visitor* pVisitor) { }
	};
}

#endif // _VISITOR_LISTENER_H_INCLUDED_

