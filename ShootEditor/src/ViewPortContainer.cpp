/* 

Amine Rehioui
Created: August 9th 2010

*/

#include "ShootEditorCommon.h"

#include "ViewPortContainer.h"

#include "EditorSettings.h"

namespace shoot
{
	// Define event table
	BEGIN_EVENT_TABLE(ViewPortContainer, wxPanel)		
		EVT_SIZE(ViewPortContainer::OnResized)		
	END_EVENT_TABLE()

	//! Constructor
	ViewPortContainer::ViewPortContainer(wxWindow *parent)
		: super(parent, wxID_ANY, wxDefaultPosition, wxSize(800, 600))
	{
		m_pViewPort = new ViewPort(this);
	}

	// event handlers
	void ViewPortContainer::OnResized(wxSizeEvent& event)
	{
		event.Skip();

		if(EditorSettings::Instance()->WindowSize == EditorSettings::WS_Stretch)
		{
			m_pViewPort->SetPosition(wxPoint(0, 0));
			m_pViewPort->SetSize(event.GetSize());
		}
		else
		{
			Size size = EditorSettings::WindowSizes[EditorSettings::Instance()->WindowSize];
			wxPoint center = wxPoint((GetSize().x - size.Width)/2, (GetSize().y - size.Height)/2);
			m_pViewPort->SetPosition(center);
			m_pViewPort->SetSize(wxSize(size.Width, size.Height));
		}		
	}
}
