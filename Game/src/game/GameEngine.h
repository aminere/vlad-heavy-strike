/* 

Amine Rehioui
Created: June 24th 2011

*/

#ifndef _GAME_ENGINE_H_INCLUDED_
#define _GAME_ENGINE_H_INCLUDED_

#include "Texture.h"

#include "GameManager.h"
#include "AudioManager.h"

namespace shoot
{
	//! Game Engine
	class GameEngine : public Engine				    			   
	{
		typedef Engine super;

	public:

		//! constructor
		GameEngine(ObjectRegistrationCallback* pRegisterObjects,
				   GraphicsDriver::E_DriverType _eDriverType = GraphicsDriver::DT_OpenGL,				   
				   InputManager::E_Type _eInputManagerType = InputManager::IMT_SDL);

		//! destructor
		virtual ~GameEngine();

		//! Engine initialization
		virtual void Init();

		//! called during the update of the engine
		/** \return: false to exit the program */
		virtual bool Update(f32 fDeltaTime);

		//! called during the rendering the engine
		virtual void Render();

		//! called when the application is resumed
		void OnResume();

		//! called when the resources have been reloaded
		static void OnResourcesReloaded();

	protected:

		//! Processes resource reload request
		void OnResourceReloadRequest();

		//! called when the loading thread has started
		void OnLoadingStarted();

		Reference<GameManager> m_GameManager;
		Reference<AudioManager> m_AudioManager;
		Reference<ContextStack> m_LoadingScreen;
	};
}

#endif // _GAME_ENGINE_H_INCLUDED_

