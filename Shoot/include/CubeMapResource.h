/* 

Amine Rehioui
Created: April 2nd 2012

*/

#ifndef _CUBEMAP_RESOURCE_H_INCLUDED_
#define _CUBEMAP_RESOURCE_H_INCLUDED_

#include "Resource.h"
#include "Texture.h"

namespace shoot
{
	// forwards
	class Texture;

	//! CubeMap Resource class
	class CubeMapResource : public Resource
	{
		DECLARE_OBJECT(CubeMapResource, Resource);

	public:

		//! serializes the object to/from a PropertyStream
		void Serialize(PropertyStream& stream);

		//! Load from disk
		void ResourceInit() { m_Texture->ResourceInit(); }

		//! Unload disk data
		void ResourceUninit() { m_Texture->ResourceUninit(); }

		//! returns the cube map texture
		Texture* GetTexture() { return m_Texture.Get(); }

		//! returns true if this resource is using the specified data
		virtual bool IsUsingData(const char* strPath) const { return m_Texture->IsUsingData(strPath); }

	protected:

		Reference<Texture> m_Texture;

		// properties
		Array< std::string > m_aTextures;		
	};
}

#endif // _CUBEMAP_RESOURCE_H_INCLUDED_

