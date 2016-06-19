/* 

Amine Rehioui
Created: April 28th 2012

*/

#include "Shoot.h"

#include "FileSystemGenerator.h"
#include "FileSystem.h"
#include "File.h"
#include "zlib.h"
#include "PNGLoader.h"
#include "XMeshLoader.h"
#include "FBXMeshLoader.h"

#ifdef DX11
#include "ShaderDX11.h"
#include <d3dcompiler.h>
#endif

namespace shoot
{
	//! generates the file system
	void FileSystemGenerator::Generate()
	{		
		const char* strPath = "../data.fs";

#ifdef DX11
		 strPath = "../data_dx11.fs";
#endif

		File* pFile = File::CreateNative(strPath, File::M_WriteBinary);
		pFile->Open();

		std::vector<PartitionInfo> aPartitions;
		ReadPartitions("data", aPartitions);

		BuildAtlases(aPartitions);

		// FS meta data
		u32 numPartitions = aPartitions.size();
		pFile->Write(&numPartitions, sizeof(u32));

		// determine partition start offset
		u32 partitionStartOffset = sizeof(u32) + numPartitions*sizeof(FileSystem::PartitionInfoWrite);
		for(u32 i=0; i<numPartitions; ++i)
		{
			u32 numFiles = aPartitions[i].aFiles.size();
			partitionStartOffset += numFiles*sizeof(FileSystem::FileInfoWrite);
		}

		u32 partitionOffset = partitionStartOffset;
		std::vector<u8*> aCompressedData;
		std::vector<u32> aCompressedDataSize;
		for(u32 i=0; i<numPartitions; ++i)
		{
			u32 numFiles = aPartitions[i].aFiles.size();
			u32 compressedDataSize = 0;
			u8* compressedData = NULL;
			u32 uncompressedDataSize = 0;
			FileSystem::FileInfoWrite* fileWriteInfo = NULL;

			if(numFiles > 0)
			{
				// get file info
				fileWriteInfo = snew FileSystem::FileInfoWrite[numFiles];				
				for(u32 j=0; j<numFiles; ++j)
				{
					std::string filePath = aPartitions[i].aFiles[j].c_str();					
					u32 fileSize = GetShootFSFileSize(filePath.c_str());
					Log << "Processing " << filePath.c_str() << " ..\n";
					uncompressedDataSize += fileSize;

					// copy file info
					fileWriteInfo[j].size = fileSize;
					u32 pathLength = filePath.length();
					if(pathLength+1 > FileSystem::MaxFilePathLength)
					{
						//SHOOT_WARNING(false, "Truncating path %s", filePath.c_str());
						pathLength = FileSystem::MaxFilePathLength-1;
					}
					memcpy(fileWriteInfo[j].path, filePath.c_str(), pathLength);
					fileWriteInfo[j].path[pathLength] = '\0';
				}

				// prepare uncompressed partition data
				u8* uncompressedData = snew u8[uncompressedDataSize];
				u32 fileOffset = 0;
				for(u32 j=0; j<numFiles; ++j)
				{
					std::string filePath = aPartitions[i].aFiles[j].c_str();
					GetShootFSFileData(filePath.c_str(), uncompressedData+fileOffset);
					Log << "Copying " << filePath.c_str() << " ..\n";
					fileOffset += fileWriteInfo[j].size;
				}

				// compress partition data
				unsigned long compressedDataSizeL = unsigned long(uncompressedDataSize * 1.1) + 12;				
				compressedData = snew u8[compressedDataSizeL];
				s32 result = compress(compressedData, &compressedDataSizeL, uncompressedData, uncompressedDataSize);
				SHOOT_ASSERT(result == Z_OK, "zlib compress failed");
				compressedDataSize = u32(compressedDataSizeL);
				sdelete_array(uncompressedData);
			}

			aCompressedData.push_back(compressedData);
			aCompressedDataSize.push_back(compressedDataSize);

			// write partition info
			FileSystem::PartitionInfoWrite partitionInfo;
			partitionInfo.offset = partitionOffset;
			partitionInfo.size = compressedDataSize;
			partitionInfo.uncompressedSize = uncompressedDataSize;
			partitionInfo.numFiles = numFiles;
			u32 partitionNameLength = aPartitions[i].name.length();
			SHOOT_ASSERT(partitionNameLength < FileSystem::MaxPartitionNameLength, "Partition name too long (Max %d)", FileSystem::MaxPartitionNameLength);
			memcpy(partitionInfo.name, aPartitions[i].name.c_str(), partitionNameLength);
			partitionInfo.name[partitionNameLength] = '\0';
			pFile->Write(&partitionInfo, sizeof(FileSystem::PartitionInfoWrite));
			if(numFiles > 0)
			{
				pFile->Write(fileWriteInfo, numFiles*sizeof(FileSystem::FileInfoWrite));
				sdelete_array(fileWriteInfo);
			}			

			partitionOffset += compressedDataSize;
		}

		// write actual data		
		for(u32 i=0; i<numPartitions; ++i)
		{			
			if(aCompressedDataSize[i] > 0)
			{
				pFile->Write(aCompressedData[i], aCompressedDataSize[i]);
			}
			sdelete_array(aCompressedData[i]);
		}		

		pFile->Close();
		delete pFile;
	}

	//! reads the partitions
	void FileSystemGenerator::ReadPartitions(const std::string& strDirectory, std::vector<PartitionInfo>& aPartitions, s32 currentPartition /*= -1*/)
	{
		WIN32_FIND_DATA findData;
		std::string filter = strDirectory + "/" + "*.*";
		HANDLE hFind = FindFirstFile(filter.c_str(), &findData);
		static std::map<std::string, u32> fileDuplicateChecker;

		if(hFind != INVALID_HANDLE_VALUE)
		{
			s32 currentDirectory = 0;
			do
			{
				if(std::string(findData.cFileName) != "."
				&& std::string(findData.cFileName) != ".."
				&& std::string(findData.cFileName) != ".svn"
				&& std::string(findData.cFileName) != "Thumbs.db")
				{
					s32 partition = currentPartition == -1 ? currentDirectory : currentPartition;
					if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						if(currentPartition == -1)
						{
							PartitionInfo info;
							info.name = findData.cFileName;
							aPartitions.push_back(info);
							m_aAtlasFiles.push_back(std::vector<std::string>());
						}
						std::string directory = strDirectory + "/" + findData.cFileName;
						ReadPartitions(directory, aPartitions, partition);
						currentDirectory++;
					}
					else if(currentPartition != -1)
					{
						std::string filePath = strDirectory + "/" + findData.cFileName;
						s32 extIndex = filePath.rfind(".");
						std::string extension = filePath.substr(extIndex+1);
						if(extension == "bak")
						{
							continue;
						}
						//if(extension == "png")
						//{
						//	m_aAtlasFiles[partition].push_back(filePath);
						//	continue;
						//}

						std::string dataFolder("data");
						size_t dataKeywordIndex = filePath.find(dataFolder);
						SHOOT_ASSERT(dataKeywordIndex != std::string::npos, "File not in data folder");
						filePath = filePath.substr(dataKeywordIndex+5);
						std::transform(filePath.begin(), filePath.end(), filePath.begin(), tolower);
						SHOOT_ASSERT(fileDuplicateChecker.find(filePath) == fileDuplicateChecker.end(), "Duplicate file path detected");
						aPartitions[partition].aFiles.push_back(filePath);
						fileDuplicateChecker[filePath] = 0;
					}
				}
			} while(FindNextFile(hFind, &findData) != 0);
			FindClose(hFind);
		}		
	}

	//! returns the ShootFS file size
	u32 FileSystemGenerator::GetShootFSFileSize(const char* strPath) const
	{
		std::string path(strPath);
		s32 extIndex = path.rfind(".");
		SHOOT_ASSERT(extIndex >= 0, "invalid path");
		std::string extension = path.substr(extIndex+1);

		if(stricmp(extension.c_str(), "png") == 0)
		{
			s32 width = 0, height = 0, channels = 0;
			u32 bpp = 0;

			void* pData = PNGLoader::Load(strPath, width, height, channels, bpp);
			u32 fileSize = sizeof(s32)*3 + sizeof(u32) + width*height*bpp;

			sdelete_array(pData);
			return fileSize;
		}
		else if(stricmp(extension.c_str(), "x") == 0 || stricmp(extension.c_str(), "fbx") == 0)
		{
			MeshResource* pMesh = snew MeshResource();
			if(stricmp(extension.c_str(), "fbx") == 0)
			{
				FBXMeshLoader(pMesh).Load(strPath);
			}
			else
			{
				XMeshLoader(pMesh).Load(strPath);
			}
			std::vector<MeshResource::SubMesh>& aSubMeshes = pMesh->GetSubMeshes();
			const std::vector<Material::CreationInfo>& materials = pMesh->GetMaterialInfos();
			u32 numSubMeshes = aSubMeshes.size();
			u32 fileSize = sizeof(u32); // numSubMeshes
			for(u32 i=0; i<numSubMeshes; ++i)
			{
				VertexBuffer* pVB = aSubMeshes[i].m_VertexBuffer;
				Material::CreationInfo material = materials[i];

				fileSize += sizeof(Vector3); // position
				fileSize += sizeof(u32)*3; // numVertices, numIndices, VertexFlags

				fileSize += sizeof(u32); // nameLength
				fileSize += pVB->GetName().length();

				fileSize += pVB->GetNumIndices()*sizeof(u16);
				fileSize += pVB->GetNumVertices()*sizeof(Vertex3D);

				fileSize += sizeof(Color);
				fileSize += sizeof(u32); // texturePathLength
				if(material.m_TextureInfos.GetSize())
				{
					fileSize += material.m_TextureInfos[0].m_Path.length();
					fileSize += sizeof(bool);
				}
			}
			sdelete(pMesh);
			return fileSize;
		}
#ifdef DX11
		else if(stricmp(extension.c_str(), "hlsl") == 0)
		{
			bool bVS = path.find("vs") != std::string::npos;
			bool bPS = path.find("ps") != std::string::npos;
			SHOOT_ASSERT(bVS || bPS, "Couldn't determine type of HLSL file: %s", strPath);
			const char* strEntryPoint = bVS ? "VSMain" : "PSMain";
			const char* strLevel = bVS ? "vs_4_0_level_9_3" : "ps_4_0_level_9_3";
			COMReference<ID3DBlob> blob = ShaderDX11::CompileHLSL(strPath, strEntryPoint, strLevel);
			u32 fileSize = sizeof(s32); // data size
			fileSize += blob->GetBufferSize();
			return fileSize;
		}
#endif
		else
		{
			File* pFile = File::CreateNative(strPath, File::M_ReadBinary);
			pFile->Open();
			pFile->SetOffset(0, File::OT_End);
			u32 fileSize = pFile->GetOffset();
			pFile->Close();
			delete pFile;
			return fileSize;
		}
	}

	//! get ShootFS file data
	void FileSystemGenerator::GetShootFSFileData(const char* strPath, u8* pDataOut) const
	{
		std::string path(strPath);
		s32 extIndex = path.rfind(".");
		SHOOT_ASSERT(extIndex >= 0, "invalid path");
		std::string extension = path.substr(extIndex+1);

		if(stricmp(extension.c_str(), "png") == 0)
		{
			s32 width = 0, height = 0, channels = 0;
			u32 bpp = 0;
			void* pData = PNGLoader::Load(strPath, width, height, channels, bpp);
			memcpy(pDataOut, &width, sizeof(s32));
			memcpy(pDataOut+=sizeof(s32), &height, sizeof(s32));
			memcpy(pDataOut+=sizeof(s32), &channels, sizeof(s32));
			memcpy(pDataOut+=sizeof(s32), &bpp, sizeof(u32));
			memcpy(pDataOut+=sizeof(u32), pData, width*height*bpp);
			sdelete_array(pData);
		}
		else if(stricmp(extension.c_str(), "x") == 0 || stricmp(extension.c_str(), "fbx") == 0)
		{
			MeshResource* pMesh = snew MeshResource();
			if(stricmp(extension.c_str(), "fbx") == 0)
			{
				FBXMeshLoader(pMesh).Load(strPath);
			}
			else
			{
				XMeshLoader(pMesh).Load(strPath);
			}
			std::vector<MeshResource::SubMesh>& aSubMeshes = pMesh->GetSubMeshes();
			const std::vector<Material::CreationInfo>& materials = pMesh->GetMaterialInfos();
			u32 numSubMeshes = aSubMeshes.size();
			memcpy(pDataOut, &numSubMeshes, sizeof(u32));
			pDataOut += sizeof(u32);
			for(u32 i=0; i<numSubMeshes; ++i)
			{
				VertexBuffer* pVB = aSubMeshes[i].m_VertexBuffer;
				Material::CreationInfo material = materials[i];

				memcpy(pDataOut, aSubMeshes[i].m_vPosition.v, sizeof(Vector3));
				u32 numVertices = pVB->GetNumVertices();
				u32 numIndices = pVB->GetNumIndices();
				u32 flags = pVB->GetVertexFlags();
				memcpy(pDataOut+=(sizeof(Vector3)), &numVertices, sizeof(u32));
				memcpy(pDataOut+=sizeof(u32), &numIndices, sizeof(u32));
				memcpy(pDataOut+=sizeof(u32), &flags, sizeof(u32));

				u32 nameLength = pVB->GetName().length();
				memcpy(pDataOut+=sizeof(u32), &nameLength, sizeof(u32));
				memcpy(pDataOut+=sizeof(u32), pVB->GetName().c_str(), nameLength);

				memcpy(pDataOut+=nameLength, pVB->GetIndices(), numIndices*sizeof(u16));
				memcpy(pDataOut+=(numIndices*sizeof(u16)), pVB->GetVertices(), numVertices*sizeof(Vertex3D));
				
				memcpy(pDataOut+=(numVertices*sizeof(Vertex3D)), &material.m_Color, sizeof(Color));
				if(material.m_TextureInfos.GetSize())
				{
					u32 texturePathLength = material.m_TextureInfos[0].m_Path.length();
					memcpy(pDataOut+=sizeof(Color), &texturePathLength, sizeof(u32));
					memcpy(pDataOut+=sizeof(u32), material.m_TextureInfos[0].m_Path.c_str(), texturePathLength);
					memcpy(pDataOut+=texturePathLength, &material.m_TextureInfos[0].m_bMipMaps, sizeof(bool));
					pDataOut += sizeof(bool);
				}
				else
				{
					memset(pDataOut+=sizeof(Color), 0, sizeof(u32));
					pDataOut += sizeof(u32);
				}
			}
			sdelete(pMesh);
		}
#ifdef DX11
		else if(stricmp(extension.c_str(), "hlsl") == 0)
		{
			bool bVS = path.find("vs") != std::string::npos;
			bool bPS = path.find("ps") != std::string::npos;
			SHOOT_ASSERT(bVS || bPS, "Couldn't determine type of HLSL file: %s", strPath);
			const char* strEntryPoint = bVS ? "VSMain" : "PSMain";
			const char* strLevel = bVS ? "vs_4_0_level_9_3" : "ps_4_0_level_9_3";
			COMReference<ID3DBlob> blob = ShaderDX11::CompileHLSL(strPath, strEntryPoint, strLevel);
			s32 size = blob->GetBufferSize();
			memcpy(pDataOut, &size, sizeof(s32));
			memcpy(pDataOut+=sizeof(s32), blob->GetBufferPointer(), size);
		}
#endif
		else
		{
			File* pFile = File::CreateNative(strPath, File::M_ReadBinary);
			pFile->Open();
			pFile->SetOffset(0, File::OT_End);
			u32 fileSize = pFile->GetOffset();
			pFile->SetOffset(0, File::OT_Start);
			pFile->Read(pDataOut, fileSize);
			pFile->Close();
			delete pFile;
		}
	}

	//! build the atlases
	void FileSystemGenerator::BuildAtlases(std::vector<PartitionInfo>& aPartitions)
	{
		SHOOT_ASSERT(m_aAtlasFiles.size() == aPartitions.size(), "Atlas build error");
		for(u32 i=0; i<m_aAtlasFiles.size(); ++i)
		{
			for(u32 j=0; j<m_aAtlasFiles[i].size(); ++j)
			{
				// TODO
			}
		}
	}
}
