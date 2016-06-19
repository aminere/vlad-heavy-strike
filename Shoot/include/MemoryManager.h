/* 

Amine Rehioui
Created: October 18th 2011

*/

#ifndef _MEMORY_MANAGER_H_INCLUDED_
#define _MEMORY_MANAGER_H_INCLUDED_

#include "Mutex.h"

namespace shoot
{
	//! memory manager
	class MemoryManager
	{
	public:

		//! initializes the memory manager
		static void Init() { m_bInitialized = true; }
		
		//! uninitialize the memory manager
		static void Uninit() 
		{			
			LeakCheck();
			m_bInitialized = false;
			sdelete(m_pMutex);
			sdelete(m_pAllocMap);
		}
		
		//! allocates memory
		static void* Alloc(u32 size, const char* strFileName, u32 line);
		
		//! frees memory
		static void Free(void* pMemory);

		//! checks for leaks
		static void LeakCheck();

		//! Get used memory in bytes
		static u32 GetUsedMemory() { return m_UsedMemory; }

	private:

		//! Allocation info
		struct AllocInfo
		{
			u32 Size;
			const char* strFileName;
			u32 Line;
		};
		
		typedef std::map<void*, AllocInfo> AllocMap;
		static AllocMap* m_pAllocMap;
		static s32 m_UsedMemory;
		static Mutex* m_pMutex;
		static bool m_bInitialized;
	};
}

#endif // _MEMORY_MANAGER_H_INCLUDED_

