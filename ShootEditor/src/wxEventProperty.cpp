/* 

Amine Rehioui
Created: August 7th 2011

*/

#include "ShootEditorCommon.h"

#include "wxEventProperty.h"

#include "Event.h"
#include "EventManager.h"

// -----------------------------------------------------------------------
// wxEventProperty
// -----------------------------------------------------------------------
WX_PG_IMPLEMENT_PROPERTY_CLASS(wxEventProperty, wxPGProperty, wxString, const wxString&, TextCtrlAndButton)

//! button click callback
bool wxEventProperty::OnButtonClick(wxPropertyGrid* propgrid, wxString& value)
{
	Object* pEvent = ObjectFactory::Instance()->Create(m_strEventName);
	SHOOT_ASSERT(pEvent->IsA(Event::TypeID), "Object is not of type Event");
	EventManager::Instance()->SendEvent(static_cast<Event*>(pEvent));

    return true;
}


