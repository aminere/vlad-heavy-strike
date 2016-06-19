/* 

Amine Rehioui
Created: May 31st 2013

*/

#include "Shoot.h"

#include "CacheFile.h"



namespace shoot
{
	//! statics
	std::map<std::string, CacheFile::Info> CacheFile::m_sCache;

	//! clears the cache
	void CacheFile::Clear()
	{
		for(std::map<std::string, CacheFile::Info>::iterator it = m_sCache.begin();
			it != m_sCache.end();
			++ it)
		{
			sdelete_array((it->second).m_pData);
		}

		m_sCache.clear();
	}

	//! clears a file from the cache
	void CacheFile::Clear(const std::string& strPath)
	{
		std::map<std::string, CacheFile::Info>::iterator it = m_sCache.find(strPath);
		if(it != m_sCache.end())
		{
			sdelete_array((it->second).m_pData);
			m_sCache.erase(it);
		}
	}

	//! returns an instance
	CacheFile* CacheFile::GetFile(const std::string& strPath)
	{
		std::map<std::string, CacheFile::Info>::iterator it = m_sCache.find(strPath);
		if(it != m_sCache.end())
		{
			return snew CacheFile(strPath.c_str(), it->second);
		}

		Info info;
		File* pNativeFile = File::CreateNative(strPath.c_str(), File::M_ReadBinary);
		pNativeFile->Open();
		pNativeFile->SetOffset(0, File::OT_End);	
		info.m_Size = pNativeFile->GetOffset();
		info.m_pData = snew u8[info.m_Size];
		pNativeFile->SetOffset(0, File::OT_Start);
		pNativeFile->Read(info.m_pData, info.m_Size);		
		pNativeFile->Close();
		delete pNativeFile;

		CacheFile* pFile = snew CacheFile(strPath.c_str(), info);
		m_sCache[strPath] = info;
		return pFile;
	}

	//! constructor
	CacheFile::CacheFile(const char* strPath, const Info& info) 
		: File(strPath, M_ReadBinary)
		, m_Info(info)
	{
	}

	//! opens the file
	bool CacheFile::Open(bool bAssertOnFailure /*= true*/)
	{
		m_Info.m_CurrentOffset = 0;
		return true;
	}

	//! reads data from the file
	s32 CacheFile::Read(void* pDest, u32 bytesToRead)
	{
		SHOOT_ASSERT(m_Info.m_CurrentOffset+bytesToRead <= m_Info.m_Size, "EOF reached");
		u8* pData = m_Info.m_pData + m_Info.m_CurrentOffset;
		memcpy(pDest, pData, bytesToRead);
		m_Info.m_CurrentOffset += bytesToRead;
		return bytesToRead;
	}

	//! reads a string
	s32 CacheFile::Read(std::string& dest, char delim1 /*= 0*/, char delim2 /*= 0*/)
	{
		char c;
		while(Read(&c, 1))
		{
			if(c != '\r' 
			&& c != '\n'
			&& c != ' '
			&& c != '\t'
			&& ((delim1 == 0) || (c != delim1))
			&& ((delim2 == 0) || (c != delim2)))
			{
				dest += c;				
			}
			else
			{
				SetOffset(-1);
				break;
			}
		}
		return dest.length();
	}

	//! reads a line until a delimiter
	s32 CacheFile::ReadLine(char* pDest, u32 numChars, char delim /*= '\n'*/)
	{
		u32 i;
		for(i=0; i<numChars; ++i)
		{
			char c;
			if(Read(&c, 1))
			{		
				if(c != delim)
				{
					pDest[i] = c;					
				}
				else
				{					
					break;
				}
			}
			else
			{
				break;
			}
		}
		pDest[i] = '\0';
		return i;
	}

	//! peeks a character without advancing the file pointer
	char CacheFile::Peek()
	{
		char c;
		Read(&c, 1);
		SetOffset(-1);
		return c;
	}

	//! ignores n characters or x characters until the delimiter is found
	void CacheFile::Ignore(u32 numChars, char delimiter /*= ' '*/)
	{
		for(u32 i=0; i<numChars; ++i)
		{
			char c;
			Read(&c, 1);
			if(c == delimiter)
			{
				break;
			}
		}
	}

	//! changes the current read/write location in the file
	void CacheFile::SetOffset(s32 offset, E_OffsetType eType /*= OT_Current*/)
	{
		switch(eType)
		{
		case OT_Current:
			m_Info.m_CurrentOffset += offset;
			break;

		case OT_Start:
			m_Info.m_CurrentOffset = offset;
			break;

		case OT_End:
			m_Info.m_CurrentOffset = offset+m_Info.m_Size;
			break;
		}
		SHOOT_ASSERT(m_Info.m_CurrentOffset >= 0 && m_Info.m_CurrentOffset <= s32(m_Info.m_Size), "Invalid file offset");
	}

	//! returns the current read/write location in the file
	u32 CacheFile::GetOffset()
	{
		return u32(m_Info.m_CurrentOffset);
	}

	//! returns true if end of file has been reached
	bool CacheFile::EOFReached()
	{
		return (m_Info.m_CurrentOffset >= s32(m_Info.m_Size));
	}
}

