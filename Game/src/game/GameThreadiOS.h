/* 

Amine Rehioui
Created: May 20th 2013

*/

#ifndef _GAME_THREAD_IOS_H_INCLUDED_
#define _GAME_THREAD_IOS_H_INCLUDED_

#include "SSemaphore.h"

namespace shoot
{
	//! GameThreadiOS
	class GameThreadiOS
	{
	public:
	
		typedef void SetContextCallback(void);
		typedef f32 GetDeltaTimeCallback(void);
		typedef void PresentCallback(void);
		typedef void ApplyInputCallback(void);
		typedef bool IsRunningCallback(void);

		//! GameThread info
		struct Info
		{
			Info()
				: frameBufferID(0)
				, colorBufferID(0)
				, pSetContextCallback(0)
				, pGetDeltaTimeCallback(0)
				, pPresentCallback(0)
				, pApplyInputCallback(0)
				, pIsRunningCallback(0)
			{
			}

			u32 frameBufferID;
			u32 colorBufferID;
			SetContextCallback* pSetContextCallback;
			GetDeltaTimeCallback* pGetDeltaTimeCallback;
			PresentCallback* pPresentCallback;
			ApplyInputCallback* pApplyInputCallback;
			IsRunningCallback* pIsRunningCallback;			
		};

		//! starts the thread
		static void Start(const Info& info);
		
		//! public semaphore
		static Semaphore* m_spSemaphore;

	private:

		//! runs the thread
		static void* Run(void* pParam);
		static Info m_Info;
	};
}

#endif // _GAME_THREAD_IOS_H_INCLUDED_

