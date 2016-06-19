/* 

Amine Rehioui
Created: July 6th 2013

*/

#include "Shoot.h"

#include "GraphicExtensionHandler.h"

#if SHOOT_PLATFORM == SHOOT_PLATFORM_IOS || SHOOT_PLATFORM == SHOOT_PLATFORM_ANDROID
#if defined(USE_GLES_1_1) || SHOOT_PLATFORM == SHOOT_PLATFORM_ANDROID // TODO support ES2 on Android
#include "OpenGLExtensionHandlerES_1_1.h"
#elif defined(USE_GLES_2_0)
#include "OpenGLExtensionHandlerES_2_0.h"
#endif
#elif !defined(DX11)
#include "OpenGLExtensionHandlerWin32.h"
#endif

namespace shoot
{
	//! static variables initialization
	GraphicExtensionHandler* GraphicExtensionHandler::m_spInstance = NULL;

	//! CreateInstance
	void GraphicExtensionHandler::CreateInstance()
	{
		if(m_spInstance)
		{
			// do nothing (necessary in tool mode)
			// TODO: print warning?
		}
		else
		{
			#if defined(DX11)
				m_spInstance = snew GraphicExtensionHandler();
			#elif SHOOT_PLATFORM == SHOOT_PLATFORM_IOS || SHOOT_PLATFORM == SHOOT_PLATFORM_ANDROID
				#if defined(USE_GLES_1_1) || SHOOT_PLATFORM == SHOOT_PLATFORM_ANDROID // TODO support ES2 on Android
					m_spInstance = snew OpenGLExtensionHandler_ES_1_1();
				#elif defined(USE_GLES_2_0)
					m_spInstance = snew OpenGLExtensionHandler_ES_2_0();
				#endif
			#else
				m_spInstance = snew OpenGLExtensionHandlerWin32();
			#endif

			SHOOT_ASSERT(m_spInstance, "could not create an OpenGLExtensionHandler on this platform");
		}		
	}

	//! destroys the driver
	void GraphicExtensionHandler::DestroyInstance()
	{
		sdelete(m_spInstance);
	}

	//! Constructor
	GraphicExtensionHandler::GraphicExtensionHandler()	
	{
		for(u32 i=0; i<E_Count; ++i)
		{
			m_bHasExtension[i] = false;
		}		
	}
}

