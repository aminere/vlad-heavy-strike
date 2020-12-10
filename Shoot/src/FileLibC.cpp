/* 

Amine Rehioui
Created: August 5th 2010

*/

#include "Shoot.h"

#include "FileLibC.h"

namespace shoot
{
	//! constructor
	FileLibC::FileLibC(const char* strPath, E_Mode eMode) 
		: File(strPath, eMode)
		, m_pFile(NULL)
	{
	}	

	//! opens the file
	bool FileLibC::Open(bool bAssertOnFailure /*= true*/)
	{
		const char* strModes[] = { "rt", "wt", "rb", "wb" };
		
		std::string path;

		if(ms_bUseWritablePath || ms_bUseExternalWritablePath)
		{
			std::string writablePath = ms_bUseWritablePath ? m_strWritablePath : m_strExternalWritablePath;
			if(!writablePath.empty())
			{
				path = writablePath + std::string("/");
			}			
		}
		else if (ms_bUseDataFolder)
		{
			path = "data/";
		}
		else if(!m_strBasePath.empty())
		{
			path = m_strBasePath + std::string("/");
		}		

		path = path + m_strPath;
		m_pFile = fopen(path.c_str(), strModes[m_eMode]);

		if(!m_pFile && bAssertOnFailure)
		{
			SHOOT_WARNING(false, "Could not open file %s\n", path.c_str());
		}
		
		return (m_pFile != NULL);
	}

	//! closes the file
	void FileLibC::Close()
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}

	//! reads data from the file
	s32 FileLibC::Read(void* pDest, u32 bytesToRead)
	{
		return (s32)fread(pDest, 1, bytesToRead, m_pFile);
	}

	//! writes data to the file
	s32 FileLibC::Write(const void* pSrc, u32 bytesToWrite)
	{
		return (s32)fwrite(pSrc, 1, bytesToWrite, m_pFile);
	}

	//! changes the current read/write location in the file
	void FileLibC::SetOffset(s32 offset, E_OffsetType eType /*= OT_Current*/)
	{
		s32 seekModes[] = { SEEK_CUR, SEEK_SET, SEEK_END };
		while(fseek(m_pFile, offset, seekModes[eType]))
		{
			Log << "WARNING: FileLibC::SetOffset: fseek failure\n";
		}
	}

	//! returns the current read/write location in the file
	u32 FileLibC::GetOffset()
	{
		return ftell(m_pFile);
	}

	//! returns true if end of file has been reached
	bool FileLibC::EOFReached()
	{
		return (feof(m_pFile) != 0);
	}

	// printing operators
	File& FileLibC::operator << (const char* param)
	{
		fprintf(m_pFile, "%s", param);
		return *this;
	}

	File& FileLibC::operator << (s32 param)
	{
		fprintf(m_pFile, "%d", param);
		return *this;
	}

	File& FileLibC::operator << (f32 param)
	{
		fprintf(m_pFile, "%f", param);
		return *this;
	}

	File& FileLibC::operator << (char param)
	{
		fprintf(m_pFile, "%c", param);
		return *this;
	}
}

