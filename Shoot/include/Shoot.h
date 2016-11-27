/* 

Amine Rehioui
Created: February 5th 2010

*/

//! common shoot engine headers
#ifndef _SHOOT_H_INCLUDED_
#define _SHOOT_H_INCLUDED_

#include <list>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <sstream>

// determine shoot platform
#define SHOOT_PLATFORM_UNDEFINED	0
#define SHOOT_PLATFORM_WIN32		1
#define SHOOT_PLATFORM_IOS			2
#define SHOOT_PLATFORM_ANDROID		3
#define SHOOT_PLATFORM_WP8			4

#if defined(TARGET_OS_IPHONE)
#define SHOOT_PLATFORM SHOOT_PLATFORM_IOS
#elif defined(__ANDROID__)
#define SHOOT_PLATFORM SHOOT_PLATFORM_ANDROID
#elif defined(WINAPI_FAMILY_PHONE_APP)
#define SHOOT_PLATFORM SHOOT_PLATFORM_WP8
#elif defined(_WIN32)
#define SHOOT_PLATFORM SHOOT_PLATFORM_WIN32
#else
#define SHOOT_PLATFORM SHOOT_PLATFORM_UNDEFINED
#error UNSUPPORTED PLATFORM!
#endif

#include "Types.h"
#include "Alloc.h"
#include "Debug.h"
#include "ProfileManager.h"
#include "Reference.h"
#include "Handle.h"
#include "MathUtils.h"
#include "PropertyStream.h"
#include "Property.h"
#include "Matrix44.h"
#include "Object.h"
#include "Component.h"
#include "Entity.h"
#include "Entity3D.h"
#include "Entity2D.h"
#include "Singleton.h"
#include "GraphicObject.h"
#include "Texture.h"
#include "GraphicsDriver.h"
#include "GraphicComponent.h"
#include "EntityRenderer.h"
#include "ObjectManager.h"
#include "ObjectFactory.h"
#include "Engine.h"
#include "ResourceManager.h"
#include "Random.h"
#include "Log.h"

#if SHOOT_PLATFORM == SHOOT_PLATFORM_WIN32 || SHOOT_PLATFORM == SHOOT_PLATFORM_WP8
#include <winsock2.h>
#include <windows.h>
#ifdef GetClassName
#undef GetClassName
#endif // GetClassName
#endif

#if defined(DX11)
#include <d3d11_1.h>
#include "COMReference.h"

#define DX_ASSERT(x) SHOOT_VERIFY(SUCCEEDED(x), #x" failed");
#define DX_CHECK(x) SHOOT_LOG_WARNING(SUCCEEDED(x), #x" failed");
#else
#include "OpenGL.h"
#endif

namespace shoot
{
	extern f32 g_fDeltaTime;
}

#endif //_SHOOT_H_INCLUDED_

