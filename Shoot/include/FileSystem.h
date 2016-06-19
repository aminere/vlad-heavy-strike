/* 

Amine Rehioui
Created: April 25th 2012

*/

#ifndef _FILE_SYSTEM_H_INCLUDED_
#define _FILE_SYSTEM_H_INCLUDED_

namespace shoot
{
	//! FileSystem class
	class FileSystem : public Singleton<FileSystem>
	{
		DECLARE_SINGLETON(FileSystem);

	public:

		static const u32 MaxPartitionNameLength = 16;
		static const u32 MaxFilePathLength = 40;
		static const u32 MaxFilesPerPartition = 1000;
		static const u32 BootPartitionIndex = 0;
		static const u32 CommonPartitionIndex = 1;

		//! file info
		struct FileInfo
        {                       
            u32 offset; // offset in uncompressed data in memory
            u32 size; // uncompressed size in memory
            u8 partition;
        };      
  
		//! partition info
        struct PartitionInfo
        {
            u32 offset; // offset in compressed data on disk
            u32 size; // compressed size on disk
			u8* pData; // uncompressed data in memory
            u32 dataSize; // uncompressed data in memory
			std::string name; // name of the partition
        };

		//! file info to write
        struct FileInfoWrite
        {
            u32 size; // uncompressed size in memory
            char path[MaxFilePathLength];
        };

		//! partition info to write
		struct PartitionInfoWrite
        {           
            u32 offset; // offset in compressed data on disk
            u32 size; // compressed size on disk
			u32 uncompressedSize; // uncompressed size in memory
            u32 numFiles;
			char name[MaxPartitionNameLength]; // name of the partition
        };
 
		typedef std::map< std::string, FileInfo > FileMap;

		//! initializes the file system
		void Init();

		//! loads a partition
		void LoadPartition(u32 index);

		//! unloads a partition
		void UnloadPartition(const std::string& strName);

		//! unloads all partitions
		void UnloadAllPartitions(const char* strToExclude = NULL);

		//! returns the file map
		FileMap& GetFileMap() { return m_FileMap; }

		//! returns the partitions
		std::vector<PartitionInfo>& GetPartitions() { return m_aPartitions; }

		//! returns true if a file exists
		bool Exists(const char* strPath) const;

	private:

		//! unloads a partition
		void UnloadPartition(u32 index);

		//! returns the data path
		const char* GetDataPath() const;

		FileMap m_FileMap;
		std::vector<PartitionInfo> m_aPartitions;
	};
}

#endif // _FILE_SYSTEM_H_INCLUDED_


