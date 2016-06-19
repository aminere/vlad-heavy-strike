/* 

Amine Rehioui
Created: August 7th 2011

*/

#ifndef _EDITOR_EVENTS_H_INCLUDED_
#define _EDITOR_EVENTS_H_INCLUDED_

#include "Event.h"

namespace shoot
{
	//! EditorEvents class
	class EditorEvents
	{
	public:

		//! registers engine events
		static void Register();

		//! sets a PathElement camera info
		class SetCameraPathElementInfoEvent : public Event
		{
		public:
			DECLARE_OBJECT(SetCameraPathElementInfoEvent, Event);
		};
	};
}

#endif // _ENGINE_EVENTS_H_INCLUDED_

