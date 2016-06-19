/* 

Amine Rehioui
Created: July 11th 2013

*/

#ifndef _FILE_APK_H_INCLUDED_
#define _FILE_APK_H_INCLUDED_

#include "File.h"

#include <android/asset_manager.h>

namespace shoot
{
	//! File class to read from Android APK
	class FileAPK : public File
	{
	public:

		//! constructor
		FileAPK(const char* strPath);

		//! opens the file
		bool Open(bool bAssertOnFailure = true);

		//! closes the file
		void Close();

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

		//! sets the asset manager
		inline static void SetAssetManager(AAssetManager* pMgr) { m_spAssetManager = pMgr; }

	private:

		AAsset* m_pFile;
		static AAssetManager* m_spAssetManager;
	};
}

#endif // _FILE_APK_H_INCLUDED_

