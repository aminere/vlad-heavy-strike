/* 

Amine Rehioui
Created: May 24th 2010

*/

#ifndef _ENTITY_SELECTOR_H_INCLUDED_
#define _ENTITY_SELECTOR_H_INCLUDED_

#include <wx/dialog.h>
#include <wx/listctrl.h>

namespace shoot
{
	//! A dialog to select an entity
	class EntitySelector : public wxDialog
	{
		typedef wxDialog super;

	public:
		
		//! return true to close the EntitySelector
		typedef bool EntitySelectedCallback(u32 entityID);

		//! Constructor
		EntitySelector(wxWindow* pParent,
					   EntitySelectedCallback* pCallBack,
					   const char* strBaseType = "Entity",
					   const wxSize& size = wxSize(640, 480),					   
					   wxWindowID id = wxID_ANY,
					   const wxString& title = "Entity Selector",
					   const wxPoint& pos = wxDefaultPosition,					    
					   long style = wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);

		//! Destructor
		virtual ~EntitySelector()
		{
		}

		// event handlers
		void OnSelectButtonClicked(wxCommandEvent& event);
		void OnCancelClicked(wxCommandEvent& event);
		void OnListItemSelected(wxListEvent& event);		

	private:

		wxListView* m_pEntityList;
		u32 m_uiCurrentItem;
		u32 m_selectedItemID;

		EntitySelectedCallback* m_pCallBack;

		//! recursively fill the list with entities
		void Fill(Entity* pEntity, std::vector<std::string>& typeFilter);

		DECLARE_EVENT_TABLE();  
	};
}

#endif // _ENTITY_SELECTOR_H_INCLUDED_


