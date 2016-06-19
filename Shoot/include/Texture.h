/* 

Amine Rehioui
Created: April 21st 2010

*/

#ifndef _TEXTURE_H_INCLUDED_
#define _TEXTURE_H_INCLUDED_

#include "Resource.h"

namespace shoot
{
	//! base Texture class
	class Texture : public Resource, public GraphicObject
	{
		DECLARE_OBJECT(Texture, Resource);

	public:

		//! texture format
		enum E_Format
		{
			TF_RGB,
			TF_RGBA,
			TF_Count,
			TF_Unknown
		};

		//! texture type
		enum E_Type
		{
			T_2D,
			T_RenderTarget,
			T_ShadowMap,
			T_Count,
			T_Invalid
		};

		//! constructor
		Texture()
			: m_bMipMaps(true)
		{
		}

		//! returns the size of the texture
		virtual Vector2 GetSize() const { return Vector2(); }

		//! returns the hardware size of the texture
		virtual Vector2 GetHardwareSize() const { return Vector2(); }

		//! returns the format of the texture
		virtual E_Format GetFormat() const { return TF_Unknown; }

		//! returns the texture type
		virtual E_Type GetType() const { return T_Invalid; }

		//! resize the texture
		virtual void Resize(const Vector2& size) { }

		//! start rendering with this texture
		virtual void Begin(u32 stage) { }

		//! ends rendering with this texture
		virtual void End() { }

		//! sets mip mapping status
		inline void SetHasMipMaps(bool bMipMaps) { m_bMipMaps = bMipMaps; }

		//! gets mip mapping status
		inline bool GetHasMipMaps() const { return m_bMipMaps; }

	protected:

		bool m_bMipMaps;
	};
}

#endif // _TEXTURE_H_INCLUDED_

