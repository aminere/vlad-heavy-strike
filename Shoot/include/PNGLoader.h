/* 

Amine Rehioui
Created: April 22nd 2012

*/

#ifndef _PNG_LOADER_H_INCLUDED_
#define _PNG_LOADER_H_INCLUDED_

namespace shoot
{
	//! PNG loader
	class PNGLoader
	{
	public:
		
		//! loads a texture
		static void* Load(const char* strPath, s32& width, s32& height, s32& channels, u32& bpp);

	private:

		//! loads a texture from pre-processed file system
		static void* LoadFS(const char* strPath, s32& width, s32& height, s32& channels, u32& bpp);		
	};
}

#endif // _PNG_LOADER_H_INCLUDED_

