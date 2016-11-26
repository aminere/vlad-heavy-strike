/* 

Amine Rehioui
Created: January 14th 2012

*/	

#include "ShootEditorCommon.h"

#include "PropertyListContainer.h"

#include "wx/sizer.h"
#include "wx/checkbox.h"

namespace shoot
{
	// Define event table
	BEGIN_EVENT_TABLE(PropertyListContainer, wxPanel)				
		EVT_CHECKBOX(ID_PropertyOverride, OnToggleOverride)
	END_EVENT_TABLE()

	//! Constructor
	PropertyListContainer::PropertyListContainer(wxWindow* pParent,	
												 wxWindowID id /*= wxID_ANY*/, 
												 const wxPoint& pos /*= wxDefaultPosition*/, 
												 const wxSize& size /*= wxSize(250, 300)*/, 
												 long style /*= 0*/) 
	: super(pParent, id, pos, size, style)
	{
		wxSize buttonSize = wxSize(24, 22);
		wxBoxSizer *pCommands = new wxBoxSizer( wxHORIZONTAL );
		m_pOverrideToggle = new wxCheckBox(this, ID_PropertyOverride, "Override");
		m_pOverrideToggle->Enable(false);
		pCommands->Add(m_pOverrideToggle); 		
				
		ObjectInspector* pInspector = static_cast<ObjectInspector*>(pParent);
		PropertyListListener* pListener = static_cast<PropertyListListener*>(pInspector);
		m_pPropertyList = new PropertyList(this, pListener);

		wxBoxSizer *pSizer = new wxBoxSizer( wxVERTICAL );
		pSizer->Add(m_pPropertyList, wxSizerFlags(1).Expand());
		pSizer->Add(pCommands);		

		SetSizer(pSizer);
		pSizer->FitInside(this);
	}	

	//! Fills the container with the properties from an object
	void PropertyListContainer::Fill(Object* pObject)
	{
		m_pPropertyList->Fill(pObject);
		m_pOverrideToggle->Enable(false);		
	}

	//! Clears the container
	void PropertyListContainer::Clear()
	{
		m_pPropertyList->Clear();
		m_pOverrideToggle->Enable(false);
	}

	//! called when a property is selected
	void PropertyListContainer::OnPropertySelected(wxPGProperty* pWxProperty)
	{
		if( pWxProperty
		 && pWxProperty->GetParent()->IsRoot()
		 && pWxProperty->GetLabel() != "ID"
		 && pWxProperty->GetLabel() != "Type"
		 && m_pPropertyList->GetSelectedObject()->IsA(Entity::TypeID))
		{
			Entity* pEntity = DYNAMIC_CAST(m_pPropertyList->GetSelectedObject(), Entity);
			bool bIsTemplate = pEntity && !pEntity->GetTemplatePath().empty();			
			bool bOverriden = (pEntity->GetOverridenProperties().GetProperty(std::string(pWxProperty->GetName())) != NULL);
			m_pOverrideToggle->Enable(bIsTemplate);
			m_pOverrideToggle->SetValue(bOverriden);
		}
		else
		{
			m_pOverrideToggle->Enable(false);
		}
	}

	// event handlers
	void PropertyListContainer::OnToggleOverride(wxCommandEvent& event)
	{
		SHOOT_ASSERT(m_pPropertyList->GetSelectedObject()->IsA(Entity::TypeID), "Tring to override a property of a non-entity");
		Entity* pEntity = static_cast<Entity*>(m_pPropertyList->GetSelectedObject());
		bool bOverride = event.IsChecked();
		wxPGProperty* pWxProperty = m_pPropertyList->GetSelection();
		SHOOT_ASSERT(pWxProperty, "Toggling override flag on NULL property");
		const char* strPropertyName = pWxProperty->GetLabel().c_str();
		PropertyStream& overridenProperties = pEntity->GetOverridenProperties();
		if(bOverride)
		{
			PropertyStream stream(SM_Write);
			pEntity->Serialize(stream);
			overridenProperties.AddProperty(stream.GetProperty(strPropertyName)->Copy());
			m_pPropertyList->EnableProperty(pWxProperty);
		}
		else
		{
			overridenProperties.RemoveProperty(overridenProperties.GetProperty(strPropertyName));
			m_pPropertyList->DisableProperty(pWxProperty);
		}
	}
}

