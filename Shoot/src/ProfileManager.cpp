/* 

Amine Rehioui
Created: May 16th 2012

*/

#include "Shoot.h"

#include "File.h"

#include <sstream>

namespace shoot
{
	File* ProfileManager::m_pFile = NULL;

	//! init
	void ProfileManager::Init()
	{
#ifdef _PROFILE_
		File::UseExternalWritablePath uwp;
		m_pFile = File::Create("Profile.txt", File::M_Write);
		bool bOpened = m_pFile->Open();
#endif // _PROFILE_
	}

	//! uninit
	void ProfileManager::Uninit()
	{
#ifdef _PROFILE_
		m_pFile->Close();
		sdelete(m_pFile);
#endif // _PROFILE_
	}

	//! print profile info
	void ProfileManager::Print(const char* strInfo)
	{
		m_pFile->Write((void*)strInfo, std::string(strInfo).length());
	}

	//! constructor
	Profiler::Profiler(const char* strFunc)
	{		
		m_strFunc = strFunc;
		std::stringstream ss;
		ss << "[" << Engine::Instance()->GetCurrentFrame() << "][" << g_fDeltaTime << "] " << m_strFunc << " begin\n";
		ProfileManager::Print(ss.str().c_str());

		m_Timer.Start();
	}

	//! destructor
	Profiler::~Profiler()
	{
		u32 time = m_Timer.GetDuration();

		std::stringstream ss;
		ss << "[" << Engine::Instance()->GetCurrentFrame() << "][" << g_fDeltaTime << "] "  << m_strFunc << " end (" << time << "us)\n";
		ProfileManager::Print(ss.str().c_str());
	}
}
