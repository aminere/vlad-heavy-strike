/* 

Amine Rehioui
Created: March 26th 2011

*/

#include "ShootEditorCommon.h"

#include "wxReferenceProperty.h"

#include "PropertyList.h"

#include "EditorUtils.h"

// -----------------------------------------------------------------------
// wxReferenceProperty
// -----------------------------------------------------------------------
wxReferenceProperty::wxReferenceProperty( const wxString& label, ReferenceProperty* pProperty, PropertyList* pParent )
: wxStringProperty(label, wxPG_LABEL)
, m_eEventType(E_None)
, m_pParent(pParent)
{
	SetClassName(pProperty->GetRefInterface()->GetClassName());
}

wxReferenceProperty::~wxReferenceProperty()
{
}

//! Fills the property - must be called after the wx property has been inserted to the property list
void wxReferenceProperty::Fill(ReferenceProperty* pProperty)
{
	PropertyStream& stream = pProperty->GetStream();
	for(u32 i=0; i<stream.GetNumProperties(); ++i)
	{
		IProperty* pProperty = stream.GetProperty(i);
		wxPGProperty* pWxProperty = PropertyList::GetWxProperty(pProperty, m_pParent);
		if(pWxProperty)
		{
			InsertChild(-1, pWxProperty);
			bool bParentDisabled = !GetGrid()->IsPropertyEnabled(this);
			if(bParentDisabled)
			{
				GetGrid()->DisableProperty(pWxProperty);
			}

			if(pProperty->GetType() == PT_Array
			|| pProperty->GetType() == PT_Reference
			|| pProperty->GetType() == PT_Struct)
			{
				GetParent()->FillArrayProperties(pProperty, pWxProperty);	
			}

			if(pProperty->GetName() == "ID")
			{
				pWxProperty->SetAttribute(wxPG_UINT_BASE, wxPG_BASE_HEX);
				pWxProperty->SetFlag(wxPG_PROP_DISABLED);
			}
		}		
	}

	// update property name
	IReference* pRefInterface = pProperty->GetRefInterface();
	std::string strTemplatePath = pRefInterface->Get() ? pRefInterface->Get()->GetTemplatePath() : "";
	wxString label = pProperty->GetName();
	SetLabel(label);

	UpdateValue(pProperty);
	GetParent()->RefreshProperty(this);
}

//! updates the reference property value
void wxReferenceProperty::UpdateValue(ReferenceProperty* pProperty)
{
	std::stringstream ss;
	IReference* pReference = pProperty ? pProperty->GetRefInterface() : NULL;
	Object* pObject = pReference ? pReference->Get() : NULL;
	if(pObject)
	{
		ss << pObject->GetClassName();
		if(!pObject->GetTemplatePath().empty())
		{
			ss << " (" << pObject->GetTemplatePath() << ")";
		}
		else
		{
			ss << " [" << std::hex << pObject->GetID() << "]";
		}
	}
	else
	{
		ss << "null";
	}	
	SetValue(wxVariant(ss.str()));
}

//! implement the reference property editor
WX_PG_IMPLEMENT_EDITOR_CLASS(ReferencePropertyEditor, wxReferencePropertyEditor, wxPGTextCtrlEditor)

wxPGWindowList wxReferencePropertyEditor::CreateControls( wxPropertyGrid* propGrid,
														  wxPGProperty* property,
														  const wxPoint& pos,
														  const wxSize& sz ) const
{
    // Create and populate buttons-subwindow
    wxPGMultiButton* buttons = new wxPGMultiButton( propGrid, sz );
    buttons->Add("+"); // create new
    buttons->Add("-"); // clear
	buttons->Add("b"); // browse existing
	buttons->Add("s"); // save

    // Create the 'primary' editor control (textctrl in this case)
    wxPGWindowList wndList = wxPGTextCtrlEditor::CreateControls( propGrid, property, pos, buttons->GetPrimarySize());

    // Finally, move buttons-subwindow to correct position and make sure
    // returned wxPGWindowList contains our custom button list.
    buttons->FinalizePosition(pos);

    wndList.SetSecondary( buttons );
    return wndList;
}

bool wxReferencePropertyEditor::OnEvent( wxPropertyGrid* propGrid,
										 wxPGProperty* property,
										 wxWindow* ctrl,
										 wxEvent& event ) const
{
    if ( event.GetEventType() == wxEVT_COMMAND_BUTTON_CLICKED )
    {
        wxPGMultiButton* buttons = (wxPGMultiButton*) propGrid->GetEditorControlSecondary();
		wxReferenceProperty* pRefProperty = static_cast<wxReferenceProperty*>(property);

		if(event.GetId() == buttons->GetButtonId(0)) // + set reference
		{
			if(pRefProperty->GetChildCount() == 0)
			{				
				// update the corresponding property in the property stream
				wxPropertyGridEvent PGEvent;
				PGEvent.SetProperty(property);
				pRefProperty->SetEventType(wxReferenceProperty::E_PropertySetNew);				
				pRefProperty->GetParent()->OnPropertyGridChange(PGEvent);								
			}			
		}	
		else if(event.GetId() == buttons->GetButtonId(1)) // - clear reference
		{
			int childCount = property->GetChildCount();
			if(childCount > 0)
			{			
				// remove the wx property
				property->DeleteChildren();			
				pRefProperty->UpdateValue(NULL);
				propGrid->RefreshProperty(property);				

				// update the corresponding property in the property stream
				wxPropertyGridEvent PGEvent;
				PGEvent.SetProperty(property);
				pRefProperty->SetEventType(wxReferenceProperty::E_PropertyCleared);
				pRefProperty->GetParent()->OnPropertyGridChange(PGEvent);
			}
		}
		else if(event.GetId() == buttons->GetButtonId(2)) // b - browse existing
		{
			if(pRefProperty->GetChildCount() == 0)
			{		
				std::string path;			
				if(EditorUtils::OpenFileDialog(pRefProperty->GetParent(), path, "Object files (*.xml)|*.xml", "Select Object"))
				{
					// update the corresponding property in the property stream
					wxPropertyGridEvent PGEvent;
					PGEvent.SetProperty(property);
					pRefProperty->SetEventType(wxReferenceProperty::E_PropertySetFromTemplate);
					pRefProperty->SetTemplatePath(path);
					pRefProperty->GetParent()->OnPropertyGridChange(PGEvent);		
				}										
			}				
		}	
		else if(event.GetId() == buttons->GetButtonId(3)) // s - save to file
		{
			if(pRefProperty->GetChildCount() != 0)
			{
				// update the corresponding property in the property stream
				wxPropertyGridEvent PGEvent;
				PGEvent.SetProperty(property);
				pRefProperty->SetEventType(wxReferenceProperty::E_PropertySaved);				
				pRefProperty->GetParent()->OnPropertyGridChange(PGEvent);		
			}
		}

		return true;
    }
	else
	{
		return wxPGTextCtrlEditor::OnEvent(propGrid, property, ctrl, event);
	}    
}


