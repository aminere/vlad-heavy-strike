/* 

Amine Rehioui
Created: April 25th 2012

*/

#include "Shoot.h"

#include "FileFS.h"

#include "FileSystem.h"

namespace shoot
{
	//! constructor
	FileFS::FileFS(const char* strPath) 
		: File(strPath, M_ReadBinary)
		, m_pData(NULL)
		, m_Size(0)
		, m_CurrentOffset(0)
	{
		// clamp path to 32 characters
		if(m_strPath.length() > FileSystem::MaxFilePathLength-1)
		{
			m_strPath = m_strPath.substr(0, FileSystem::MaxFilePathLength-1);			
		}
	}	

	//! opens the file
	bool FileFS::Open(bool bAssertOnFailure /*= true*/)
	{
        SHOOT_ASSERT(FileSystem::Instance()->Exists(m_strPath.c_str()), "Could not open file %s", m_strPath.c_str());

		std::vector<FileSystem::PartitionInfo>& aPartitions = FileSystem::Instance()->GetPartitions();
		FileSystem::FileMap& fileMap = FileSystem::Instance()->GetFileMap();
		FileSystem::FileInfo& fileInfo = fileMap[m_strPath];		
        FileSystem::PartitionInfo& partition = aPartitions[fileInfo.partition];

		if(!partition.pData)
		{
			Log.Print("LOADING PARTITION %s..\n", partition.name.c_str());
			FileSystem::Instance()->LoadPartition(fileInfo.partition);
		}

		m_Size = fileInfo.size;
		SHOOT_ASSERT(fileInfo.offset+m_Size <= partition.dataSize, "File outside partition");

		m_pData = partition.pData + fileInfo.offset;

		return true;
	}	

	//! reads data from the file
	s32 FileFS::Read(void* pDest, u32 bytesToRead)
	{
		SHOOT_ASSERT(m_CurrentOffset+bytesToRead <= m_Size, "EOF reached");
		u8* pData = m_pData + m_CurrentOffset;
		memcpy(pDest, pData, bytesToRead);
		m_CurrentOffset += bytesToRead;
		return bytesToRead;		
	}

	//! changes the current read/write location in the file
	void FileFS::SetOffset(s32 offset, E_OffsetType eType /*= OT_Current*/)
	{
		switch(eType)
		{
		case OT_Current:
			m_CurrentOffset += offset;
			break;

		case OT_Start:
			m_CurrentOffset = offset;
			break;

		case OT_End:
			m_CurrentOffset = offset+m_Size;
			break;
		}
		SHOOT_ASSERT(m_CurrentOffset >= 0 && m_CurrentOffset <= s32(m_Size), "Invalid file offset");
	}

	//! returns the current read/write location in the file
	u32 FileFS::GetOffset()
	{
		return u32(m_CurrentOffset);
	}

	//! returns true if end of file has been reached
	bool FileFS::EOFReached()
	{
		return (m_CurrentOffset >= s32(m_Size));
	}	
}

