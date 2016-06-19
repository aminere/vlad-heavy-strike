/* 

Amine Rehioui
Created: April 21st 2012

*/

#ifndef _MUTEX_PT_H_INCLUDED_
#define _MUTEX_PT_H_INCLUDED_

#include "Mutex.h"

namespace shoot
{
	//! mutex class
	class MutexPT : public Mutex
	{
	public:

		//! constructor
		MutexPT();

		//! destructor
		~MutexPT();

		//! Attempts to lock the mutex
		void Lock();

		//! Unlocks the mutex
		void Unlock();

	private:

		void* m_pMutex;
	};	
}

#endif // _MUTEX_PT_H_INCLUDED_

