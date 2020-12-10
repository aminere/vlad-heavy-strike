/* 

Amine Rehioui
Created: April 25th 2012

*/

#include "Shoot.h"

#include "FileSystem.h"

#include "File.h"

#include "zlib.h"

namespace shoot
{
	//! constructor
	FileSystem::FileSystem()
	{
#ifndef SHOOT_EDITOR
		Init();
#endif
	}

	//! destructor
	FileSystem::~FileSystem()
	{
		UnloadAllPartitions(NULL);
	}

	//! initializes the file system
	void FileSystem::Init()
	{
		File* pFile = File::CreateNative(GetDataPath(), File::M_ReadBinary);
		pFile->Open();

		u32 numPartitions;
		pFile->Read(&numPartitions, sizeof(u32));

		for(u32 i=0; i<numPartitions; ++i)
		{
			PartitionInfoWrite partitionInfoWrite;
			pFile->Read(&partitionInfoWrite, sizeof(PartitionInfoWrite));

			PartitionInfo partitionInfo =
			{
				partitionInfoWrite.offset, 
				partitionInfoWrite.size, 
				NULL, 
				partitionInfoWrite.uncompressedSize, 
				std::string(partitionInfoWrite.name)
			};

			m_aPartitions.push_back(partitionInfo);

			SHOOT_ASSERT(partitionInfoWrite.numFiles < MaxFilesPerPartition, "MaxFilesPerPartition reached");
			if(partitionInfoWrite.numFiles > 0)
			{
				FileInfoWrite fileWriteInfos[MaxFilesPerPartition];
				pFile->Read(fileWriteInfos, partitionInfoWrite.numFiles*sizeof(FileInfoWrite));

				u32 fileOffset = 0;
				for(u32 j=0; j<partitionInfoWrite.numFiles; ++j)
				{
					std::string filePath = fileWriteInfos[j].path;
					FileInfo info = { fileOffset, fileWriteInfos[j].size, (u8)i };
					SHOOT_ASSERT(m_FileMap.find(filePath) == m_FileMap.end(), "Can't have multiple files with the same path '%s'", filePath.c_str());
					m_FileMap[filePath] = info;
					fileOffset += info.size;
				}
			}
		}

		pFile->Close();
		delete pFile;
	}

	//! loads a partition
	void FileSystem::LoadPartition(u32 index)
	{
		File* pFile = File::CreateNative(GetDataPath(), File::M_ReadBinary);
		pFile->Open();
		pFile->SetOffset(m_aPartitions[index].offset, File::OT_Start);
		u8* compressedData = snew u8[m_aPartitions[index].size];
		pFile->Read(compressedData, m_aPartitions[index].size);
		pFile->Close();
		delete pFile;

		// decompress
		u8* uncompressedData = snew u8[m_aPartitions[index].dataSize];
		unsigned long uncompressedDataSizeL = m_aPartitions[index].dataSize;
		s32 result = uncompress(uncompressedData, &uncompressedDataSizeL, compressedData, m_aPartitions[index].size);
		SHOOT_ASSERT(result == Z_OK, "zlib uncompress failed");
		SHOOT_ASSERT(m_aPartitions[index].dataSize == uncompressedDataSizeL, "unexpected uncompressedDataSize");

		m_aPartitions[index].pData = uncompressedData;
		sdelete_array(compressedData);
	}

	//! unloads a partition
	void FileSystem::UnloadPartition(const std::string& strName)
	{
		for(u32 i=0; i<m_aPartitions.size(); ++i)
		{
			if(m_aPartitions[i].name == strName)
			{
				UnloadPartition(i);
				return;
			}
		}

		SHOOT_WARNING(false, "Unknown partition: %s", strName.c_str());
	}

	//! unloads a partition
	void FileSystem::UnloadPartition(u32 index)
	{
		if(m_aPartitions[index].pData)
		{
			Log.Print("UNLOADING PARTITION %s..\n", m_aPartitions[index].name.c_str());
			sdelete_array(m_aPartitions[index].pData);
		}
	}

	//! unloads all partitions
	void FileSystem::UnloadAllPartitions(const char* strToExclude /*= NULL*/)
	{
		for(u32 i=0; i<m_aPartitions.size(); ++i)
		{
			if(strToExclude && m_aPartitions[i].name == strToExclude)
			{
				 continue;
			}

			UnloadPartition(i);
		}
	}

	//! returns true if a file exists
	bool FileSystem::Exists(const char* strPath) const
	{
		bool bExists = (m_FileMap.find(std::string(strPath)) != m_FileMap.end());
		return bExists;
	}

	//! returns the data path
	const char* FileSystem::GetDataPath() const
	{
#ifdef DX11
		return "data_dx11.fs";
#else
		return "data.fs";
#endif
	}
}
