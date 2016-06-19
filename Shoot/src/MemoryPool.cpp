/* 

Amine Rehioui
Created: February 29th 2012

*/

#include "Shoot.h"

#include "MemoryPool.h"

namespace shoot
{
	DEFINE_OBJECT(MemoryPool);

	//! constructor
	MemoryPool::MemoryPool(u32 size)
	{
		m_Pool = snew u8[size];
		Chunk freeChunk = { m_Pool, size };
		m_FreeChunks.push_front(freeChunk);
	}

	//! destructor
	MemoryPool::~MemoryPool()
	{
		sdelete_array(m_Pool);
	}
}

