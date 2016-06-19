/* 

Amine Rehioui
Created: April 10th 2010

*/

#include "ShootEditorCommon.h"

#include "App.h"

#include "wxArrayProperty.h"
#include "wxReferenceProperty.h"

#include "PropertyLocator.h"
#include "TabContainer.h"

#include "Plane.h"
#include "Triangle.h"

#include "EventManager.h"
#include "EditorEvents.h"

#include "Path.h"

#include "CameraManager.h"
#include "EditorSettings.h"
#include "Entity3DController.h"
#include "EditorRenderer.h"
#include "ObjectViewer.h"
#include "ToolbarManager.h"
#include "UndoManager.h"
#include "ResourceWatcher.h"
#include "AudioDriver.h"

#ifdef DX11
#include "DirectX11Driver.h"
#endif // DX11

namespace shoot
{	
	//! Constructor
	ShootEditor::ShootEditor()	
	: m_bEngineStarted(false)
	, m_bEnginePaused(false)
	, m_eControlMode(ControlMode_Translate)
	, m_fDeltaTime(1.0f/60.0f)
	, m_bHasPin(false)
	, m_vPinPosition(Vector3::Zero)
	, m_b3DControl(true)
	, m_bAudioEnabled(true)
	{
		// create singletons
		EditorSettings::CreateInstance();
		CameraManager::CreateInstance();
		Entity3DController::CreateInstance();
		EditorRenderer::CreateInstance();
		UndoManager::CreateInstance();
		ResourceWatcher::CreateInstance();
	}

	//! Destructor
	ShootEditor::~ShootEditor()
	{
		// destroy singletons
		ResourceWatcher::DestroyInstance();
		Entity3DController::DestroyInstance();
		EditorRenderer::DestroyInstance();
		CameraManager::DestroyInstance();
		EditorSettings::DestroyInstance();
		UndoManager::DestroyInstance();	
	}

	//! Initialize the editor
	void ShootEditor::Init(MainFrame* pFrame)
	{
		// create the tab control
		//wxNotebook* pTabControl = new wxNotebook(pFrame, wxID_ANY, wxDefaultPosition, wxSize(240, 800));
		m_pTreeView = new TreeView(pFrame);
		
		//pTabControl->AddPage(m_pTreeView, "Tree", true);
		//pTabControl->AddPage(m_pObjectInspector, "Properties", true);
		pFrame->GetUIManager().AddPane(m_pTreeView, wxAuiPaneInfo().Left().CaptionVisible(false).CloseButton(false));

		m_pObjectInspector = new ObjectInspector(pFrame);
		pFrame->GetUIManager().AddPane(m_pObjectInspector, wxAuiPaneInfo().Right().CaptionVisible(false).CloseButton(false));

		// create the tab container
		m_pMainTabControl = new MainTabControl(pFrame);
		m_pMainTabControl->Init();
		pFrame->GetUIManager().AddPane(m_pMainTabControl, wxAuiPaneInfo().Center().MaximizeButton().CloseButton(false).CaptionVisible(false).PaneBorder(false));

		//Camera3D* pMainCamera = new Camera3D();
		//m_pViewPortContainer[VP_Main] = new ViewPortContainer(pFrame, pMainCamera);
		//pFrame->GetUIManager().AddPane(m_pViewPortContainer[VP_Main], wxAuiPaneInfo().Center().MaximizeButton().CloseButton(false).Caption("Main"));

		//Camera3D* pTopCamera = new Camera3D();
		//pTopCamera->SetPosition(Vector3::Create(0.0f, 10.0f, 0.001f));
		//pTopCamera->SetUpVector(Vector3::Create(0.0f, 0.0f, -1.0f));
		//m_pViewPortContainer[VP_Top] = new ViewPortContainer(pFrame, pTopCamera, m_pViewPortContainer[VP_Main]->GetViewPort()->GetContext());
		//pFrame->GetUIManager().AddPane(m_pViewPortContainer[VP_Top], wxAuiPaneInfo().Center().MaximizeButton().Right().Caption("Top"));
		//
		//Camera3D* pSideCamera = new Camera3D();
		//pSideCamera->SetPosition(Vector3::Create(10.0f, 0.0f, 0.0f));
		//m_pViewPortContainer[VP_Side] = new ViewPortContainer(pFrame, pSideCamera, m_pViewPortContainer[VP_Main]->GetViewPort()->GetContext());
		//pFrame->GetUIManager().AddPane(m_pViewPortContainer[VP_Side], wxAuiPaneInfo().Center().MaximizeButton().Right().Caption("Side"));

		// create the resource window
		//m_pResourceWindow = new ResourceWindow(pFrame, wxID_ANY, wxDefaultPosition, wxSize(250, 100));
		//pFrame->GetUIManager().AddPane(m_pResourceWindow, wxAuiPaneInfo().Name(wxT("Resources")).Caption(wxT("Resources")).Bottom());

		// create the status bar
		const u32 numFields = 5;
		wxStatusBar* pStatusBar = pFrame->CreateStatusBar(numFields);
		int statusWidths[numFields] = { 200, 80, 120, 300, -1 };
		pStatusBar->SetStatusWidths(numFields, statusWidths);
		pFrame->SetStatusText(_T("Ready"));

		// additional initialization now that the context has been created

#ifdef DX11
		if(GraphicsDriver::Instance()->GetType() == GraphicsDriver::DT_DirectX11)
		{
			HWND hWindow = (HWND)GetViewPort()->GetHWND();
			DirectX11Driver::Instance()->SetWindow(hWindow);
		}
#endif // DX11

		Engine::Instance()->Init();
		EditorSettings::Instance()->Init();
		EditorRenderer::Instance()->Init();

		EntityRenderer::Instance()->SetRenderViewField(false);
		//GetTabContainer()->Load("1/Level1.xml");
	}

	//! Updates the editor
	void ShootEditor::Update()
	{
		if(IsEngineRunning())
		{
			App::Instance()->Update(m_fDeltaTime);
		}
		else
		{
			// always update the following
			Engine::Instance()->CalculateFPS(m_fDeltaTime);
			EventManager::Instance()->Update();
			ResourceManager::Instance()->Update();
		}

		char buf[16];
		sprintf(buf, "FPS: %.2f", Engine::Instance()->GetFPS());
		static_cast<wxFrame*>(wxTheApp->GetTopWindow())->SetStatusText(buf);

		// refresh the rendering window
		GetViewPort()->Refresh(true);
	}

	//! Returns a viewport
	ViewPort* ShootEditor::GetViewPort()
	{
		return GetTabContainer()->GetViewPortContainer()->GetViewPort();
	}

	//! Selects an entity
	Entity* ShootEditor::SelectEntity(Entity* pEntity)
	{
		if(m_LockedEntity.IsValid())
		{
			return m_LockedEntity.Get();
		}

		m_pObjectInspector->Fill(pEntity);
		GetTabContainer()->SetSelectedEntity(pEntity);		
		m_pTreeView->SelectItemFromEntity(pEntity);
		CameraManager::Instance()->SetCameraPathElementInfo(false);

		Entity3D* pEntity3D = DYNAMIC_CAST(pEntity, Entity3D);
		Entity3DController::Instance()->SetEntity(pEntity3D);

		if(pEntity3D)
		{
			char buf[256];
			Vector3 vWorldPos = pEntity3D->GetTransformationMatrix().GetTranslation(); 
			sprintf(buf, "World: %f, %f, %f", vWorldPos.X, vWorldPos.Y, vWorldPos.Z);
			static_cast<wxFrame*>(wxTheApp->GetTopWindow())->SetStatusText(buf, 3);
		}
		return pEntity;
	}

	//! Selects an entity
	Entity* ShootEditor::SelectEntity(Entity* pEntity, const Vector3& vRayStart, const Vector3& vRayDir)
	{
		return SelectEntity(pEntity);
	}

	//! Returns the selected entity
	Entity* ShootEditor::GetSelectedEntity()
	{
		return m_LockedEntity.IsValid() ? m_LockedEntity.Get() : GetTabContainer()->GetSelectedEntity();		
	}

	//! update viewports sizes
	void ShootEditor::UpdateViewPortsSizes()
	{
		if(EditorSettings::Instance()->WindowSize == EditorSettings::WS_Stretch)
		{
			ViewPort* pViewPort = GetViewPort();
			pViewPort->SetPosition(wxPoint(0, 0));
			pViewPort->SetSize(GetTabContainer()->GetViewPortContainer()->GetSize());					
		}
		else
		{
			Size size = EditorSettings::WindowSizes[EditorSettings::Instance()->WindowSize];
			ViewPort* pViewPort = GetViewPort();
			wxSize viewPortSize = pViewPort->GetSize();
			wxPoint center = wxPoint((viewPortSize.x - size.Width)/2, (viewPortSize.y - size.Height)/2);
			pViewPort->SetPosition(center);
			pViewPort->SetSize(wxSize(size.Width, size.Height));			
		}
	}

	//! updates the camera selector
	void ShootEditor::UpdateCameraSelectors()
	{
		GetTabContainer()->UpdateCameraSelectors();
	}

	//! returns a tab container
	TabContainer* ShootEditor::GetTabContainer()
	{
		return static_cast<TabContainer*>(m_pMainTabControl->GetPage(0));		
	}

	//! starts the engine
	void ShootEditor::StartEngine()
	{
		if(!m_bEngineStarted || m_bEnginePaused)
		{
			m_bEngineStarted = true;
			m_bEnginePaused = false;
			static_cast<wxFrame*>(wxTheApp->GetTopWindow())->SetStatusText("Engine Running", 2);

			EntityRenderer::Instance()->SetRenderViewField(true);
			ToolbarManager::Instance()->GetMainToolbar()->ToggleTool(ID_Engine_Start, true);
		}
	}

	//! stops the engine
	void ShootEditor::StopEngine()
	{
		if(m_bEngineStarted)
		{
			m_bEngineStarted = false;
			static_cast<wxFrame*>(wxTheApp->GetTopWindow())->SetStatusText("Engine Stopped", 2);

			EntityRenderer::Instance()->SetRenderViewField(false);

			// restore the context stack
			GetTabContainer()->Reload();
		}

		ToolbarManager::Instance()->GetMainToolbar()->ToggleTool(ID_Engine_Stop, true);
	}

	//! pauses the engine
	void ShootEditor::PauseEngine()
	{
		if(IsEngineRunning())
		{
			m_bEnginePaused = true;
			static_cast<wxFrame*>(wxTheApp->GetTopWindow())->SetStatusText("Engine Paused", 2);

			m_pTreeView->DeleteAllItems();
			m_pTreeView->InsertEntity(Engine::Instance()->GetContextStack());			
			UpdateCameraSelectors();

			EntityRenderer::Instance()->SetRenderViewField(false);
		}

		ToolbarManager::Instance()->GetMainToolbar()->ToggleTool(m_bEngineStarted ? ID_Engine_Pause : ID_Engine_Stop, true);
	}

	//! sets pin status
	void ShootEditor::SetHasPin(bool bSet)
	{
		m_bHasPin = bSet;
		ToolbarManager::Instance()->GetMainToolbar()->EnableTool(ID_Edit_Pin, bSet);
	}

	//! sets audio status
	void ShootEditor::SetAudioEnabled(bool bEnabled)
	{
		if(bEnabled)
		{
			AudioDriver::Instance()->Resume();
		}
		else
		{
			AudioDriver::Instance()->Pause();
		}
		m_bAudioEnabled = bEnabled;
	}

	//! returns a menu category for a class type
	std::string ShootEditor::GetMenuCategory(const char* classType)
	{
		std::map<const char*, const char*> aCategoryMap;

		const char* strCore = "Core";
		aCategoryMap["RenderableEntity"] = strCore;
		aCategoryMap["Context"] = strCore;
		aCategoryMap["ContextStack"] = strCore;

		// 2D
		const char* str2D = "2D";
		aCategoryMap["Entity2D"] = str2D;
		aCategoryMap["Image"] = str2D;
		aCategoryMap["Sprite"] = str2D;
		aCategoryMap["Text"] = str2D;
		aCategoryMap["ParticleGenerator2D"] = str2D;

		// 3D
		const char* str3D = "3D";
		aCategoryMap["Entity3D"] = str3D;
		aCategoryMap["Camera"] = str3D;
		aCategoryMap["MeshEntity"] = str3D;
		aCategoryMap["ParticleGenerator"] = str3D;
		aCategoryMap["BillBoardEntity"] = str3D;	
		aCategoryMap["SkyBoxEntity"] = str3D;	
		aCategoryMap["Path"] = str3D;
		aCategoryMap["PathElement"] = str3D;	
		aCategoryMap["CameraPath"] = str3D;
		aCategoryMap["CameraPathElement"] = str3D;

		// UI
		aCategoryMap["Button"] = "UI";

		for(std::map<const char*, const char*>::iterator it = aCategoryMap.begin(); it != aCategoryMap.end(); ++it)
		{
			if(std::string((*it).first) == classType)
			{
				return (*it).second;
			}
		}

		return "";
	}
}

