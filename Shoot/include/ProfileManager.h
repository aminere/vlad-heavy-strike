/* 

Amine Rehioui
Created: May 16th 2012

*/

#ifndef _PROFILER_H_INCLUDED_
#define _PROFILER_H_INCLUDED_

#include "HighResTimer.h"

#ifdef _PROFILE_
#define SHOOT_PROFILE_FUNC Profiler profiler(__FUNCTION__);
#define SHOOT_PROFILE(x) Profiler profiler(#x);
#else
#define SHOOT_PROFILE_FUNC
#define SHOOT_PROFILE(x)
#endif // _PROFILE_

namespace shoot
{
	// forwards
	class File;

	//! Profile manager
	class ProfileManager
	{
	public:

		//! init
		static void Init();

		//! uninit
		static void Uninit();

		//! print profile info
		static void Print(const char* strInfo);

	private:

		static File* m_pFile;
	};

	//! Profiler
	class Profiler
	{
	public:

		//! constructor
		Profiler(const char* strFunc);

		//! destructor
		~Profiler();

	private:

		HighResTimer m_Timer;
		const char* m_strFunc;
	};
}

#endif // _PROFILER_H_INCLUDED_

