/* 

Amine Rehioui
Created: April 10th 2010

*/

#ifndef _PROPERTY_LIST_H_INCLUDED_
#define _PROPERTY_LIST_H_INCLUDED_

namespace shoot
{
	// forwards
	class Entity;

	//! property list event listener
	class PropertyListListener
	{
	public:

		//! call on property change
		virtual void OnPropertyChanged(wxPropertyGridEvent &event) = 0;

		//! called while a property is changing
		virtual void OnPropertyChanging(wxPropertyGridEvent &event) = 0;

		//! called while a property is selected
		virtual void OnPropertySelected(wxPropertyGridEvent &event) = 0;

		//! called while a property is right clicked
		virtual void OnPropertyRightClick(wxPropertyGridEvent &event) { }		
	};

	//! A class that allows the edition of shoot properties through a visual property list
	class PropertyList : public wxPropertyGrid
	{
		typedef wxPropertyGrid super;

	public:
		
		//! Constructor
		PropertyList(wxWindow* pParent,	
					 PropertyListListener* pListener,
					 wxWindowID id = wxID_ANY, 
					 const wxPoint& pos = wxDefaultPosition, 
					 const wxSize& size = wxSize(250, 300), 
					 long style = wxPG_SPLITTER_AUTO_CENTER | wxPG_BOLD_MODIFIED, 
					 const wxString& name = wxPropertyGridNameStr);

		//! Destructor
		virtual ~PropertyList()
		{
		}

		//! Fills the list with the properties from an object
		void Fill(Object* pObject);

		//! creates a wx property from a shoot property
		static wxPGProperty* GetWxProperty(IProperty* pProperty, PropertyList* pInstance);

		//! fills a shoot property from a wx property
		static void FillProperty(wxPGProperty* pWxProperty, IProperty* pProperty);

		//! recursively fills sub array properties
		/** Array properties must be filled after they are added to the list */
		void FillArrayProperties(IProperty* pProperty, wxPGProperty* pWxProperty);

		//! returns the selected object
		Object* GetSelectedObject() { return m_pSelectedObject; }

		wxPGEditor* ArrayEditor;
		wxPGEditor* ReferenceEditor;

		// Event handlers
		void OnPropertyGridChange(wxPropertyGridEvent& event); 
		void OnPropertyGridChanging(wxPropertyGridEvent& event);
		void OnPropertySelected(wxPropertyGridEvent& event);
		void OnPropertyRightClick(wxPropertyGridEvent& event);		

	private:

		PropertyListListener* m_pListener;
		Object* m_pSelectedObject;

		DECLARE_EVENT_TABLE();  
	};
}

#endif // _PROPERTY_LIST_H_INCLUDED_


