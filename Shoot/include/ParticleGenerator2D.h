/* 

Amine Rehioui
Created: May 30th 2010

*/

#ifndef _PARTICLE_GENERATOR_2D_H_INCLUDED_
#define _PARTICLE_GENERATOR_2D_H_INCLUDED_

#include "MemoryPool.h"

namespace shoot
{
	//! 2D Particle Generator - only box volume for now and with simplistic implementation
	class ParticleGenerator2D : public Entity2D			    			   
	{
	public:

		//! Macro to provide type information
		DECLARE_OBJECT(ParticleGenerator2D, Entity2D);

		//! constructor
		ParticleGenerator2D();

		//! serializes the entity to/from a PropertyStream
		virtual void Serialize(PropertyStream& stream);

		//! called during the initialization of the entity
		virtual void Init();

		//! called during the update of the entity		
		virtual void Update();

		//! returns world transform status
		inline bool GetApplyWorldTransform() const { return m_bApplyWorldTransform; }
			
	protected:

		//! particle info
		struct Particle
		{
			Vector2 Position;
			Vector2 Speed;
			f32 Life;
			f32 Alpha;
			f32 InitialLife;
		};

		Reference<MemoryPool> m_Pool;
		std::list<Particle*> m_aParticles;
		f32 m_fNewParticlesCounter;
		bool m_bActive;

		// properties
		u32 m_MaxParticles;
		s32 m_ParticlesPerSecond;
		Range m_rParticleLife;
		Range m_rParticleInitialSpeed;
		f32 m_fParticleSize;
		Vector2 m_vDirection;		
		Vector2 m_vGravity;
		u32 m_PoolSize;
		f32 m_fActiveTime;
		bool m_bExplosive;
		bool m_bApplyWorldTransform;
	};
}

#endif // _PARTICLE_GENERATOR_2D_H_INCLUDED_

