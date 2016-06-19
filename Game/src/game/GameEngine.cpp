/* 

Amine Rehioui
Created: June 24th 2011

*/

#include "ShootTest.h"
#include "GameEngine.h"

#include "InputManager.h"
#include "EventManager.h"
#include "FileSystem.h"
#include "SequenceVisitor.h"

namespace shoot
{
	//! constructor
	GameEngine::GameEngine(ObjectRegistrationCallback* pRegisterObjects,
						   GraphicsDriver::E_DriverType eDriverType /*= GraphicsDriver::DT_OpenGL*/,
						   InputManager::E_Type _eInputManagerType /*= InputManagre::IMT_SDL*/)
	: super(pRegisterObjects, eDriverType, _eInputManagerType)
	{
	}

	//! destructor
	GameEngine::~GameEngine()
	{
	}

	//! Engine initialization
	void GameEngine::Init()
	{
		super::Init();

		m_AudioManager = static_cast<AudioManager*>(ObjectManager::Instance()->Get("common/AudioManager.xml"));
		m_AudioManager->Init();

		m_GameManager = static_cast<GameManager*>(ObjectManager::Instance()->Get("common/GameManager.xml"));
		m_GameManager->Init();

		m_LoadingScreen = static_cast<ContextStack*>(ObjectManager::Instance()->Get("common/Loading.xml"));
		m_LoadingScreen->Init();
	}

	//! called during the update of the engine
	/** \return: false to exit the program */
	bool GameEngine::Update(f32 fDeltaTime)
	{
		if(IsLoading())
		{
			CalculateFPS(fDeltaTime);
			m_LoadingScreen->Update();
			UpdateLoadingThreadStatus();
			return true;
		}

		if(m_bRequestResourceReload)
		{
			super::Update(fDeltaTime);
			return true;
		}

		m_AudioManager->Update();
		m_GameManager->Update();

		if(InputManager::Instance()->IsKeyJustPressed(InputManager::KT_Back))
		{
			shoot::UIEvent* pEvent = snew shoot::UIEvent();
			pEvent->m_Command = "BACK";
			EventManager::Instance()->SendEvent(pEvent);
		}

#ifndef _RETAIL_
		if(InputManager::Instance()->IsKeyJustReleased(InputManager::KT_Y)) // pause
		{
			if(Context* pContext = GetContext(0))
			{
				pContext->SetEnabled(!pContext->IsEnabled());
			}
		}

		// cheats
		if(InputManager::Instance()->IsKeyPressed(InputManager::KT_R)
		&& InputManager::Instance()->IsKeyPressed(InputManager::KT_L))
		{
			if(InputManager::Instance()->IsKeyJustPressed(InputManager::KT_A))
			{
				m_GameManager->OnLevelFinished();
			}
		}

		// god mode
		static f32 fDeltaTimeAccelTimer = 0.0f;
		if(InputManager::Instance()->IsKeyPressed(InputManager::KT_A)
		&& InputManager::Instance()->IsKeyPressed(InputManager::KT_B))
		{
			fDeltaTimeAccelTimer += fDeltaTime;
			return super::Update(fDeltaTime*3.0f*((u32)fDeltaTimeAccelTimer+1));
		}
		else
		{
			fDeltaTimeAccelTimer = 0.0f;
			return super::Update(fDeltaTime);
		}
#else
		return super::Update(fDeltaTime);
#endif // _RETAIL_
	}

	//! called during the rendering the engine
	void GameEngine::Render()
	{
		if(IsLoading())
		{
			GraphicsDriver::Instance()->Begin();
			EntityRenderer::Instance()->Clear();
			m_LoadingScreen->RegisterForRendering();
			EntityRenderer::Instance()->Render();
			return;
		}

		if(m_bRequestResourceReload)
		{
			return;
		}

		super::Render();
	}

	//! called when the loading thread has started
	void GameEngine::OnLoadingStarted()
	{
		Camera* pCamera = static_cast<Camera*>(m_LoadingScreen->GetChildByName("Camera2D"));
		pCamera->Activate();

		Entity* pLoadingText = m_LoadingScreen->GetChildByName("LoadingText");
		pLoadingText->GetComponent<GraphicComponent>()->GetMaterial()->GetTexture()->ResourceInit();
		pLoadingText->SetVisible(false);
		pLoadingText->GetComponent<SequenceVisitor>()->Visit(pLoadingText);
	}

	//! called when the application is resumed
	void GameEngine::OnResume()
	{
		super::OnResume();
		m_GameManager->Pause();
	}

	//! called when the resources have been reloaded
	void GameEngine::OnResourcesReloaded()
	{
		FileSystem::Instance()->UnloadAllPartitions("common");
		GraphicsDriver::Instance()->LoadObjects();
	}
}

