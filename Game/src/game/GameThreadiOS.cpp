/* 

Amine Rehioui
Created: May 20th 2013

*/

#include "ShootTest.h"
#include "GameThreadiOS.h"
 
#include "OpenGLExtensionHandlerES_1_1.h"
#include "Thread.h"

namespace shoot
{
	//! static vars
	GameThreadiOS::Info GameThreadiOS::m_Info;
	Semaphore* GameThreadiOS::m_spSemaphore = NULL;

	//! starts the thread
	void GameThreadiOS::Start(const Info& info)
	{
		m_spSemaphore = Semaphore::Create(0);
		
		m_Info = info;
		Thread* pThread = Thread::CreateThread();
		pThread->Start(&Run);
		sdelete(pThread);
	}

	//! runs the thread
	void* GameThreadiOS::Run(void* pParam)
	{
		OpenGLExtensionHandler* pExtHandler = OpenGLExtensionHandler::Instance();
		Engine* pEngine = Engine::Instance();
		
		m_spSemaphore->Wait();
		m_spSemaphore->Wait();		
		m_Info.pSetContextCallback();		
		
		while(m_Info.pIsRunningCallback())
		{
			m_spSemaphore->Wait();
			pExtHandler->extGlBindFramebuffer(GL_FRAMEBUFFER_OES, m_Info.frameBufferID);
			pExtHandler->extGlBindRenderbuffer(GL_RENDERBUFFER_OES, m_Info.colorBufferID);
			f32 fDeltaTime = m_Info.pGetDeltaTimeCallback();
			m_Info.pApplyInputCallback();
			pEngine->Update(fDeltaTime);
			pEngine->Render();
			m_Info.pPresentCallback();
		}
		
		return 0;
	}
}

