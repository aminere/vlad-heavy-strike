/* 

Amine Rehioui
Created: April 7th 2013

*/

#ifndef _SEMAPHORE_PT_H_INCLUDED_
#define _SEMAPHORE_PT_H_INCLUDED_

#include "SSemaphore.h"

namespace shoot
{
	//! SemaphorePT class
	class SemaphorePT : public Semaphore
	{
	public:

		//! constructor
		SemaphorePT(u32 value);

		//! destructor
		~SemaphorePT();

		//! Decrement the semaphore and block the thread if necessary
		void Wait();

		//! Increment the semaphore and unlock waiting threads if necessary
		void Post();

	private:

		void* m_pSemaphore;
	};	
}

#endif // _SEMAPHORE_PT_H_INCLUDED_

