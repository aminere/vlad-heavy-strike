/* 

Amine Rehioui
Created: July 23rd 2011

*/

#ifndef _SHOOT_EDITOR_EVENT_H_INCLUDED_
#define _SHOOT_EDITOR_EVENT_H_INCLUDED_

#include "Event.h"

namespace shoot
{
	//! EditorEvents handler class
	class ShootEditorEventsHandler
	{
	public:

		//! constructor
		ShootEditorEventsHandler();

		//! destructor
		~ShootEditorEventsHandler();

		//! handles SetCameraPathElementInfoEvent
		static void HandleSetCameraPathElementInfoEvent(void* pListener, Event* pEvent);
	};
}

#endif // _EDITOR_EVENT_H_INCLUDED_

