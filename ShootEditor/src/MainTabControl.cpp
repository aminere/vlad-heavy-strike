/* 

Amine Rehioui
Created: March 13th 2011

*/

#include "ShootEditorCommon.h"

#include "MainTabControl.h"

#include "TabContainer.h"

#include "ObjectViewer.h"

namespace shoot
{
	// Define event table
	BEGIN_EVENT_TABLE(MainTabControl, wxAuiNotebook)
		EVT_AUINOTEBOOK_PAGE_CHANGING(wxID_ANY, OnTabChanging)
		EVT_AUINOTEBOOK_PAGE_CHANGED(wxID_ANY, OnTabChanged)
		EVT_AUINOTEBOOK_PAGE_CLOSE(wxID_ANY, OnTabClose)
	END_EVENT_TABLE()

	//! Constructor
	MainTabControl::MainTabControl(wxWindow* pParent,
								   long style /*= wxAUI_NB_DEFAULT_STYLE*/,
								   wxWindowID id /*= wxID_ANY*/, 
								   const wxPoint& pos /*= wxDefaultPosition*/, 
								   const wxSize& size /*= wxSize(800, 600)*/) 
	: super(pParent, id, pos, size, style)
	{
	}

	//! Initializes the control
	void MainTabControl::Init()
	{			
		// create a default tab		
		TabContainer* pTabContainer = new TabContainer(this);
		AddPage(pTabContainer, "New", true);

		AddPage(new ObjectViewer(this), "ObjectViewer", false);
	}

	// event handlers
	void MainTabControl::OnTabChanging(wxAuiNotebookEvent& evt)
	{
		evt.Skip();
	}

	void MainTabControl::OnTabChanged(wxAuiNotebookEvent& evt)
	{
		s32 tabIndex = GetSelection();
		if(tabIndex < s32(GetPageCount())-1)
		{
			TabContainer* pTabContainer = static_cast<TabContainer*>(this->GetPage(tabIndex));
			pTabContainer->OnSelected();
		}		
	}

	void MainTabControl::OnTabClose(wxAuiNotebookEvent& evt)
	{		
		evt.Veto();		
	}
}
