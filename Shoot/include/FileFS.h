/* 

Amine Rehioui
Created: April 25th 2012

*/

#ifndef _FILE_FS_H_INCLUDED_
#define _FILE_FS_H_INCLUDED_

#include "File.h"

namespace shoot
{
	//! File that accesses the shoot file system
	class FileFS : public File
	{
	public:

		//! constructor
		FileFS(const char* strPath);	

		//! opens the file
		bool Open(bool bAssertOnFailure = true);

		//! closes the file
		void Close() { }

		//! reads data from the file
		s32 Read(void* pDest, u32 bytesToRead);

		//! writes data to the file
		s32 Write(const void* pSrc, u32 bytesToWrite) { return 0; }
	
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

		s32 m_CurrentOffset;
		u8* m_pData;
		u32 m_Size;
	};
}

#endif // _FILE_FS_H_INCLUDED_

