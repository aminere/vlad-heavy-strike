/* 

Amine Rehioui
Created: April 7th 2013

*/

#ifndef _THREAD_PT_H_INCLUDED_
#define _THREAD_PT_H_INCLUDED_

#include "Thread.h"

namespace shoot
{
	//! ThreadPT class
	class ThreadPT : public Thread
	{
	public:

		//! constructor
		ThreadPT();

		//! starts the thread
		void Start(ThreadFunc threadFunc, void* pParam = 0);

		//! yields the thread execution
		static void YieldThread();

	private:

		void* m_pThread;
	};
}

#endif // _THREAD_PT_H_INCLUDED_

