/* 

Amine Rehioui
Created: April 7th 2013

*/

#ifndef _SEMAPHORE_SDL_H_INCLUDED_
#define _SEMAPHORE_SDL_H_INCLUDED_

#include "SSemaphore.h"

namespace shoot
{
	//! SemaphoreSDL class
	class SemaphoreSDL : public Semaphore
	{
	public:

		//! constructor
		SemaphoreSDL(u32 value);

		//! destructor
		~SemaphoreSDL();

		//! Decrement the semaphore and block the thread if necessary
		void Wait();

		//! Increment the semaphore and unlock waiting threads if necessary
		void Post();

	private:

		void* m_pSemaphore;
	};	
}

#endif // _SEMAPHORE_SDL_H_INCLUDED_

