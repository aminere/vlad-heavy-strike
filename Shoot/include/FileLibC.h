/* 

Amine Rehioui
Created: August 5th 2010

*/

#ifndef _FILE_LIBC_H_INCLUDED_
#define _FILE_LIBC_H_INCLUDED_

#include "File.h"

#include <stdio.h>

namespace shoot
{
	//! File class with default LibC implementation
	class FileLibC : public File
	{
	public:

		//! constructor
		FileLibC(const char* strPath, E_Mode eMode);	

		//! opens the file
		bool Open(bool bAssertOnFailure = true);

		//! closes the file
		void Close();

		//! reads data from the file
		s32 Read(void* pDest, u32 bytesToRead);

		//! writes data to the file
		s32 Write(const void* pSrc, u32 bytesToWrite);
	
		//! changes the current read/write location in the file
		void SetOffset(s32 offset, E_OffsetType eType = OT_Current);

		//! returns the current read/write location in the file
		u32 GetOffset();

		//! returns true if end of file has been reached
		bool EOFReached();

		// printing operators
		File& operator << (const char* param);
		File& operator << (s32 param);
		File& operator << (f32 param);
		File& operator << (char param);

	private:

		FILE* m_pFile;			
	};
}

#endif // _FILE_LIBC_H_INCLUDED_

