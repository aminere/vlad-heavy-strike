/* 

Amine Rehioui
Created: July 14th 2010

*/

#include "Shoot.h"

#include "VertexBuffer.h"

#include "GraphicExtensionHandler.h"

#if defined(DX11)
#include "DirectX11Driver.h"
#elif SHOOT_PLATFORM == SHOOT_PLATFORM_IOS || SHOOT_PLATFORM == SHOOT_PLATFORM_ANDROID
#include "OpenGLDriverES_1_1.h"
#include "OpenGLDriverES_2_0.h"
#else
#include "OpenGLDriver.h"
#endif

namespace shoot
{
	GraphicsDriver* GraphicsDriver::m_spInstance = NULL;

	//! creates a driver - the singleton pattern was not used because GraphicsDriver has pure virtual methods
	void GraphicsDriver::CreateInstance(E_DriverType eType)
	{
		SHOOT_ASSERT(!m_spInstance, "Cannot create multiple GraphicsDriver instances");
		
		switch(eType)
		{
		case DT_NULL:
			m_spInstance = snew NullGraphicsDriver();
			break;

#if SHOOT_PLATFORM == SHOOT_PLATFORM_IOS || SHOOT_PLATFORM == SHOOT_PLATFORM_ANDROID
		case DT_OpenGLES_1_1:
			m_spInstance = snew OpenGLDriverES_1_1();
			break;

		case DT_OpenGLES_2_0:
			m_spInstance = snew OpenGLDriverES_2_0();
			break;
#elif defined(DX11)
		case DT_DirectX11:
			m_spInstance = snew DirectX11Driver();
			break;		
#else
		case DT_OpenGL:
			m_spInstance = snew OpenGLDriver();
			break;
#endif

		default:
			SHOOT_ASSERT(false, "GraphicsDriver not supported on this platform");
		}

		m_spInstance->m_eType = eType;
	}

	//! destroys the driver
	void GraphicsDriver::DestroyInstance()
	{
		SHOOT_ASSERT(m_spInstance, "Cannot destroy NULL GraphicsDriver instance");
		delete m_spInstance;
	}

	//! driver initialization
	void GraphicsDriver::Init()
	{
		GraphicExtensionHandler::CreateInstance();
	}

	//! Constructor
	GraphicsDriver::GraphicsDriver()
		: m_eType(DT_NULL)
	{
		for(u32 i=0; i<TS_Count; ++i)
		{
			m_StateMatrices[i] = Matrix44::Identity;
		}
	}

	//! Destructor
	GraphicsDriver::~GraphicsDriver()
	{
		GraphicExtensionHandler::DestroyInstance();

		SHOOT_ASSERT(m_GraphicObjectMap.empty(), "GraphicObject Leaks Detected");
		m_spInstance = NULL;
	}

	//! resizes the screen surface
	void GraphicsDriver::ResizeScreen(const Size& newSize)
	{
		SetViewPort(AABBox2D(Vector2(0.0f, 0.0f), Vector2(f32(newSize.Width), f32(newSize.Height))));
	}

	//! registers a graphic object
	void GraphicsDriver::RegisterObject(GraphicObject* pObject)
	{
		SHOOT_ASSERT(m_GraphicObjectMap.find(pObject) == m_GraphicObjectMap.end(), "Trying to register a GraphicObject twice");
		m_GraphicObjectMap[pObject] = 0;
	}

	//! unregister a graphic object
	void GraphicsDriver::UnregisterObject(GraphicObject* pObject)
	{
		SHOOT_ASSERT(m_GraphicObjectMap.find(pObject) != m_GraphicObjectMap.end(), "Trying to unregister an unexisting GraphicObject");
		m_GraphicObjectMap.erase(pObject);
	}

	//! unloads graphic objects
	void GraphicsDriver::UnloadObjects()
	{
		for(GraphicObjectMap::iterator it = m_GraphicObjectMap.begin(); it != m_GraphicObjectMap.end(); ++it)
		{
			(*it).first->GraphicUnload();
		}
	}

	//! loads graphic objects
	void GraphicsDriver::LoadObjects()
	{
		for(GraphicObjectMap::iterator it = m_GraphicObjectMap.begin(); it != m_GraphicObjectMap.end(); ++it)
		{
			if(!(*it).first->IsLoaded())
			{
				(*it).first->GraphicLoad();
			}
		}
	}

	//! create a vertex buffer
	VertexBuffer* NullGraphicsDriver::CreateVertexBuffer()
	{ 
		return snew VertexBuffer();
	}

	//! create a skybox vertex buffer
	VertexBuffer* NullGraphicsDriver::CreateSkyBoxVertexBufferNoCubemap(Texture* pTexture)
	{ 
		return snew VertexBuffer();
	}
}

