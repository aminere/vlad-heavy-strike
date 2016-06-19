/* 

Amine Rehioui
Created: April 21st 2012

*/

#ifndef _TEXTURE_LOADER_H_INCLUDED_
#define _TEXTURE_LOADER_H_INCLUDED_

namespace shoot
{
	//! texture loader
	class TextureLoader
	{
	public:
		
		//! loads a texture
		static void* Load(const char* strPath, s32& width, s32& height, s32& channels, u32& bpp);
	};
}

#endif // _TEXTURE_LOADER_H_INCLUDED_

