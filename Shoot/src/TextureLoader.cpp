/* 

Amine Rehioui
Created: April 21st 2012

*/

#include "Shoot.h"

#include "TextureLoader.h"

#include "PNGLoader.h"

namespace shoot
{
	//! loads a texture
	void* TextureLoader::Load(const char* strPath, s32& width, s32& height, s32& channels, u32& bpp)
	{
		std::string path(strPath);
		s32 extIndex = path.rfind(".");		
		std::string extension = path.substr(extIndex+1);

		if(extension == "png")
		{
			return PNGLoader::Load(strPath, width, height, channels, bpp);
		}
		
		return 0;
	}
}
