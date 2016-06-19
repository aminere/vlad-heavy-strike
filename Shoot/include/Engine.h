/* 

Amine Rehioui
Created: February 21th 2010

*/

#ifndef _ENGINE_H_INCLUDED_
#define _ENGINE_H_INCLUDED_

#include "InputManager.h"

#include "Context.h"
#include "ContextStack.h"

#include "Size.h"

#include "EntityOperation.h"
#include "FadeRenderer.h"

namespace shoot
{
	// forwards
	class Thread;

	//! Shoot Engine base class
	class Engine
	{
	public:

		//! Object registration callback
		typedef void ObjectRegistrationCallback(void);
		
		//! Loading finished callback
		typedef void LoadingFinishedCallback(void);

		//! returns the unique instance of the engine
		inline static Engine* Instance() { return m_spInstance; }

		//! Constructor
		Engine(ObjectRegistrationCallback* pRegisterObjects = NULL,
			   GraphicsDriver::E_DriverType _eDriverType = GraphicsDriver::DT_OpenGL,
			   InputManager::E_Type _eInputManagerType = InputManager::IMT_SDL);

		//! Destructor
		virtual ~Engine();

		//! Engine initialization
		virtual void Init();

		//! called during the update of the engine
		virtual bool Update(f32 fDeltaTime);

		//! called during the rendering the engine
		virtual void Render();

		//! resizes the screen surface
		virtual void ResizeScreen(const Size& newSize);

		//! called when the application is paused
		void OnPause();

		//! called when the application is resumed
		virtual void OnResume();
		
		//! Loads a new context stack after clearing the existing one
		void LoadContextStack(const char* strPath, bool bImmediate = false, LoadingFinishedCallback* pCallback = NULL);

		//! pushs a new context
		void PushContext(const char* strPath);

		//! pops the top context
		inline void PopContext() const { GetTopContext()->Pop(); }

		//! returns the number of contexts in the stack
		inline u32 GetNumContexts() const { return m_ContextStack->GetChildCount(); }

		//! returns a context by index
		inline Context* GetContext(u32 index) const { return static_cast<Context*>(m_ContextStack->GetChild(index)); }

		//! returns the top context
		Context* GetTopContext() const;

		//! returns the context stack
		ContextStack* GetContextStack() { return m_ContextStack; }
		
		//! returns the engine fps
		f32 GetFPS() const { return m_fps; }

		//! returns the current frame
		u32 GetCurrentFrame() const { return m_numFrames; }

		//! calculates FPS
		void CalculateFPS(f32 fDeltaTime);

		//! adds an entity operation
		void AddEntityOperation(const EntityOperationInfo& info);

		//! returns the loading status
		inline bool IsLoading() const { return m_pLoadingThread != NULL; }

		//! returns running status
		inline bool IsRunning() const { return !m_bExitRequested; }

		//! returns paused status
		inline bool IsPaused() const { return m_bPaused; }

		//! removes an entity
		void RemoveEntity(Entity* pEntity);

		//! requests resource reload
		void RequestResourceReload(LoadingFinishedCallback* pCallback = NULL);

		//! requests an application exit
		void RequestExit();

	protected:

		Reference<ContextStack> m_ContextStack;
		Reference<FadeRenderer> m_FadeRenderer;
		std::string m_ContextStackToLoad;
		bool m_bRequestResourceReload;
		Thread* m_pLoadingThread;
		LoadingFinishedCallback* m_pLoadingFinishedCallback;
		bool m_bLoadingThreadFinished;
		LoadingFinishedCallback* m_pResourceReloadFinishedCallback;
		bool m_bResourceReloadThreadFinished;

		// fps calculation
		f32 m_fps;
		f32 m_frameTimer;
		u32 m_numFrames;
		u32 m_numFramesPerSecond;

		//! do entity operation
		void DoEntityOperations();

		std::vector<EntityOperationInfo> m_aEntityOperations;

		//! entity operation comparator
		static bool EntityOperationInfoComparator(const EntityOperationInfo& _1, const EntityOperationInfo& _2);

		//! loading thread
		static void* LoadingThread(void* pParam);

		//! resource reload thread
		static void* ResourceReloadThread(void* pParam);

		//! updates loading thread status
		void UpdateLoadingThreadStatus();

		//! called when the loading thread has started
		virtual void OnLoadingStarted() { }

	private:

		static Engine* m_spInstance;

		bool m_bInitialized;
		bool m_bPaused;
		bool m_bExitRequested;
	};
}

#endif // _ENGINE_H_INCLUDED_

