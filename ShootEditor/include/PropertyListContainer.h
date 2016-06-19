/* 

Amine Rehioui
Created: March 18th 2012

*/

#ifndef _PROPERTY_LIST_CONTAINER_H_INCLUDED_
#define _PROPERTY_LIST_CONTAINER_H_INCLUDED_

#include "wx/checkbox.h"

#include "PropertyList.h"

namespace shoot
{
	//! Property list container
	class PropertyListContainer : public wxPanel
	{
		typedef wxPanel super;

	public:

		//! Constructor
		PropertyListContainer(wxWindow* pParent,	
							  wxWindowID id = wxID_ANY, 
							  const wxPoint& pos = wxDefaultPosition, 
							  const wxSize& size = wxSize(250, 300), 
							  long style = 0);

		//! Destructor
		virtual ~PropertyListContainer()
		{
		}

		//! Fills the container with the properties from an object
		void Fill(Object* pObject);

		//! Clears the container
		void Clear();

		//! returns the property list
		PropertyList* GetPropertyList() { return m_pPropertyList; }

		//! called when a property is selected
		void OnPropertySelected(wxPGProperty* pWxProperty);

	private:

		PropertyList* m_pPropertyList;
		wxCheckBox* m_pOverrideToggle;

		// event handlers
		void OnToggleOverride(wxCommandEvent& event);

		DECLARE_EVENT_TABLE();
	};
}

#endif // _PROPERTY_LIST_CONTAINER_H_INCLUDED_

