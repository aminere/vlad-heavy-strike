/* 

Amine Rehioui
Created: March 13th 2011

*/

#include "ShootEditorCommon.h"

#include "TabContainer.h"
#include "IconManager.h"

#include "ToolBarManager.h"
#include "EditorUtils.h"

#include "EventManager.h"
#include "EditorEvents.h"
#include "UndoManager.h"

#include "wx/sizer.h"
#include <wx/artprov.h> // wxArtProvider
#include <wx/bmpbuttn.h>

namespace shoot
{
	// Define event table
	BEGIN_EVENT_TABLE(TabContainer, wxPanel)
		EVT_COMMAND(ID_Menu_New, wxEVT_COMMAND_BUTTON_CLICKED, OnNew)
		EVT_COMMAND(ID_Menu_Open, wxEVT_COMMAND_BUTTON_CLICKED, OnOpen)
		EVT_COMMAND(ID_Menu_Save, wxEVT_COMMAND_BUTTON_CLICKED, OnSave)
		EVT_COMMAND(ID_Menu_SaveAs, wxEVT_COMMAND_BUTTON_CLICKED, OnSaveAs)
		EVT_COMMAND(ID_Menu_ReloadLastSave, wxEVT_COMMAND_BUTTON_CLICKED, OnReloadLastSave)
		EVT_COMBOBOX(ID_3DCameraSelector, On3DCameraChanged)
		EVT_COMBOBOX(ID_2DCameraSelector, On2DCameraChanged)
		EVT_COMMAND(ID_Tab_ResetView, wxEVT_COMMAND_BUTTON_CLICKED, OnResetView)
		EVT_COMMAND(ID_Tab_ClearFade, wxEVT_COMMAND_BUTTON_CLICKED, OnClearFade)
		EVT_COMMAND(ID_Tab_ToggleBlackBars, wxEVT_COMMAND_BUTTON_CLICKED, OnToggleBlackBars)		
	END_EVENT_TABLE()

	//! Constructor
	TabContainer::TabContainer(wxWindow* pParent,
							   wxWindowID id /*= wxID_ANY*/, 
							   const wxPoint& pos /*= wxDefaultPosition*/, 
							   const wxSize& size /*= wxSize(800, 600)*/, 
							   long style /*= 0*/) 
	: super(pParent, id, pos, size, style)	
	, m_p3DCamera(NULL)
	, m_p2DCamera(NULL)
	{
		wxSize iconSize = wxSize(24, 22);

		wxBoxSizer *pMenuSizer = new wxBoxSizer( wxHORIZONTAL );

		wxBoxSizer *pToolBarSizer = new wxBoxSizer( wxHORIZONTAL );
		pToolBarSizer->Add(new wxBitmapButton(this, ID_Menu_New, wxArtProvider::GetBitmap(wxART_NEW), wxDefaultPosition, iconSize));
		pToolBarSizer->Add(new wxBitmapButton(this, ID_Menu_Open, wxArtProvider::GetBitmap(wxART_FILE_OPEN), wxDefaultPosition, iconSize));
		pToolBarSizer->Add(new wxBitmapButton(this, ID_Menu_Save, wxArtProvider::GetBitmap(wxART_FILE_SAVE), wxDefaultPosition, iconSize));
		pToolBarSizer->Add(new wxBitmapButton(this, ID_Menu_SaveAs, wxArtProvider::GetBitmap(wxART_FILE_SAVE_AS), wxDefaultPosition, iconSize));		
		pToolBarSizer->Add(new wxButton(this, ID_Menu_ReloadLastSave, "R", wxDefaultPosition, iconSize), 0, wxALIGN_CENTRE_VERTICAL);
		pToolBarSizer->AddSpacer(8);

		// create the camera selectors
		wxString cameras[] = { "Editor" };
		E_EventID aCameraSelectorIDs[] = { ID_3DCameraSelector, ID_2DCameraSelector };
		const char* aCameraSeletorCaptions[] = { "3D Camera ", "2D Camera " };
		for(u32 i=CS_3D; i<CS_Count; ++i)
		{
			m_pCameraSelectors[i] = new wxComboBox(this, aCameraSelectorIDs[i], "Camera", wxDefaultPosition, wxSize(80, 22), 1, cameras, wxCB_DROPDOWN | wxCB_READONLY);
			m_pCameraSelectors[i]->SetValue(cameras[0]);
			pToolBarSizer->Add(new wxStaticText(this, wxID_ANY, aCameraSeletorCaptions[i]), 0, wxALIGN_CENTRE_VERTICAL);			
			pToolBarSizer->Add(m_pCameraSelectors[i]);
			pToolBarSizer->AddSpacer(8);
		}

		pToolBarSizer->Add(new wxButton(this, ID_Tab_ResetView, "RV", wxDefaultPosition, iconSize), 0, wxALIGN_CENTRE_VERTICAL);
		pToolBarSizer->Add(new wxButton(this, ID_Tab_ClearFade, "CF", wxDefaultPosition, iconSize), 0, wxALIGN_CENTRE_VERTICAL);
		pToolBarSizer->Add(new wxButton(this, ID_Tab_ToggleBlackBars, "TB", wxDefaultPosition, iconSize), 0, wxALIGN_CENTRE_VERTICAL);		
		
		m_pViewPortContainer = new ViewPortContainer(this);
		m_p3DCamera = m_pViewPortContainer->GetViewPort()->Get3DCamera();
		m_p2DCamera = m_pViewPortContainer->GetViewPort()->Get2DCamera();


		wxBoxSizer *pSizer = new wxBoxSizer( wxVERTICAL );
		pSizer->Add(pMenuSizer);
		pSizer->Add(pToolBarSizer);		
		pSizer->Add(m_pViewPortContainer, wxSizerFlags(1).Expand());		

		SetSizer(pSizer);
		pSizer->FitInside(this);
	}

	//! Destructor
	TabContainer::~TabContainer()
	{
	}

	void TabContainer::OnNew(wxCommandEvent& event)
	{
		Engine::Instance()->GetContextStack()->Clear();

		m_CurrentProjectPath = "";
		ShootEditor::Instance()->SelectEntity(NULL);

		ShootEditor::Instance()->GetTreeView()->DeleteAllItems();

		// clear undo history
		UndoManager::Instance()->ClearHistory();		

		UpdateCameraSelectors();

		SetTabName("New");
	}

	void TabContainer::OnOpen(wxCommandEvent& event)
	{		
		if(EditorUtils::OpenFileDialog(this, m_CurrentProjectPath))
		{
			Load(m_CurrentProjectPath);
		}
	}

	void TabContainer::OnSave(wxCommandEvent& event)
	{
		if(!ShootEditor::Instance()->IsEngineStarted())
		{
			if(m_CurrentProjectPath == "")
			{
				if(!EditorUtils::OpenFileDialog(this, m_CurrentProjectPath, "XML Files (*.xml)|*.xml", "Save File", wxFD_SAVE))
				{
					return;
				}
			}

			EditorUtils::FormatObjectPath(Engine::Instance()->GetContextStack(), m_CurrentProjectPath);
			Engine::Instance()->GetContextStack()->SaveToXML(m_CurrentProjectPath.c_str());
		}
		else
		{
			wxMessageBox("Can't save when the engine is started. Stop the engine first.", "Can't save", wxOK | wxICON_INFORMATION, this);
		}
	}

	void TabContainer::OnSaveAs(wxCommandEvent& event)
	{
		std::string path;
		if(EditorUtils::OpenFileDialog(this, path, "XML Files (*.xml)|*.xml", "Save File As", wxFD_SAVE))
		{
			EditorUtils::FormatObjectPath(Engine::Instance()->GetContextStack(), path);
			Engine::Instance()->GetContextStack()->SaveToXML(path.c_str());

			SetProjectPath(path);
		}
	}
	
	void TabContainer::OnReloadLastSave(wxCommandEvent& event)
	{
		if(m_CurrentProjectPath.length() == 0)
		{
			wxMessageBox("No save available, cannot reload level", "No save available", wxOK | wxICON_INFORMATION, this);			
		}
		else
		{
			Reload();
		}
	}

	void TabContainer::On3DCameraChanged(wxCommandEvent& event)
	{
		std::string strCameraName(m_pCameraSelectors[CS_3D]->GetStringSelection().c_str());		
		bool bGameCamera = (m_pCameraSelectors[CS_3D]->GetSelection() != 0);
		if(bGameCamera)
		{
			Camera* pCamera = DYNAMIC_CAST(Engine::Instance()->GetContextStack()->GetChildByName(strCameraName), Camera);
			SHOOT_ASSERT(pCamera, "Camera not found or not of type Camera");
			EntityRenderer::Instance()->Set3DCamera(pCamera);
		}
		else
		{
			if(ViewPort* pViewPort = ShootEditor::Instance()->GetViewPort())
			{
				EntityRenderer::Instance()->Set3DCamera(pViewPort->Get3DCamera());
			}			
		}

		m_p3DCamera = EntityRenderer::Instance()->Get3DCamera();
	}

	void TabContainer::On2DCameraChanged(wxCommandEvent& event)
	{
		std::string strCameraName(m_pCameraSelectors[CS_2D]->GetStringSelection().c_str());
		bool bGameCamera = (m_pCameraSelectors[CS_2D]->GetSelection() != 0);		
		if(bGameCamera)
		{
			Camera* pCamera = DYNAMIC_CAST(Engine::Instance()->GetContextStack()->GetChildByName(strCameraName), Camera);
			SHOOT_ASSERT(pCamera, "Camera not found or not of type Camera");
			EntityRenderer::Instance()->Set2DCamera(pCamera);
		}
		else
		{
			if(ViewPort* pViewPort = ShootEditor::Instance()->GetViewPort())
			{
				EntityRenderer::Instance()->Set2DCamera(pViewPort->Get2DCamera());
			}			
		}

		m_p2DCamera = EntityRenderer::Instance()->Get2DCamera();
	}

	void TabContainer::OnResetView(wxCommandEvent& event)
	{
		m_pViewPortContainer->GetViewPort()->ResetView();
		EntityRenderer::Instance()->Set3DCamera(m_pViewPortContainer->GetViewPort()->Get3DCamera());
		EntityRenderer::Instance()->Set2DCamera(m_pViewPortContainer->GetViewPort()->Get2DCamera());
		UpdateCameraSelectors();
	}

	void TabContainer::OnClearFade(wxCommandEvent& event)
	{
		FadeRenderer::Instance()->SetColor(Color::Zero);
	}

	void TabContainer::OnToggleBlackBars(wxCommandEvent& event)
	{
		bool bRenderBlackBars = EntityRenderer::Instance()->GetRenderViewField();
		EntityRenderer::Instance()->SetRenderViewField(!bRenderBlackBars);
	}

	//! called when the tab is just selected
	void TabContainer::OnSelected()
	{
		EntityRenderer::Instance()->Set3DCamera(m_p3DCamera);
		EntityRenderer::Instance()->Set2DCamera(m_p2DCamera);

		// Fill the tree view
		ShootEditor::Instance()->GetTreeView()->DeleteAllItems();
		ShootEditor::Instance()->GetTreeView()->InsertEntity(Engine::Instance()->GetContextStack());

		ShootEditor::Instance()->SelectEntity(m_SelectedEntity);

		UpdateCameraSelectors();
	}

	//! called when the tab is about to close, return false to forbid closing
	bool TabContainer::OnClose()
	{		
		ShootEditor::Instance()->SelectEntity(NULL);

		ShootEditor::Instance()->GetTreeView()->DeleteAllItems();

		return true;
	}

	//! updates a camera selector		
	void TabContainer::UpdateCameraSelectors()
	{
		for(u32 SelectorIndex = CS_3D; SelectorIndex < CS_Count; SelectorIndex++)
		{
			m_pCameraSelectors[SelectorIndex]->Clear();
			m_pCameraSelectors[SelectorIndex]->Append("Editor");

			std::vector< Handle<Entity> > aCameras;
			Engine::Instance()->GetContextStack()->GetChildrenByType(Camera::GetStaticClassName(), aCameras);
			for(u32 i=0; i<aCameras.size(); ++i)
			{
				if(SelectorIndex == CS_3D 
				&& (static_cast<Camera*>(aCameras[i].Get())->GetType() == Camera::Type_Perspective
				||  static_cast<Camera*>(aCameras[i].Get())->GetType() == Camera::Type_Ortho3D))
				{
					m_pCameraSelectors[SelectorIndex]->Append(aCameras[i]->GetName());
				}
				else if(SelectorIndex == CS_2D
				&& static_cast<Camera*>(aCameras[i].Get())->GetType() == Camera::Type_Ortho2D)
				{
					m_pCameraSelectors[SelectorIndex]->Append(aCameras[i]->GetName());
				}
			}

			// set current selection
			Camera* pCamera = NULL;
			if(SelectorIndex == CS_3D)
			{	
				m_p3DCamera = EntityRenderer::Instance()->Get3DCamera();
				pCamera = m_p3DCamera;				
			}
			else
			{
				m_p2DCamera = EntityRenderer::Instance()->Get2DCamera();
				pCamera = m_p2DCamera;
			}
			
			bool bGameCamera = (pCamera
							 && pCamera != m_pViewPortContainer->GetViewPort()->Get3DCamera()
							 && pCamera != m_pViewPortContainer->GetViewPort()->Get2DCamera());
			if(bGameCamera)
			{
				int index = m_pCameraSelectors[SelectorIndex]->FindString(pCamera->GetName());
				SHOOT_ASSERT(index != wxNOT_FOUND, "Camera not found in the camera selector");
				m_pCameraSelectors[SelectorIndex]->Select(index);
			}
			else
			{
				m_pCameraSelectors[SelectorIndex]->Select(0);
			}
		}
	}

	//! reloads the current level
	void TabContainer::Reload()
	{
		if(!m_CurrentProjectPath.empty())
		{
			if(ShootEditor::Instance()->IsEngineStarted())
			{
				ShootEditor::Instance()->StopEngine();
			}
			else
			{
				Load(m_CurrentProjectPath);
			}
		}
	}

	//! loads a project
	void TabContainer::Load(const std::string& strPath)
	{
		ShootEditor::Instance()->SelectEntity(NULL);

		Engine::Instance()->LoadContextStack(strPath.c_str(), true);
		
		// Fill the tree view
		ShootEditor::Instance()->GetTreeView()->DeleteAllItems();
		ShootEditor::Instance()->GetTreeView()->InsertEntity(Engine::Instance()->GetContextStack());

		// clear undo history
		UndoManager::Instance()->ClearHistory();

		UpdateCameraSelectors();
		SetProjectPath(strPath);
	}

	//! sets tab name
	void TabContainer::SetTabName(const std::string& name)
	{
		MainTabControl* pTabControl = ShootEditor::Instance()->GetMainTabControl();
		pTabControl->SetPageText(pTabControl->GetSelection(), name);
	}

	//! sets the current project path
	void TabContainer::SetProjectPath(const std::string& path)
	{
		// extract filename without ID (if any)
		size_t fileStartIndex = path.rfind("/");
		size_t fileEndIndex = path.rfind("_");
		if(fileEndIndex == std::string::npos)
		{
			fileEndIndex = path.rfind(".");
		}
		std::string fileName = path.substr(fileStartIndex+1, (fileEndIndex-fileStartIndex-1));
		SetTabName(fileName);

		m_CurrentProjectPath = path;
	}
}

