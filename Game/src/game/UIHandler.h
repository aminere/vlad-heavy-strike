/* 

Amine Rehioui
Created: October 5th 2013

*/

#ifndef _UI_HANDLER_H_INCLUDED_
#define _UI_HANDLER_H_INCLUDED_

#include "Event.h"

namespace shoot
{
	//! UIHandler
	class UIHandler
	{
	public:

		//! shoot event listeners
		static void HandleEvent(void* pListener, Event* pEvent);

		//! shoot event listeners
		static void HandleTriggerEvent(void* pListener, Event* pEvent);
	};
}

#endif // _UI_HANDLER_H_INCLUDED_


