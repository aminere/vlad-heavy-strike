/* 

Amine Rehioui
Created: September 25th 2011

*/

#ifndef _STARFIELD_H_INCLUDED_
#define _STARFIELD_H_INCLUDED_

#include "MemoryPool.h"

namespace shoot
{
	//! Starfield generator
	class Starfield : public Entity3D				    			   
	{
	public:

		//! Macro to provide type information
		DECLARE_OBJECT(Starfield, Entity3D);

		//! constructor
		Starfield();

		//! serializes the entity to/from a PropertyStream
		virtual void Serialize(PropertyStream& stream);

		//! called during the initialization of the entity
		virtual void Init();

		//! called during the update of the entity		
		virtual void Update();
			
	private:
		
		Reference<MemoryPool> m_Pool;

		std::list<Vector3*> m_lParticles;
		std::list<Vector3*> m_lDeadParticles;

		//! init the particles
		void InitParticles();

		// properties
		u32 m_MaxParticles;	
		f32 m_fRadius;
		f32 m_fFadeInDist;
		f32 m_fParticleSize;
		f32 m_fSpeed;
	};
}

#endif // _STARFIELD_H_INCLUDED_

