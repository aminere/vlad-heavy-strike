/* 

Amine Rehioui
Created: November 26th 2011

*/

#ifndef _ANIMATED_MATERIAL_H_INCLUDED_
#define _ANIMATED_MATERIAL_H_INCLUDED_

#include "Material.h"

namespace shoot
{
	//! Animated Material
	class AnimatedMaterial : public Material
	{
	public:

		DECLARE_OBJECT(AnimatedMaterial, Material);

		//! constructor
		AnimatedMaterial();

		//! destructor
		~AnimatedMaterial();

		//! Reads/Writes struct properties from/to a stream
		virtual void Serialize(PropertyStream& stream);

		//! returns a texture
		virtual Texture* GetTexture(u32 index = 0);

		//! updates the material
		virtual void Update();
						
	private:

		s32 m_CurrentFrame;
		f32 m_fCurrentFrameDuration;
		f32 m_fFrameDeltaTime;
		s32 m_Direction;

		// properties
		f32 m_framesPerSecond;
		bool m_bToggle;
	};
}

#endif // _ANIMATED_MATERIAL_H_INCLUDED_

