/* 

Amine Rehioui
Created: January 14th 2012

*/	

#include "ShootEditorCommon.h"

#include "PropertyLocator.h"
#include "wxArrayProperty.h"
#include "wxReferenceProperty.h"
#include "Visitor.h"
#include "EditorUtils.h"
#include "PropertyListContainer.h"
#include "Image.h"

#include "IconManager.h"
#include "EventManager.h"
#include "EditorEvents.h"
#include "ObjectSelector.h"
#include "UndoManager.h"

#include "wx/sizer.h"
#include <wx/bmpbuttn.h>
#include <wx/artprov.h> // wxArtProvider

#include "tinyxml/tinyxml2.h"

namespace shoot
{
	// Define event table
	BEGIN_EVENT_TABLE(ObjectInspector, wxPanel)				
		EVT_COMMAND(ID_ObjectInspector_Load, wxEVT_COMMAND_BUTTON_CLICKED, OnLoad)
		EVT_COMMAND(ID_ObjectInspector_Save, wxEVT_COMMAND_BUTTON_CLICKED, OnSave)
		EVT_COMMAND(ID_ObjectInspector_SaveAs, wxEVT_COMMAND_BUTTON_CLICKED, OnSaveAs)
		EVT_COMMAND(ID_ObjectInspector_Reload, wxEVT_COMMAND_BUTTON_CLICKED, OnReload)
		EVT_COMMAND(ID_ObjectInspector_AddComponent, wxEVT_COMMAND_BUTTON_CLICKED, OnAddComponent)
		EVT_COMMAND(ID_ObjectInspector_RemoveComponent, wxEVT_COMMAND_BUTTON_CLICKED, OnRemoveComponent)
		EVT_COMMAND(ID_ObjectInspector_CopyComponent, wxEVT_COMMAND_BUTTON_CLICKED, OnCopyComponent)
		EVT_COMMAND(ID_ObjectInspector_PasteComponent, wxEVT_COMMAND_BUTTON_CLICKED, OnPasteComponent)		
		EVT_TREE_SEL_CHANGED(ID_ObjectInspector_SelectItem, OnSelectItem)
	END_EVENT_TABLE()

	//! Constructor
	ObjectInspector::ObjectInspector(wxWindow* pParent,	
								     wxWindowID id /*= wxID_ANY*/, 
								     const wxPoint& pos /*= wxDefaultPosition*/, 
								     const wxSize& size /*= wxSize(250, 300)*/, 
								     long style /*= 0*/) 
	: super(pParent, id, pos, size, style)
	{
		IconManager* pIconMgr = IconManager::Instance();

		wxSize buttonSize = wxSize(24, 22);
		wxBoxSizer *pCommands = new wxBoxSizer( wxHORIZONTAL );
		pCommands->Add(new wxBitmapButton(this, ID_ObjectInspector_Load, wxArtProvider::GetBitmap(wxART_FILE_OPEN), wxDefaultPosition, buttonSize)); 		
		pCommands->Add(new wxBitmapButton(this, ID_ObjectInspector_Save, wxArtProvider::GetBitmap(wxART_FILE_SAVE), wxDefaultPosition, buttonSize));
		pCommands->Add(new wxBitmapButton(this, ID_ObjectInspector_SaveAs, wxArtProvider::GetBitmap(wxART_FILE_SAVE_AS), wxDefaultPosition, buttonSize));
		pCommands->Add(m_pReloadButton = new wxButton(this, ID_ObjectInspector_Reload, "R", wxDefaultPosition, buttonSize), 0, wxALIGN_CENTRE_VERTICAL);		
		pCommands->Add(new wxBitmapButton(this, ID_ObjectInspector_AddComponent, pIconMgr->GetBitmap(IconManager::Icon_Add), wxDefaultPosition, buttonSize)); 		
		pCommands->Add(new wxBitmapButton(this, ID_ObjectInspector_RemoveComponent, pIconMgr->GetBitmap(IconManager::Icon_Remove), wxDefaultPosition, buttonSize));
		pCommands->Add(m_pComponentCopyButton = new wxButton(this, ID_ObjectInspector_CopyComponent, "C", wxDefaultPosition, buttonSize), 0, wxALIGN_CENTRE_VERTICAL);
		pCommands->Add(m_pComponentPasteButton = new wxButton(this, ID_ObjectInspector_PasteComponent, "P", wxDefaultPosition, buttonSize), 0, wxALIGN_CENTRE_VERTICAL);

		m_pTreeControl = new wxTreeCtrl(this, ID_ObjectInspector_SelectItem, wxDefaultPosition, wxSize(120, 120), wxTR_DEFAULT_STYLE);
		m_pTreeControl->SetImageList(pIconMgr->GetImageList());
		
		m_pPropertyListContainer = new PropertyListContainer(this);
		m_pPropertyList = m_pPropertyListContainer->GetPropertyList();

		wxBoxSizer *pSizer = new wxBoxSizer( wxVERTICAL );
		pSizer->Add(pCommands);
		pSizer->Add(m_pTreeControl, wxSizerFlags(0).Expand().Border(wxBOTTOM | wxTOP, 2));		
		pSizer->Add(m_pPropertyListContainer, wxSizerFlags(1).Expand());

		SetSizer(pSizer);
		pSizer->FitInside(this);
	}

	//! Fills the object inspector
	void ObjectInspector::Fill(Object* pObject)
	{
		m_pTreeControl->DeleteAllItems();
		m_pPropertyListContainer->Clear();		

		if(pObject)		
		{
			// fill entity
			std::string rootName = pObject->GetClassName();
			if(pObject->GetTemplatePath().length())
			{
				rootName = rootName + " (" + pObject->GetTemplatePath() + ")";
			}
			wxTreeItemId rootItem = m_pTreeControl->AddRoot(rootName, IconManager::Instance()->GetIconIndex(pObject), IconManager::Instance()->GetIconIndex(pObject), new ItemData(pObject));

			if(Entity* pEntity = DYNAMIC_CAST(pObject, Entity))
			{
				// fill components
				u32 numComponents = pEntity->GetComponentCount();
				for(u32 i=0; i<numComponents; ++i)
				{
					Component* pComponent = pEntity->GetComponent(i);
					wxString itemName = pComponent->GetClassName();
					std::string strTemplatePath = pComponent->GetTemplatePath();
					if(!strTemplatePath.empty())
					{
						itemName += " [ref]";
					}
					m_pTreeControl->AppendItem(rootItem, itemName, IconManager::Instance()->GetIconIndex(pComponent), IconManager::Instance()->GetIconIndex(pComponent), new ItemData(pComponent));
				}
			}

			m_pTreeControl->Expand(rootItem);
			m_pTreeControl->SelectItem(rootItem);
		}
		else
		{
			m_pReloadButton->Enable(false);
			m_pComponentCopyButton->Enable(false);
			m_pComponentPasteButton->Enable(false);
		}
	}

	//! updates a property
	void ObjectInspector::UpdateProperty(const char* strPropertyName, wxVariant& value, Entity* _pEntity /*= NULL*/)
	{
		Entity* pEntity = _pEntity ? _pEntity : ShootEditor::Instance()->GetSelectedEntity();

		// make property overriden if not already done
		if(pEntity
		&& !pEntity->GetTemplatePath().empty()
		&& !pEntity->GetOverridenProperties().GetProperty(strPropertyName))
		{
			PropertyStream stream(SM_Write);
			pEntity->Serialize(stream);
			pEntity->GetOverridenProperties().AddProperty(stream.GetProperty(strPropertyName)->Copy());
		}

		if(ShootEditor::Instance()->GetSelectedEntity()
		&& (!_pEntity || (_pEntity == ShootEditor::Instance()->GetSelectedEntity()))
		&& m_pTreeControl->GetSelection() == m_pTreeControl->GetRootItem())
		{
			// make sure we're updating the selected entitie's property list
			m_pPropertyList->SetPropertyValue(strPropertyName, value); 
			m_pPropertyList->EnableProperty(strPropertyName);
		}
	}

	//! loads an object from a path
	void ObjectInspector::Load(Object* pObject, const std::string& path)
	{
		if(Entity* pEntity = DYNAMIC_CAST(pObject, Entity))
		{
			pEntity->LoadFromXML(path.c_str());
			pEntity->Init();

			// update name an children in tree
			ShootEditor* pEditor = ShootEditor::Instance();
			wxTreeItemId itemId = pEditor->GetTreeView()->GetItemIDFromEntity(pEditor->GetTreeView()->GetRootItem(), pEntity);
			if(itemId.IsOk())
			{
				std::string name = pEntity->GetName().empty() ? pEntity->GetClassName() : pEntity->GetName();
				pEditor->GetTreeView()->SetItemText(itemId, name);
				pEditor->GetTreeView()->RefreshEntity(pEntity);
			}

			// refresh new entity
			Fill(pEntity);
		}
		else if(Component* pComponent = DYNAMIC_CAST(pObject, Component))
		{
			tinyxml2::XMLDocument document;
			SHOOT_ASSERT(!document.LoadFile(path.c_str()), "Could not load file '%s'", path.c_str());
			tinyxml2::XMLElement* pRoot = document.FirstChildElement();

			pComponent->LoadFromXML(pRoot);
			pComponent->Init();

			// refresh new component
			m_pPropertyList->Fill(pObject);
		}
	}

	//! adds a component to an entity
	void ObjectInspector::AddComponent(Entity* pEntity, Component* pComponent)
	{
		pEntity->AddComponent(pComponent, true);
		pComponent->Init();

		m_pTreeControl->AppendItem(m_pTreeControl->GetRootItem(), pComponent->GetClassName(), IconManager::Instance()->GetIconIndex(pComponent), IconManager::Instance()->GetIconIndex(pComponent), new ItemData(pComponent));
		m_pTreeControl->Expand(m_pTreeControl->GetRootItem());
	}

	// event handlers
	void ObjectInspector::OnLoad(wxCommandEvent& event)
	{
		if(Entity* pEntity = ShootEditor::Instance()->GetSelectedEntity())
		{
			std::string path;
			if(EditorUtils::OpenFileDialog(this, path, "Object files (*.xml)|*.xml", "Select Object"))
			{
				Load(pEntity, path);
			}
		}
	}

	void ObjectInspector::OnReload(wxCommandEvent& event)
	{
		if(Object* pObject = m_pPropertyList->GetSelectedObject())
		{
			std::string path = pObject->GetTemplatePath();
			SHOOT_ASSERT(!path.empty(), "Object to reload has no template path");
			Load(pObject, path);

			if(Entity* pEntity = DYNAMIC_CAST(pObject, Entity))
			{
				pEntity->Serialize(pEntity->GetOverridenProperties());

				if(Context* pContext = pEntity->GetRootContext())
				{
					QuadTree* pQuadTree = pContext->GetQuadTree();
					if(pQuadTree && pEntity->IsA(Entity3D::TypeID))
					{
						pQuadTree->ExtractEntities(pEntity);
						pQuadTree->Rebuild();
					}
				}
			}
		}
	}

	void ObjectInspector::OnSave(wxCommandEvent& event)
	{		
		if(Object* pObject = m_pPropertyList->GetSelectedObject())
		{
			std::string path = pObject->GetTemplatePath();
			bool bExists = !pObject->GetTemplatePath().empty();
			if(bExists || EditorUtils::OpenFileDialog(this, path, "Object files (*.xml)|*.xml", "Save File", wxFD_SAVE))			
			{
				EditorUtils::FormatObjectPath(pObject, path);
				pObject->SaveToXML(path.c_str());

				if(Entity* pEntity = DYNAMIC_CAST(pObject, Entity))
				{
					pEntity->GetOverridenProperties().Clear();
				}
			}
		}
	}

	void ObjectInspector::OnSaveAs(wxCommandEvent& event)
	{		
		if(Object* pObject = m_pPropertyList->GetSelectedObject())
		{
			std::string path;
			if(EditorUtils::OpenFileDialog(this, path, "Object files (*.xml)|*.xml", "Save File", wxFD_SAVE))			
			{
				EditorUtils::FormatObjectPath(pObject, path);
				pObject->SaveToXML(path.c_str());
			}
		}
	}

	void ObjectInspector::OnAddComponent(wxCommandEvent& event)
	{
		if(Entity* pEntity = ShootEditor::Instance()->GetSelectedEntity())
		{
			Object* pObject = NULL;

			if(wxMessageBox("Add from a template?", "Add from a template?", wxYES_NO | wxICON_QUESTION) == wxYES)
			{
				std::string path;
				if(EditorUtils::OpenFileDialog(this, path, "Object files (*.xml)|*.xml", "Select Object"))
				{
					pObject = ObjectManager::Instance()->Get(path.c_str());
				}
			}
			else
			{
				std::string className;
				ObjectSelector selector(this);
				if(selector.Select(Component::GetStaticClassName(), className))
				{
					pObject = ObjectFactory::Instance()->Create(className);			
				}
			}

			if(pObject)
			{
				Component* pComponent = DYNAMIC_CAST(pObject, Component);
				SHOOT_ASSERT(pComponent, "Cannot create an Component from Object");
				AddComponent(pEntity, pComponent);
			}
		}
	}

	void ObjectInspector::OnRemoveComponent(wxCommandEvent& event)
	{
		if(m_pTreeControl->GetRootItem().IsOk()
		&& m_pTreeControl->GetChildrenCount(m_pTreeControl->GetRootItem()) > 0)
		{
			Entity* pEntity = ShootEditor::Instance()->GetSelectedEntity();
			SHOOT_ASSERT(pEntity, "Cannot RemoveComponent on NULL entity");
			wxTreeItemId selectedItem = m_pTreeControl->GetSelection();			
			if( selectedItem.IsOk()
			&& (selectedItem != m_pTreeControl->GetRootItem()))
			{
				if( wxMessageBox("Do you really want to remove the selected component?",
					"Comfirm removal",
					wxYES_NO | wxICON_QUESTION) == wxYES )
				{
					ItemData* pItemData = static_cast<ItemData*>(m_pTreeControl->GetItemData(selectedItem));
					Component* pComponent = static_cast<Component*>(pItemData->m_pObject);
					pEntity->RemoveComponent(pComponent, true);

					m_pTreeControl->Delete(selectedItem);					
					m_pTreeControl->SelectItem(m_pTreeControl->GetRootItem());
				}
			}
		}
	}

	void ObjectInspector::OnCopyComponent(wxCommandEvent& event)
	{
		SHOOT_ASSERT(m_pPropertyList->GetSelectedObject()->IsA(Component::TypeID), "Called OnCopyComponent on a non-component");
		m_ComponentClipboard = static_cast<Component*>(m_pPropertyList->GetSelectedObject()->Copy());
	}

	void ObjectInspector::OnPasteComponent(wxCommandEvent& event)
	{
		SHOOT_ASSERT(m_pPropertyList->GetSelectedObject()->IsA(Entity::TypeID), "Called OnPasteComponent on a non-entity");
		AddComponent(static_cast<Entity*>(m_pPropertyList->GetSelectedObject()), static_cast<Component*>(m_ComponentClipboard->Copy()));
	}

	void ObjectInspector::OnSelectItem(wxTreeEvent& event)
	{
		wxTreeItemId itemId = event.GetItem();
		if(itemId.IsOk())
		{
			Object* pObject = static_cast<ItemData*>(m_pTreeControl->GetItemData(itemId))->m_pObject;
			m_pPropertyList->Fill(pObject);

			bool bIsTemplateInstance = !pObject->GetTemplatePath().empty();
			m_pReloadButton->Enable(bIsTemplateInstance);
			m_pComponentCopyButton->Enable(pObject->IsA(Component::TypeID));
			m_pComponentPasteButton->Enable(m_ComponentClipboard.IsValid() && pObject->IsA(Entity::TypeID));
		}
	}

	void ObjectInspector::OnPropertyMenuClicked(wxEvent& event)
	{
		PropertyStream stream(SM_Write);
		m_PropertyMenuData.pObject->Serialize(stream);
		
		PropertyLocator locator;
		IProperty* pProperty = locator.GetProperty(m_PropertyMenuData.pWxProperty, stream);

		switch(event.GetId())
		{
		case ID_Property_Remove:
			{
				wxPGProperty* pWxParentProperty = m_PropertyMenuData.pWxProperty->GetParent();				
				wxArrayProperty* pWxArrayProperty = static_cast<wxArrayProperty*>(m_PropertyMenuData.pWxProperty);
				ArrayProperty* pArrayProperty = static_cast<ArrayProperty*>(pProperty);
				u32 indexInParent = pWxArrayProperty->GetIndexInParent();
				m_pPropertyList->DeleteProperty(pWxArrayProperty);

				// delete the property
				IProperty* pSubProperty = pArrayProperty->GetProperties()[m_PropertyMenuData.propertyIndex];
				pArrayProperty->GetProperties().erase(pArrayProperty->GetProperties().begin()+m_PropertyMenuData.propertyIndex);
				delete pSubProperty;

				// delete the corresponding element in the user data
				IArray* pArray = pArrayProperty->GetArray();
				pArray->Delete(m_PropertyMenuData.propertyIndex);				

				wxPGProperty* pNewProperty = PropertyList::GetWxProperty(pArrayProperty, m_pPropertyList);
				m_pPropertyList->Insert(pWxParentProperty, indexInParent, pNewProperty);
				m_pPropertyList->FillArrayProperties(pArrayProperty, pNewProperty);
			}
			break;

		case ID_Property_Copy:
			{
				// TODO
			}
			break;

 		case ID_Property_MoveUp:
		case ID_Property_MoveDown:
			{
				wxPGProperty* pWxParentProperty = m_PropertyMenuData.pWxProperty->GetParent();				
				wxArrayProperty* pWxArrayProperty = static_cast<wxArrayProperty*>(m_PropertyMenuData.pWxProperty);
				ArrayProperty* pArrayProperty = static_cast<ArrayProperty*>(pProperty);
				u32 indexInParent = pWxArrayProperty->GetIndexInParent();
				m_pPropertyList->DeleteProperty(pWxArrayProperty);

				// move the property
				IProperty* pSubProperty = pArrayProperty->GetProperties()[m_PropertyMenuData.propertyIndex];
				pArrayProperty->GetProperties().erase(pArrayProperty->GetProperties().begin()+m_PropertyMenuData.propertyIndex);
				u32 newIndex = m_PropertyMenuData.propertyIndex + ((event.GetId() == ID_Property_MoveDown) ? 1 : -1);
				pArrayProperty->GetProperties().insert(pArrayProperty->GetProperties().begin()+newIndex, pSubProperty);

				// move the corresponding element in the user data
				IArray* pArray = pArrayProperty->GetArray();
				void* pElement = pArray->Remove(m_PropertyMenuData.propertyIndex);
				pArray->Insert(newIndex, pElement);

				wxPGProperty* pNewProperty = PropertyList::GetWxProperty(pArrayProperty, m_pPropertyList);
				m_pPropertyList->Insert(pWxParentProperty, indexInParent, pNewProperty);
				m_pPropertyList->FillArrayProperties(pArrayProperty, pNewProperty);
			}
 			break;
 		}
	}

	//! call on property change
	void ObjectInspector::OnPropertyChanged(wxPropertyGridEvent &event)
	{
		Object* pObject = m_pPropertyList->GetSelectedObject();

		PropertyStream stream(shoot::SM_Write);
		pObject->Serialize(stream);

		// check if this is a sub property
		wxPGProperty* pWxProperty = event.GetProperty();
		if(!pWxProperty->GetParent()->IsRoot()) 
		{
			PropertyLocator locator;
			IProperty* pParentProperty = locator.GetProperty(pWxProperty->GetParent(), stream);

			// if this was part of an array, mark it
			if(pParentProperty->GetType() == PT_Array)
			{
				wxArrayProperty* pWxArrayParent = static_cast<wxArrayProperty*>(pWxProperty->GetParent());
				pWxArrayParent->SetEventType(wxArrayProperty::E_SubPropertyChanged);
				pWxArrayParent->SetChangedSubPropertyIndex(pWxProperty->GetIndexInParent());
			}			
			else if(pParentProperty->GetType() == PT_Reference)
			{
				wxReferenceProperty* pWxRefParent = static_cast<wxReferenceProperty*>(pWxProperty->GetParent());
				pWxRefParent->SetEventType(wxReferenceProperty::E_SubPropertyChanged);				
			}
			
			IProperty* pRootProperty = locator.GetRootProperty(pWxProperty, stream);

			// add to undo stack
			UndoManager::Instance()->RecordPropertyState(pObject, pRootProperty);

			PropertyList::FillProperty(pWxProperty->GetParent(), pParentProperty);
			stream.SetMode(shoot::SM_Read);
			stream.SetPropertyToUpdate(pRootProperty->GetName());
			pObject->Serialize(stream);
		}
		else 
		{
			PropertyLocator locator;
			IProperty* pProperty = locator.GetProperty(pWxProperty, stream);
			SHOOT_ASSERT(pProperty->GetName() == pWxProperty->GetName(), "wxProperty and shoot Property mismatch");

			// add to undo stack
			UndoManager::Instance()->RecordPropertyState(pObject, pProperty);

			PropertyList::FillProperty(pWxProperty, pProperty);
			stream.SetMode(shoot::SM_Read);
			stream.SetPropertyToUpdate(pProperty->GetName());
			pObject->Serialize(stream);

			// if an entity's name has changed, change it in the tree control as well
			if(pObject->IsA(Entity::TypeID)
			&& pProperty->GetName() == "Name")
			{
				if(pProperty->GetType() != PT_Reference)
				{
					Entity* pEntity = static_cast<Entity*>(pObject);
					ShootEditor* pEditor = ShootEditor::Instance();
					wxTreeItemId itemId = pEditor->GetTreeView()->GetItemIDFromEntity(pEditor->GetTreeView()->GetRootItem(), pEntity);			 
					pEditor->GetTreeView()->SetItemText(itemId, pProperty->GetString());
				}

				// if a camera name changed, update the camera selector				
				if(pObject->IsA(Camera::TypeID))
				{
					ShootEditor::Instance()->UpdateCameraSelectors();					
				}
			}			
		}

		// If we changed a visitor property, reinit the visitor
		if(Visitor* pVisitor = DYNAMIC_CAST(pObject, Visitor))
		{
			if(pVisitor->GetAutoStart())
			{
				if(pVisitor->IsActive())
				{
					pVisitor->Leave();
				}
				pVisitor->Init();
			}
		}
		else if(Component* pComponent = DYNAMIC_CAST(pObject, Component))
		{			
			// if changed the component of an image, update the geometry accordingly
			if(Image* pImage = DYNAMIC_CAST(pComponent->GetParent(), Image))
			{
				pImage->UpdateGeometry();
			}
		}
		else if(Entity* pEntity = DYNAMIC_CAST(pObject, Entity))
		{
			ShootEditor::Instance()->GetTreeView()->RefreshEntity(pEntity);			
		}
	}

	//! called while a property is changing
	void ObjectInspector::OnPropertyChanging(wxPropertyGridEvent &event)
	{
		// TODO
	}

	//! called while a property is selected
	void ObjectInspector::OnPropertySelected(wxPropertyGridEvent &event)
	{
		m_pPropertyListContainer->OnPropertySelected(event.GetProperty());
	}

	//! called while a property is right clicked
	void ObjectInspector::OnPropertyRightClick(wxPropertyGridEvent &event)
	{
		wxPGProperty* pWxProperty = event.GetProperty();
		if(pWxProperty->IsEnabled())
		{
			// check if part of an array property
			if(!pWxProperty->GetParent()->IsRoot())
			{
				Object* pObject = m_pPropertyList->GetSelectedObject();
				PropertyStream stream(shoot::SM_Write);
				pObject->Serialize(stream);

				PropertyLocator locator;
				IProperty* pParentProperty = locator.GetProperty(pWxProperty->GetParent(), stream);
				if(pParentProperty->GetType() == PT_Array)
				{				
					wxMenu subMenu;

					subMenu.Append(ID_Property_Remove, "Remove");
					subMenu.Append(ID_Property_Copy, "Copy");

					if(pWxProperty->GetIndexInParent() > 0)
					{
						subMenu.Append(ID_Property_MoveUp, "Move Up");
					}					
					
					if(pWxProperty->GetIndexInParent() < pWxProperty->GetParent()->GetChildCount()-1)
					{
						subMenu.Append(ID_Property_MoveDown, "Move Down");
					}					
					m_PropertyMenuData.pObject = pObject;
					m_PropertyMenuData.pWxProperty = pWxProperty->GetParent();
					m_PropertyMenuData.propertyIndex = pWxProperty->GetIndexInParent();
					subMenu.Connect(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&ObjectInspector::OnPropertyMenuClicked, NULL, this);
					PopupMenu(&subMenu);
				}
			}
		}
	}
}

