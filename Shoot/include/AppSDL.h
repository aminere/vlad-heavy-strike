/* 

Amine Rehioui
Created: August 4th 2010

*/

#ifndef _APP_SDL_H_INCLUDED_
#define _APP_SDL_H_INCLUDED_

#include "App.h"
#include <SDL.h>

namespace shoot
{
	//! AppSDL class
	class AppSDL : public App
	{
		typedef App super;

	public:

		//! constructor
		AppSDL(Size screenSize = Size(800, 600),
			   bool bFullScreen = false,
			   bool bUseSDLWindow = true,
			   u8 _BitsPerPixel = 24, 
			   u32 SDLFlags = SDL_INIT_VIDEO, 
			   u32 VideoFlags = SDL_WINDOW_OPENGL);

		//! destructor
		~AppSDL();

		//! main loop
		void Run();

		//! updates the app
		bool Update(f32 fDeltaTime);

	private:

		SDL_Window* m_Window;
		bool m_bUseSDLWindow;
	};
}

#endif // _APP_SDL_H_INCLUDED_

