/* 

Amine Rehioui
Created: October 18th 2011

*/

#include "Shoot.h"

#include "MemoryManager.h"



#include <sstream>

namespace shoot
{
	// static variables initialization
	MemoryManager::AllocMap* MemoryManager::m_pAllocMap = new MemoryManager::AllocMap();
	s32 MemoryManager::m_UsedMemory = 0;
	Mutex* MemoryManager::m_pMutex = Mutex::Create();
	bool MemoryManager::m_bInitialized = false;

	//! allocates memory
	void* MemoryManager::Alloc(u32 size, const char* strFileName, u32 line)
	{
		void* pMemory = malloc(size);
		SHOOT_ASSERT(pMemory, "Could not allocate memory");	

#if SHOOT_PLATFORM == SHOOT_PLATFORM_WIN32
		AllocInfo info = { size, strFileName, line };

		m_pMutex->Lock();
		(*m_pAllocMap)[pMemory] = info;
		m_UsedMemory += size;
		m_pMutex->Unlock();
#endif

		return pMemory;
	}
		
	//! frees memory
	void MemoryManager::Free(void* pMemory)
	{
#if SHOOT_PLATFORM == SHOOT_PLATFORM_WIN32
		if(m_bInitialized)
		{
			if(!(*m_pAllocMap).empty())
			{
				m_pMutex->Lock();
				std::map<void*, AllocInfo>::iterator it = (*m_pAllocMap).find(pMemory);
				m_pMutex->Unlock();

				if(it != (*m_pAllocMap).end())
				{
					m_pMutex->Lock();
					m_UsedMemory -= it->second.Size;
					(*m_pAllocMap).erase(it);
					m_pMutex->Unlock();
				}
			}
		}
#endif

		free(pMemory);
	}

	//! checks for leaks
	void MemoryManager::LeakCheck()
	{
		if(m_pAllocMap && !(*m_pAllocMap).empty())
		{
			SHOOT_ASSERT(false, "memory leak detected: %d bytes", m_UsedMemory);
			for(std::map<void*, AllocInfo>::iterator it = (*m_pAllocMap).begin(); it != (*m_pAllocMap).end(); it++)
			{
				AllocInfo info = it->second;
				std::stringstream addr;
				addr << "0x" << std::hex << *(u32*)it->first;
				Log << info.Size << " bytes @ " << info.strFileName << " line " << info.Line << " addr " << addr.str().c_str() << "\n";				
			}
		}
	}
}

