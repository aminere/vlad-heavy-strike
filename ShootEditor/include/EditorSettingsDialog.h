/* 

Amine Rehioui
Created: August 7th 2010

*/

#ifndef _EDITOR_SETTINGS_DIALOG_H_INCLUDED_
#define _EDITOR_SETTINGS_DIALOG_H_INCLUDED_

#include <wx/dialog.h>

#include "PropertyList.h"

namespace shoot
{
	//! Dialog to edit editor settings
	class EditorSettingsDialog : public wxDialog
							   , public PropertyListListener
	{
		typedef wxDialog super;

	public:
		
		//! Constructor
		EditorSettingsDialog(wxWindow* pParent);

	private:

		//! call on property change
		void OnPropertyChanged(wxPropertyGridEvent &event);

		//! called while a property is changing
		void OnPropertyChanging(wxPropertyGridEvent &event);

		//! called while a property is selected
		void OnPropertySelected(wxPropertyGridEvent &event) { } // TODO

		DECLARE_EVENT_TABLE();  
	};
}

#endif // _EDITOR_SETTINGS_DIALOG_H_INCLUDED_


