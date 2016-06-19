/* 

Amine Rehioui
Created: March 13th 2011

*/

#ifndef _MAIN_TAB_CONTROL_H_INCLUDED_
#define _MAIN_TAB_CONTROL_H_INCLUDED_

#include <wx/aui/auibook.h>

namespace shoot
{
	//! Main Tab Control
	class MainTabControl : public wxAuiNotebook							   
	{
		typedef wxAuiNotebook super;

	public:
		
		//! Constructor
		MainTabControl(wxWindow* pParent,
					   long style = wxAUI_NB_DEFAULT_STYLE,
					   wxWindowID id = wxID_ANY, 
					   const wxPoint& pos = wxDefaultPosition, 
					   const wxSize& size = wxSize(800, 600));
	
		//! Initializes the control
		void Init();

		//! Destructor
		virtual ~MainTabControl()
		{
		}		

	private:

		// event handlers
		void OnTabChanging(wxAuiNotebookEvent& evt);
		void OnTabChanged(wxAuiNotebookEvent& evt);
		void OnTabClose(wxAuiNotebookEvent& evt);

		DECLARE_EVENT_TABLE();
	};
}

#endif // _MAIN_TAB_CONTROL_H_INCLUDED_

