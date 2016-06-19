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
WX_PG_IMPLEMENT_STRING_PROPERTY_WITH_VALIDATOR(wxEventProperty, wxPG_NO_ESCAPE)

//! button click callback
bool wxEventProperty::OnButtonClick(wxPropertyGrid* propgrid, wxString& value)
{
	Object* pEvent = ObjectFactory::Instance()->Create(m_strEventName);
	SHOOT_ASSERT(pEvent->IsA(Event::TypeID), "Object is not of type Event");
	EventManager::Instance()->SendEvent(static_cast<Event*>(pEvent));

    return true;
}

wxValidator* wxEventProperty::DoGetValidator() const
{
    return NULL;
}

