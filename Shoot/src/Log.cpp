/* 

Amine Rehioui
Created: April 1st 2011

*/

#include "Shoot.h"

#include <iostream>

#if defined(SHOOT_EDITOR)
extern void EditorOutput(const char* strMessage);
#endif

#if SHOOT_PLATFORM == SHOOT_PLATFORM_ANDROID
#include <android/log.h>
#endif

namespace shoot
{
	BaseLog Log;

	//! Prints to available output streams
	void BaseLog::Print(const char* format, ...)
	{
		static char buffer[1024];
		va_list va;
		va_start(va, format);
		vsprintf(buffer, format, va);
		va_end(va);
		
		PrintInternal(buffer);
	}

	BaseLog& BaseLog::operator << (const char* str)
	{
		PrintInternal(str);
		return *this;
	}

	BaseLog& BaseLog::operator << (size_t i)
	{
		std::stringstream ss(std::stringstream::in | std::stringstream::out);
		ss << i;
		std::string str = ss.str();
		PrintInternal(str.c_str());
		return *this;
	}

	BaseLog& BaseLog::operator << (float f)
	{
		std::stringstream ss(std::stringstream::in | std::stringstream::out);
		ss << f;
		std::string str = ss.str();
		PrintInternal(str.c_str());
		return *this;
	}

	/*BaseLog& BaseLog::operator << (u32 _u32)
	{
		return operator << (s32(_u32));		
	}

	BaseLog& BaseLog::operator << (s32 _s32)
	{
		std::stringstream ss(std::stringstream::in | std::stringstream::out);
		ss << _s32;
		std::string str = ss.str();
		PrintInternal(str.c_str());
		return *this;
	}

	BaseLog& BaseLog::operator << (f32 _f32)
	{
		std::stringstream ss(std::stringstream::in | std::stringstream::out);
		ss << _f32;
		std::string str = ss.str();
		PrintInternal(str.c_str());
		return *this;
	}*/

	//! Prints to available output streams
	void BaseLog::PrintInternal(const char* str)
	{
//#if !defined(_RETAIL_) || (SHOOT_PLATFORM == SHOOT_PLATFORM_ANDROID)
		std::cout << str;

		#if SHOOT_PLATFORM == SHOOT_PLATFORM_WIN32 || SHOOT_PLATFORM == SHOOT_PLATFORM_WP8
		wchar_t buf[1024];
		mbstowcs(buf, str, 1024);
		OutputDebugString(buf);
		#endif

		#if defined(SHOOT_EDITOR)
		EditorOutput(str);
		#endif

		#if SHOOT_PLATFORM == SHOOT_PLATFORM_ANDROID
		__android_log_print(ANDROID_LOG_INFO, "Shoot", "%s", str);
		#endif
//#endif
	}
}

