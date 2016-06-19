/* 

Amine Rehioui
Created: February 21th 2010

*/

#include "Shoot.h"

#include "EventManager.h"
#include "TimeManager.h"
#include "StatsManager.h"
#include "EditorEvents.h"
#include "MaterialProvider.h"
#include "GeometryProvider.h"
#include "FileSystem.h"
#include "MemoryManager.h"
#include "Thread.h"
#include "AudioDriver.h"
#include "NetworkManager.h"

#ifdef SHOOT_EDITOR
#include "FBXMeshLoader.h"
#endif

#ifdef DX11
#include "DirectX11Driver.h"
#endif // DX11

namespace shoot
{
	// static variable initialization
	Engine* Engine::m_spInstance = NULL;
	f32 g_fDeltaTime = 1.0f/60.0f;

	//! Constructor
	Engine::Engine(ObjectRegistrationCallback* pRegisterObjects /*= NULL*/,
				   GraphicsDriver::E_DriverType eDriverType /*= GraphicsDriver::DT_OpenGL*/,
				   InputManager::E_Type _eInputManagerType /*= InputManagre::IMT_SDL*/)
	: m_fps(0.0f)
	, m_frameTimer(0.0f)
	, m_numFrames(0)
	, m_numFramesPerSecond(0)
	, m_bInitialized(false)
	, m_pLoadingThread(NULL)
	, m_pLoadingFinishedCallback(NULL)
	, m_bLoadingThreadFinished(false)
	, m_pResourceReloadFinishedCallback(NULL)
	, m_bResourceReloadThreadFinished(false)
	, m_bRequestResourceReload(false)
	, m_bPaused(false)
	, m_bExitRequested(false)
	{
		SHOOT_ASSERT(!m_spInstance, "Cannot create multiple Engine instances");
		
		MemoryManager::Init();
		ProfileManager::Init();
		
		m_spInstance = this;
		
		// initializes the random number generator
		Random::Initialize();	

		// create ObjectFactory and register objects
		ObjectFactory::CreateInstance();
		ObjectManager::CreateInstance();
		if(pRegisterObjects)
		{
			pRegisterObjects();
		}

#ifdef SHOOT_EDITOR
		EditorEvents::Register();
#endif // SHOOT_EDITOR

		// create core singletons	
		FileSystem::CreateInstance();
		EventManager::CreateInstance();
		GraphicsDriver::CreateInstance(eDriverType);
		AudioDriver::CreateInstance();
		EntityRenderer::CreateInstance();
		ResourceManager::CreateInstance();
		TimeManager::CreateInstance();
		MaterialProvider::CreateInstance();
		GeometryProvider::CreateInstance();
		InputManager::CreateInstance(_eInputManagerType);
		NetworkManager::CreateInstance();
		NetworkManager::Instance()->Init();
		
		m_ContextStack = snew ContextStack();
	}

	Engine::~Engine()
	{
		m_bExitRequested = true;
		m_aEntityOperations.clear();
		m_ContextStack.Clear();
		m_FadeRenderer.Clear();

#ifdef DX11
		ID3D11Debug* pDebug = NULL;
		DirectX11Driver::Instance()->GetDevice()->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&pDebug));
#endif

#ifdef SHOOT_EDITOR
		FBXMeshLoader::Uninit();
#endif

		InputManager::DestroyInstance();
		if(StatsManager::IsCreated())
		{
			StatsManager::DestroyInstance();
		}
		MaterialProvider::DestroyInstance();
		GeometryProvider::DestroyInstance();
		EntityRenderer::DestroyInstance();
		ResourceManager::DestroyInstance();
		GraphicsDriver::DestroyInstance();
		AudioDriver::DestroyInstance();
		ObjectFactory::DestroyInstance();
		ObjectManager::DestroyInstance();
		EventManager::DestroyInstance();
		TimeManager::DestroyInstance();
		FileSystem::DestroyInstance();
		NetworkManager::DestroyInstance();
			
		m_spInstance = NULL;
		
		ProfileManager::Uninit();
		MemoryManager::Uninit();

#ifdef DX11
		if(pDebug)
		{
			pDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
			pDebug->Release();
		}
#endif
	}

	//! Engine initialization
	void Engine::Init()
	{
		GraphicsDriver::Instance()->Init();
		AudioDriver::Instance()->Init();
		StatsManager::CreateInstance();
		m_FadeRenderer = snew FadeRenderer();
		m_bInitialized = true;
	}

	//! Loads a new context stack after clearing the existing one
	void Engine::LoadContextStack(const char* strPath, bool bImmediate /*= false*/, LoadingFinishedCallback* pCallback /*= NULL*/)
	{
		if(bImmediate)
		{
			m_aEntityOperations.clear();
			m_ContextStack = snew ContextStack();
			m_ContextStack->LoadFromXML(strPath);
			m_ContextStack->Init();
			m_ContextStackToLoad.clear();
		}
		else
		{
			m_ContextStackToLoad = strPath;
			m_pLoadingFinishedCallback = pCallback;
		}
	}

	//! pushs a new context
	void Engine::PushContext(const char* strPath)
	{
		Object* pObject = ObjectManager::Instance()->Get(strPath);
		Context* pContext = DYNAMIC_CAST(pObject, Context);
		pContext->Init();
		m_ContextStack->AddChild(pContext, true);
	}

	//! returns the top context
	Context* Engine::GetTopContext() const
	{ 
		if(GetNumContexts() > 0)
		{
			return GetContext(GetNumContexts()-1);
		}
		return NULL;
	}
	
	//! called during the update of the engine
	bool Engine::Update(f32 fDeltaTime)
	{
		SHOOT_PROFILE_FUNC

		if(IsLoading())
		{
			UpdateLoadingThreadStatus();
			return true;
		}

		if(m_bPaused)
		{
			AudioDriver::Instance()->Update();
			return true;
		}

		if(!m_ContextStackToLoad.empty())
		{
			m_aEntityOperations.clear();
			m_ContextStack = NULL;

			OnLoadingStarted();
			m_pLoadingThread = Thread::CreateThread();
			m_pLoadingThread->Start(&LoadingThread);
			m_FadeRenderer->StartFade(Color::Black, Color::Zero, 1.0f);
			return true;
		}

		if(m_bRequestResourceReload)
		{
			OnLoadingStarted();
			m_pLoadingThread = Thread::CreateThread();
			m_pLoadingThread->Start(&ResourceReloadThread);
			m_bRequestResourceReload = false;
			return true;
		}

		CalculateFPS(fDeltaTime);

		TimeManager::Instance()->Update();
		AudioDriver::Instance()->Update();

		if(!m_aEntityOperations.empty())
		{
			DoEntityOperations();
		}

		m_ContextStack->Update();

#ifndef _RETAIL_
		if(StatsManager::IsCreated())
		{
			StatsManager::Instance()->Update();
		}
#endif // _RETAIL_

		EventManager::Instance()->Update();

		ResourceManager::Instance()->Update();

		return !m_bExitRequested;
	}

	//! called during the rendering the engine
	void Engine::Render()
	{
		SHOOT_PROFILE_FUNC

		if(IsLoading())
		{
			return;
		}

		GraphicsDriver::Instance()->Begin();

		EntityRenderer::Instance()->Clear();

		m_ContextStack->RegisterForRendering();

#ifndef _RETAIL_
		if(StatsManager::IsCreated())
		{
			StatsManager::Instance()->RegisterForRendering();
		}
#endif // _RETAIL_
		
		EntityRenderer::Instance()->Render();
	}	

	//! resizes the screen surface
	void Engine::ResizeScreen(const Size& newSize)
	{
		Log.Print("Engine::ResizeScreen(%d, %d)\n", newSize.Width, newSize.Height);
		GraphicsDriver::Instance()->ResizeScreen(newSize);

		if(m_bInitialized)
		{
			EntityRenderer::Instance()->Init(newSize);
		}

		if(m_FadeRenderer.IsValid())
		{
			m_FadeRenderer->Resize();
		}
	}

	//! called when the application is paused
	void Engine::OnPause()
	{
		m_bPaused = true;
		Log << "Engine::OnPause()\n";
		// TODO: might need to queue and process in Update(), audio is not thread safe
		AudioDriver::Instance()->Pause();
	}

	//! called when the application is resumed
	void Engine::OnResume()
	{
		Log << "Engine::OnResume()\n";
		// TODO: might need to queue and process in Update(), audio is not thread safe
		m_bPaused = false;
		AudioDriver::Instance()->Resume();
	}

	//! calculates FPS
	void Engine::CalculateFPS(f32 fDeltaTime)
	{
#ifndef SHOOT_EDITOR
		g_fDeltaTime = Math::Min(fDeltaTime, 0.033333f);
#else
		g_fDeltaTime = fDeltaTime;
#endif
		
		// calc FPS
		m_frameTimer += fDeltaTime;
		m_numFrames++;
		m_numFramesPerSecond++;
		if(m_frameTimer >= 1.0f)
		{
			m_fps = f32(m_numFramesPerSecond) / m_frameTimer;
			m_numFramesPerSecond = 0;
			m_frameTimer = 0.0f;					
		}
	}

	//! adds an entity operation
	void Engine::AddEntityOperation(const EntityOperationInfo& info)
	{
		// make sure there are no duplicate operations		
		if(std::find(m_aEntityOperations.begin(), m_aEntityOperations.end(), info) == m_aEntityOperations.end())
		{
			m_aEntityOperations.push_back(info);
		}
	}

	//! do entity operation
	void Engine::DoEntityOperations()
	{
		// sort operations - always do add operations first
		std::sort(m_aEntityOperations.begin(), m_aEntityOperations.end(), EntityOperationInfoComparator);

		for(u32 i=0; i<m_aEntityOperations.size(); ++i)
		{
			EntityOperationInfo& info = m_aEntityOperations[i];
			switch(info.m_Operation)
			{
			case EO_AddChild:
				{
					SHOOT_ASSERT(info.m_Object->IsA(Entity::TypeID), "Invalid Entity Operation");
					Entity* pEntity = static_cast<Entity*>(info.m_Object.Get());
					info.m_Entity->AddChild(pEntity, true);
					if(!pEntity->IsInitialized())
					{
						pEntity->Init();
					}

					// notify the quadtree
					if(QuadTree* pQuadTree = pEntity->GetRootContext()->GetQuadTree())
					{
						pQuadTree->OnEntityAdded(pEntity);
					}
				}
				break;

			case EO_RemoveChild:
				SHOOT_ASSERT(info.m_Object->IsA(Entity::TypeID), "Invalid Entity Operation");				
				info.m_Entity->RemoveChild(static_cast<Entity*>(info.m_Object.Get()), true);
				break;

			case EO_AddComponent:
				{
					SHOOT_ASSERT(info.m_Object->IsA(Component::TypeID), "Invalid Entity Operation");
					Component* pComponent = static_cast<Component*>(info.m_Object.Get());
					info.m_Entity->AddComponent(pComponent, true);
					pComponent->Init();
				}
				break;

			case EO_RemoveComponent:
				SHOOT_ASSERT(info.m_Object->IsA(Component::TypeID), "Invalid Entity Operation");
				info.m_Entity->RemoveComponent(static_cast<Component*>(info.m_Object.Get()), true);
				break;

			default: SHOOT_ASSERT(0, "Invalid E_EntityOperation");
			}
		}

		m_aEntityOperations.clear();
	}

	//! entity operation comparator
	bool Engine::EntityOperationInfoComparator(const EntityOperationInfo& _1, const EntityOperationInfo& _2)
	{
		return _1.m_Operation < _2.m_Operation;
	}

	//! loading thread
	void* Engine::LoadingThread(void* pParam)
	{
		Engine* pEngine = Engine::Instance();
		
		pEngine->m_ContextStack = snew ContextStack();
		pEngine->m_ContextStack->LoadFromXML(pEngine->m_ContextStackToLoad.c_str());
		pEngine->m_ContextStack->Init();
		pEngine->m_ContextStackToLoad.clear();
		pEngine->m_bLoadingThreadFinished = true;
		return NULL;
	}

	
	//! resource reload thread
	void* Engine::ResourceReloadThread(void* pParam)
	{
		ResourceManager::Instance()->ReloadResources();
		Engine::Instance()->m_bResourceReloadThreadFinished = true;
		return NULL;
	}

	//! updates loading thread status
	void Engine::UpdateLoadingThreadStatus()
	{
		AudioDriver::Instance()->Update();
		if(m_bLoadingThreadFinished)
		{
			sdelete(m_pLoadingThread);
			if(m_pLoadingFinishedCallback)
			{
				m_pLoadingFinishedCallback();
				m_pLoadingFinishedCallback = NULL;
			}
			m_bLoadingThreadFinished = false;
		}
		else if(m_bResourceReloadThreadFinished)
		{
			sdelete(m_pLoadingThread);
			if(m_pResourceReloadFinishedCallback)
			{
				m_pResourceReloadFinishedCallback();
				m_pResourceReloadFinishedCallback = NULL;
			}
			m_bResourceReloadThreadFinished = false;
		}
	}

	//! removes an entity
	void Engine::RemoveEntity(Entity* pEntity)
	{
		if(pEntity->GetParent())
		{
			pEntity->GetParent()->RemoveChild(pEntity);
		}
		else
		{
			for(std::vector<EntityOperationInfo>::iterator it = m_aEntityOperations.begin(); it != m_aEntityOperations.end(); )
			{
				if((*it).m_Object.Get() == pEntity)
				{
					it = m_aEntityOperations.erase(it);
				}
				else
				{
					++it;
				}
			}
		}
	}

	//! requests resource reload
	void Engine::RequestResourceReload(LoadingFinishedCallback* pCallback /*= NULL*/) 
	{
		m_bRequestResourceReload = true;
		m_pResourceReloadFinishedCallback = pCallback;
	}

	//! requests an application exit
	void Engine::RequestExit()
	{
		m_bExitRequested = true;
	}
}

