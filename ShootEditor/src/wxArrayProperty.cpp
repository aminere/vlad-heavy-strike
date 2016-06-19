/* 

Amine Rehioui
Created: May 28th 2010

*/

#include "ShootEditorCommon.h"

#include "wxArrayProperty.h"

#include "PropertyList.h"

// -----------------------------------------------------------------------
// wxArrayProperty
// -----------------------------------------------------------------------
wxArrayProperty::wxArrayProperty( const wxString& label, ArrayProperty* pProperty, PropertyList* pParent )
: wxStringProperty(label, wxPG_LABEL)
, m_eEventType(E_None)
, m_pSubPropertyTemplate(NULL)
, m_pStructTemplate(NULL)
, m_pRefTemplate(NULL)
, m_pLinkTemplate(NULL)
, m_pParent(pParent)
{
	m_pSubPropertyTemplate = IProperty::CreateFromType(pProperty->GetSubType());
	m_MaxElems = pProperty->GetMaxElems();

	switch(m_pSubPropertyTemplate->GetType())
	{
	case PT_Struct:
		{
			StructProperty* pStructProperty = static_cast<StructProperty*>(m_pSubPropertyTemplate);
			pStructProperty->GetStream().SetMode(SM_Write);
			m_pStructTemplate = static_cast<ISerializableStruct*>(pProperty->GetArray()->Alloc());		
			m_pStructTemplate->Serialize(pStructProperty->GetStream());
		}
		break;

	case PT_Reference:
		{
			m_pRefTemplate = static_cast<IReference*>(pProperty->GetArray()->Alloc());		
			ReferenceProperty* pRefProperty = static_cast<ReferenceProperty*>(m_pSubPropertyTemplate);			
			pRefProperty->SetRefInterface(m_pRefTemplate);
		}
		break;

	case PT_Link:
		{
			m_pLinkTemplate = static_cast<ILink*>(pProperty->GetArray()->Alloc());		
			LinkProperty* pLinkProperty = static_cast<LinkProperty*>(m_pSubPropertyTemplate);			
			pLinkProperty->SetData(m_pLinkTemplate);
		}
	}

	UpdateValue();
}

wxArrayProperty::~wxArrayProperty()
{
	sdelete(m_pSubPropertyTemplate);
	sdelete(m_pStructTemplate);
	sdelete(m_pRefTemplate);
	sdelete(m_pLinkTemplate);
}

//! fills the property with children - must be called after the array has been inserted to the property list
void wxArrayProperty::Fill(ArrayProperty* pProperty)
{
	s32 numProperties = pProperty->GetProperties().size();
	for(s32 i=0; i<numProperties; ++i)
	{
		IProperty* pSubProperty = pProperty->GetProperties()[i];

		std::stringstream ss;
		ss << "[" << i << "]";
		pSubProperty->SetName(ss.str());

		wxPGProperty* pWxProperty = PropertyList::GetWxProperty(pSubProperty, m_pParent);
		InsertChild(-1, pWxProperty);
		bool bParentDisabled = !GetGrid()->IsPropertyEnabled(this);
		if(bParentDisabled)
		{
			GetGrid()->DisableProperty(pWxProperty);
		}
	}

	UpdateValue();
}

//! updates the array property value
void wxArrayProperty::UpdateValue()
{
	// display number of elements
	s32 numElements = this->GetChildCount();
	std::stringstream ss;
	ss << numElements << " elems";
	if(m_MaxElems > 0)
	{
		ss << " / " << m_MaxElems;
	}
	SetValue(wxVariant(ss.str()));
}

//! implement the array property editor
WX_PG_IMPLEMENT_EDITOR_CLASS(ArrayPropertyEditor, wxArrayPropertyEditor, wxPGTextCtrlEditor)

wxPGWindowList wxArrayPropertyEditor::CreateControls( wxPropertyGrid* propGrid,
                                                      wxPGProperty* property,
                                                      const wxPoint& pos,
                                                      const wxSize& sz ) const
{
    // Create and populate buttons-subwindow
    wxPGMultiButton* buttons = new wxPGMultiButton( propGrid, sz );
    buttons->Add("+");
    buttons->Add("-");

    // Create the 'primary' editor control (textctrl in this case)
    wxPGWindowList wndList = wxPGTextCtrlEditor::CreateControls( propGrid, property, pos, buttons->GetPrimarySize());

    // Finally, move buttons-subwindow to correct position and make sure
    // returned wxPGWindowList contains our custom button list.
    buttons->FinalizePosition(pos);

    wndList.SetSecondary( buttons );
    return wndList;
}

bool wxArrayPropertyEditor::OnEvent( wxPropertyGrid* propGrid,
                                     wxPGProperty* property,
                                     wxWindow* ctrl,
                                     wxEvent& event ) const
{
    if ( event.GetEventType() == wxEVT_COMMAND_BUTTON_CLICKED )
    {
        wxPGMultiButton* buttons = (wxPGMultiButton*) propGrid->GetEditorControlSecondary();
		wxArrayProperty* pArrayProperty = static_cast<wxArrayProperty*>(property);

		if(event.GetId() == buttons->GetButtonId(0)) // + - add element
		{
			if(pArrayProperty->GetMaxElems() < 0
			|| pArrayProperty->GetChildCount() < u32(pArrayProperty->GetMaxElems()))
			{
				// determine new property name
				IProperty* pNewProperty = pArrayProperty->GetSubPropertyTemplate();
				std::stringstream ss;
				ss << "[" << property->GetChildCount() << "]";
				pNewProperty->SetName(ss.str());

				// create a new wxPGProperty out of it and insert it
				wxPGProperty* pWxProperty = PropertyList::GetWxProperty(pNewProperty, pArrayProperty->GetParent());
				property->InsertChild(-1, pWxProperty);

				if(pNewProperty->GetType() == PT_Reference)
				{
					pArrayProperty->GetParent()->FillArrayProperties(pNewProperty, pWxProperty);	
				}

				pArrayProperty->UpdateValue();
				propGrid->RefreshProperty(property);			

				// update the corresponding property in the property stream
				wxPropertyGridEvent PGEvent;
				PGEvent.SetProperty(property);
				pArrayProperty->SetEventType(wxArrayProperty::E_PropertyAdded);
				pArrayProperty->GetParent()->OnPropertyGridChange(PGEvent);
			}			
		}	
		else if(event.GetId() == buttons->GetButtonId(1)) // - remove element
		{
			int childCount = property->GetChildCount();
			if(childCount > 0)
			{
				// remove the wx property
				wxPGProperty* pChild = property->Item(childCount-1);
				propGrid->DeleteProperty(pChild);	
				pArrayProperty->UpdateValue();
				propGrid->RefreshProperty(property);

				// update the corresponding property in the property stream
				wxPropertyGridEvent PGEvent;
				PGEvent.SetProperty(property);
				pArrayProperty->SetEventType(wxArrayProperty::E_PropertyRemoved);
				pArrayProperty->GetParent()->OnPropertyGridChange(PGEvent);
			}
		}

		return true;
    }
	else
	{
		return wxPGTextCtrlEditor::OnEvent(propGrid, property, ctrl, event);
	}    
}

