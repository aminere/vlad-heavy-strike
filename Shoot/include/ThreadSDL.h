/* 

Amine Rehioui
Created: November 27th 2011

*/

#ifndef _THREAD_SDL_H_INCLUDED_
#define _THREAD_SDL_H_INCLUDED_

#include "Thread.h"

namespace shoot
{
	//! ThreadSDL class
	class ThreadSDL : public Thread
	{
	public:

		//! constructor
		ThreadSDL();

		//! starts the thread
		void Start(ThreadFunc threadFunc, void* pParam = 0);

	private:

		void* m_pThread;
	};
}

#endif // _THREAD_SDL_H_INCLUDED_

