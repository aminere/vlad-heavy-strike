/* 

Amine Rehioui
Created: April 20th 2012

*/

#include "Shoot.h"

#include "FakeCubeMapTexture.h"

namespace shoot
{
	//! constructor
	FakeCubeMapTexture::FakeCubeMapTexture(const char* strPath[6])
	{
		for(u32 i=0; i<6; ++i)
		{
			m_Textures[i] = ResourceManager::Instance()->GetResource<Texture>(strPath[i]);
			m_Textures[i]->SetHasMipMaps(false);
		}
	}

	//! constructor
	FakeCubeMapTexture::FakeCubeMapTexture()
	{
		SHOOT_ASSERT(false, "Invalid Constructor");
	}
}

