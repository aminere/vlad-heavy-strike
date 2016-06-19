/* 

Amine Rehioui
Created: January 16th 2012

*/

#ifndef _OBJECT_INSPECTOR_H_INCLUDED_
#define _OBJECT_INSPECTOR_H_INCLUDED_

#include "wx/treectrl.h"
#include "PropertyListContainer.h"

namespace shoot
{
	// forwards
	class Entity;

	//! Inspects entities
	class ObjectInspector : public wxPanel
						  , public PropertyListListener
	{
		typedef wxPanel super;

	public:

		//! Constructor
		ObjectInspector(wxWindow* pParent,	
					    wxWindowID id = wxID_ANY, 
					    const wxPoint& pos = wxDefaultPosition, 
					    const wxSize& size = wxSize(250, 300), 
					    long style = 0);

		//! Destructor
		virtual ~ObjectInspector()
		{
		}

		//! Fills the object inspector
		void Fill(Object* pObject);

		//! updates a property
		void UpdateProperty(const char* strPropertyName, wxVariant& value, Entity* _pEntity = NULL);

		//! returns the property list
		PropertyList* GetPropertyList() { return m_pPropertyList; }

		// PropertyListListener interface //

		//! call on property change
		void OnPropertyChanged(wxPropertyGridEvent &event);

		//! called while a property is changing
		void OnPropertyChanging(wxPropertyGridEvent &event);

		//! called while a property is selected
		void OnPropertySelected(wxPropertyGridEvent &event);

		//! called while a property is right clicked
		void OnPropertyRightClick(wxPropertyGridEvent &event);

	private:

		//! loads an object from a path
		void Load(Object* pObject, const std::string& path);

		//! adds a component to an entity
		void AddComponent(Entity* pEntity, Component* pComponent);

		wxTreeCtrl* m_pTreeControl;
		PropertyList* m_pPropertyList;
		PropertyListContainer* m_pPropertyListContainer;
		wxButton* m_pReloadButton;
		wxButton* m_pComponentCopyButton;
		wxButton* m_pComponentPasteButton;
		Reference<Component> m_ComponentClipboard;

		//! struct to map between the tree items and the components
		struct ItemData : public wxTreeItemData
		{
			ItemData(Object* pObject) : m_pObject(pObject)
			{
			}
			Object* m_pObject;
		};

		// event handlers
		void OnLoad(wxCommandEvent& event);
		void OnSave(wxCommandEvent& event);
		void OnSaveAs(wxCommandEvent& event);
		void OnReload(wxCommandEvent& event);		
		void OnAddComponent(wxCommandEvent& event);
		void OnRemoveComponent(wxCommandEvent& event);
		void OnCopyComponent(wxCommandEvent& event);
		void OnPasteComponent(wxCommandEvent& event);
		void OnSelectItem(wxTreeEvent& event);
		void OnPropertyMenuClicked(wxEvent& event);

		//!PropertyMenuData
		struct PropertyMenuData
		{
			Object* pObject;
			wxPGProperty* pWxProperty;
			u32 propertyIndex;
		};

		PropertyMenuData m_PropertyMenuData;

		DECLARE_EVENT_TABLE();
	};
}

#endif // _OBJECT_INSPECTOR_H_INCLUDED_


