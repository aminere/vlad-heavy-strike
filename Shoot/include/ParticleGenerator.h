/* 

Amine Rehioui
Created: May 30th 2010

*/

#ifndef _PARTICLE_GENERATOR_H_INCLUDED_
#define _PARTICLE_GENERATOR_H_INCLUDED_

#include "Range.h"

#include "AABBox3D.h"
#include "MemoryPool.h"

namespace shoot
{
	//! 3D Particle Generator - only box volume for now and with simplistic implementation
	class ParticleGenerator : public Entity3D				    			   
	{
	public:

		//! Macro to provide type information
		DECLARE_OBJECT(ParticleGenerator, Entity3D);

		//! constructor
		ParticleGenerator();

		//! serializes the entity to/from a PropertyStream
		virtual void Serialize(PropertyStream& stream);

		//! called during the initialization of the entity
		virtual void Init();

		//! called during the update of the entity		
		virtual void Update();

		//! returns the affine transformation matrix
		virtual Matrix44 GetTransformationMatrix();
			
		//! Get the local axis-aligned bounding box
		AABBox3D GetBoundingBox() const;

		//! returns the emit box
		const AABBox3D& GetEmitBox() const { return m_EmitBox; }

		//! returns world transform status
		inline bool GetApplyWorldTransform() const { return m_bApplyWorldTransform; }

		//! sets the active status
		inline void SetActive(bool bActive) { m_bActive = bActive; }

	protected:

		//! particle info
		struct Particle
		{
			Vector3 Position;
			Vector3 Speed;
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
		Vector3 m_vDirection;		
		Vector3 m_vGravity;
		AABBox3D m_EmitBox;
		AABBox3D m_BoundingBox;
		u32 m_PoolSize;
		f32 m_fActiveTime;
		bool m_bExplosive;
		bool m_bApplyWorldTransform;
	};
}

#endif // _PARTICLE_GENERATOR_H_INCLUDED_

