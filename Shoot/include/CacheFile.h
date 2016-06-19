/* 

Amine Rehioui
Created: May 31st 2013

*/

#ifndef _CACHE_FILE_H_INCLUDED_
#define _CACHE_FILE_H_INCLUDED_

#include "File.h"

namespace shoot
{
	//! CacheFile class
	class CacheFile : public File
	{
	public:

		//! File Info
		struct Info
		{
			Info()
				: m_CurrentOffset(0)
				, m_pData(NULL)
				, m_Size(0)
			{
			}

			s32 m_CurrentOffset;
			u8* m_pData;
			u32 m_Size;
		};

		//! clears the cache
		static void Clear();

		//! clears a file from the cache
		static void Clear(const std::string& strPath);

		//! returns an instance
		static CacheFile* GetFile(const std::string& strPath);

		//! constructor
		CacheFile(const char* strPath, const Info& info);

		//! opens the file
		bool Open(bool bAssertOnFailure = true);

		//! closes the file
		void Close() { }

		//! reads data from the file
		s32 Read(void* pDest, u32 bytesToRead);

		//! reads a string
		s32 Read(std::string& dest, char delim1 = 0, char delim2 = 0);

		//! reads a line until a delimiter
		s32 ReadLine(char* pDest, u32 numChars, char delim = '\n');

		//! writes data to the file
		s32 Write(const void* pSrc, u32 bytesToWrite) { return 0; }

		//! peeks a character without advancing the file pointer
		char Peek();

		//! ignores n characters or x characters until the delimiter is found
		void Ignore(u32 numChars, char delimiter = ' ');
	
		//! changes the current read/write location in the file
		void SetOffset(s32 offset, E_OffsetType eType = OT_Current);

		//! returns the current read/write location in the file
		u32 GetOffset();

		//! returns true if end of file has been reached
		bool EOFReached();

		// printing operators
		File& operator << (const char* param) { return *this; }
		File& operator << (s32 param) { return *this; }
		File& operator << (f32 param) { return *this; }
		File& operator << (char param) { return *this; }

	private:

		Info m_Info;
		static std::map<std::string, Info> m_sCache;
	};
}

#endif // _CACHE_FILE_H_INCLUDED_

