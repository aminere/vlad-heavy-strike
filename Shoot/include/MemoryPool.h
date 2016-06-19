/* 

Amine Rehioui
Created: February 29th 2012

*/

#ifndef _MEMORY_POOL_H_INCLUDED_
#define _MEMORY_POOL_H_INCLUDED_

namespace shoot
{
	//! memory pool
	class MemoryPool : public Object
	{
		DECLARE_OBJECT(MemoryPool, Object);

	public:

		//! constructor
		MemoryPool(u32 size);

		//! constructor
		MemoryPool()
		{
			SHOOT_ASSERT(false, "Default constructor not handled.");
		}

		//! destructor
		~MemoryPool();

		static const u32 AllocHeaderSize = sizeof(u32);

		//! Creates and object in the pre-allocated buffer
		template <class T>
		T* Alloc()
		{
			bool bFound = false;
			u32 allocSize = sizeof(T)+AllocHeaderSize;

			std::list<Chunk>::iterator it = m_FreeChunks.begin();
			for(; it != m_FreeChunks.end(); it++)
			{
				if((*it).m_Size >= allocSize)
				{
					bFound = true;
					break;
				}
			}

			if(bFound)
			{
				u8* pMemory = (*it).m_pAddr;
				*((u32*)pMemory) = allocSize;
				// use T instead of T() ctor to avoid compiler warning (level 2) C4345
				// object will have no default initialization
				T* t = new(pMemory+AllocHeaderSize) T;
				(*it).m_pAddr += allocSize;
				(*it).m_Size -= allocSize;
				if((*it).m_Size == 0)
				{
					m_FreeChunks.erase(it);
				}
				return t;
			}

			SHOOT_ASSERT(false, "MemoryPool overflow");
			return NULL;			
		}

		//! Frees an object from the pre-allocated buffer		
		void Free(void *pAddr)
		{
			u8* pMemory = ((u8*)pAddr)-AllocHeaderSize;
			u32 size = *((u32*)pMemory);
			Chunk freeChunk = { pMemory, size };
			m_FreeChunks.push_front(freeChunk);
		}

	private:

		//! chunk info
		struct Chunk
		{
			u8* m_pAddr;
			u32 m_Size;			
		};

		u8* m_Pool;
		std::list<Chunk> m_FreeChunks;		
	};
}

#endif // _MEMORY_POOL_H_INCLUDED_

