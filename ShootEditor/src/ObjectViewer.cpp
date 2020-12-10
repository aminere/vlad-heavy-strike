/* 

Amine Rehioui
Created: March 31st 2011

*/

#include "ShootEditorCommon.h"

#include "ObjectViewer.h"

#include "IconManager.h"

#include "ShootEditor.h"

#include <wx/sizer.h>

#include <wx/aui/auibook.h>

namespace shoot
{	
	// Define event table
	BEGIN_EVENT_TABLE(ObjectViewer, wxPanel)
		EVT_LIST_COL_CLICK(wxID_ANY, OnColumnClick)
		EVT_LIST_ITEM_SELECTED(wxID_ANY, OnItemSelected)
		EVT_IDLE(OnIdle)
	END_EVENT_TABLE()

	//! Constructor
	ObjectViewer::ObjectViewer(wxWindow* pParent)
		: super(pParent, wxID_ANY, wxDefaultPosition, wxDefaultSize)
	{
		wxBoxSizer *pCommands = new wxBoxSizer( wxHORIZONTAL );
		//pCommands->Add(new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize), 0, wxALIGN_CENTRE_VERTICAL);

		m_pObjectList = new wxListView(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
		m_pObjectList->SetImageList(IconManager::Instance()->GetImageList(), wxIMAGE_LIST_SMALL);

		// Fill columns
		wxListItem column;
		column.SetText(_T("Type"));
		m_pObjectList->InsertColumn(0, column);		

		column.SetText(_T("ID"));
		m_pObjectList->InsertColumn(1, column);

		column.SetText(_T("Name"));
		m_pObjectList->InsertColumn(2, column);

		column.SetText(_T("Template"));
		m_pObjectList->InsertColumn(3, column);

		column.SetText(_T("RefCount"));
		m_pObjectList->InsertColumn(4, column);

		wxBoxSizer *pSizer = new wxBoxSizer( wxVERTICAL );
		pSizer->Add(pCommands);
		pSizer->Add(m_pObjectList, wxSizerFlags(1).Expand());

		SetSizer(pSizer); // use the sizer for layout

		pSizer->FitInside(this);

		memset(m_ColumSortDescending, 0, NumColumns*sizeof(bool));
	}

	// Destructor
	ObjectViewer::~ObjectViewer()
	{
	}	

	//! adds an object
	void ObjectViewer::AddObject(Object* pObject)
	{
		// fill type column		
		long item = m_pObjectList->InsertItem(m_pObjectList->GetItemCount(), pObject->GetClassName(), IconManager::Instance()->GetIconIndex(pObject));		
		m_pObjectList->SetItemData(item, (wxIntPtr)pObject);
		m_pObjectList->SetColumnWidth(0, wxLIST_AUTOSIZE);

		// fill ID column
		{
			std::stringstream ss;
			ss << "0x" << std::hex << pObject->GetID();
			m_pObjectList->SetItem(item, 1, ss.str());
			m_pObjectList->SetColumnWidth(1, wxLIST_AUTOSIZE);
		}

		// fill name column
		if(pObject->GetName().length())
		{
			m_pObjectList->SetItem(item, 2, pObject->GetName());
			m_pObjectList->SetColumnWidth(2, wxLIST_AUTOSIZE);
		}		

		// fill template column
		if(pObject->GetTemplatePath().length())
		{
			m_pObjectList->SetItem(item, 3, pObject->GetTemplatePath());
			m_pObjectList->SetColumnWidth(3, wxLIST_AUTOSIZE);
		}
		else if(Material* pMaterial = DYNAMIC_CAST(pObject, Material))
		{
			if(Texture* pTexture = pMaterial->GetTexture(0))
			{
				m_pObjectList->SetItem(item, 3, pTexture->GetTemplatePath());
				m_pObjectList->SetColumnWidth(3, wxLIST_AUTOSIZE);
			}
		}

		// fill RefCount column
		{
			std::stringstream ss;
			ObjectManager::ObjectContainer* pContainer = ObjectManager::Instance()->GetContainer(pObject);
			ss << pContainer->RefCount;
			m_pObjectList->SetItem(item, 4, ss.str());			
		}
	}	

	int wxCALLBACK ObjectViewer::ObjectViewerSort(wxIntPtr item1, wxIntPtr item2, wxIntPtr sortData)
	{
		Object* pObject1 = (Object*)item1;
		Object* pObject2 = (Object*)item2;
		ObjectViewer* pObjectViewer = (ObjectViewer*)sortData;
		s32 column = pObjectViewer->m_ColumnToSort;
		s32 result = 0;

		switch(column)
		{
		case 0: // sort by ClassName
			result = std::string(pObject1->GetClassName()).compare(pObject2->GetClassName());
			break;

		case 1: // sort by ID
			result = (pObject1->GetID() < pObject2->GetID()) ? -1 : 1;
			break;

		case 2: // sort by name
			result = pObject1->GetName().compare(pObject2->GetName());
			break;

		case 3: // sort by template path
			{
				std::string template1 = pObject1->GetTemplatePath();
				std::string template2 = pObject2->GetTemplatePath();
				result = template1.compare(template2);
			}
			break;

		case 4: // sort by refcount
			{
				ObjectManager::ObjectContainer* pContainer1 = ObjectManager::Instance()->GetContainer(pObject1);
				ObjectManager::ObjectContainer* pContainer2 = ObjectManager::Instance()->GetContainer(pObject2);
				if(pContainer1->RefCount < pContainer2->RefCount)
				{
					result = -1;
				}
				else if(pContainer1->RefCount > pContainer2->RefCount)
				{
					result = 1;
				}
				else
				{
					result = 0;
				}
			}			
			break;
		}

		if(pObjectViewer->m_ColumSortDescending[column])
		{
			result = -result;
		}

		return result;
	}

	//! event handlers
	void ObjectViewer::OnColumnClick(wxListEvent& event)
	{
		m_ColumnToSort = event.GetColumn();

		m_pObjectList->SortItems(ObjectViewer::ObjectViewerSort, (wxIntPtr)this);

		SHOOT_ASSERT(m_ColumnToSort >= 0 && m_ColumnToSort < s32(NumColumns), "Invalid Column Index");
		m_ColumSortDescending[m_ColumnToSort] = !m_ColumSortDescending[m_ColumnToSort];
	}

	void ObjectViewer::OnItemSelected(wxListEvent& event)
	{
		long item = event.GetItem();
		Object* pObject = reinterpret_cast<Object*>(m_pObjectList->GetItemData(item));		
		ShootEditor::Instance()->GetObjectInspector()->Fill(pObject);
	}

	void ObjectViewer::OnIdle(wxIdleEvent& event)
	{
		ObjectManager::ObjectMap& objectMap = ObjectManager::Instance()->GetObjectMap();
		MainTabControl* pTabControl = ShootEditor::Instance()->GetMainTabControl();

		if((m_PreviousMap != objectMap)
		&& pTabControl->GetSelection() == pTabControl->GetPageCount()-1)
		{
			// Fill items
			m_pObjectList->DeleteAllItems();
			for(ObjectManager::ObjectMap::iterator it = objectMap.begin(); it != objectMap.end(); ++it)			
			{
				AddObject((*it).first);
			}			
			
			// update tab title
			wxAuiNotebook* notebook = static_cast<wxAuiNotebook*>(this->GetParent());
			std::stringstream ss;
			ss << "ObjectViewer (" << objectMap.size() << " Objects)";
			notebook->SetPageText(notebook->GetPageCount()-1, ss.str());

			m_PreviousMap = objectMap;
		}
	}
}

