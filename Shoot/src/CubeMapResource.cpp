/* 

Amine Rehioui
Created: April 2nd 2012

*/

#include "Shoot.h"

#include "CubeMapResource.h"

namespace shoot
{
	DEFINE_OBJECT(CubeMapResource);

	//! serializes the object to/from a PropertyStream
	void CubeMapResource::Serialize(PropertyStream& stream)
	{
		stream.SerializeArray("Textures", &m_aTextures, PT_File);

		if(stream.GetMode() == SM_Read)
		{
			SHOOT_ASSERT(m_aTextures.GetSize() == 6, "Invalid CubeMap Resource");
			const char* aPaths[6] =
			{
				m_aTextures[0].c_str(),
				m_aTextures[1].c_str(),
				m_aTextures[2].c_str(),
				m_aTextures[3].c_str(),
				m_aTextures[4].c_str(),
				m_aTextures[5].c_str()
			};
			m_Texture = GraphicsDriver::Instance()->CreateCubeMapTexture(aPaths);
		}
	}
}
