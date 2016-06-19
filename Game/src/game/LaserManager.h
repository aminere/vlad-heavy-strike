/* 

Amine Rehioui
Created: March 5th 2013

*/

#ifndef _LASER_MANAGER_H_INCLUDED_
#define _LASER_MANAGER_H_INCLUDED_

#include "RenderableEntity.h"

#include "MemoryPool.h"

namespace shoot
{
	// Laser class
	class Laser
	{
	public:

		//! params
		struct Params
		{
			Params()
				: fLife(5.0f)
				, bStatic(false)
				, vStart(Vector3::Zero)				
				, vDir(Vector3::Zero)
				, fSpeed(10.0f)
				, fMaxLength(4.0f)
				, damage(30.0f)
				, fThickness(.5f)
			{
			}

			bool bStatic;
			Vector3 vStart;
			Vector3 vDir;
			f32 fSpeed;
			f32 fLife;
			f32 fThickness;
			f32 fMaxLength;
			f32 damage;
		};

		//! Initialize
		void Init(const Params& params)
		{
			bStatic = params.bStatic;
			vStart = params.vStart;
			vDir = params.vDir;
			fSpeed = params.fSpeed;
			fLife = params.fLife;
			fThickness = params.fThickness;
			fMaxLength = params.fMaxLength;
			fLength = 0.0f;
			damage = params.damage;
		}

		bool bStatic;
		Vector3 vStart;
		Vector3 vDir;
		f32 fSpeed;
		f32 fLife;
		f32 fThickness;
		f32 fMaxLength;
		f32 fLength;
		f32 damage;
	};

	//! LaserManager class
	class LaserManager : public RenderableEntity
	{
		DECLARE_OBJECT(LaserManager, RenderableEntity);

	public:		

		//! Constructor
		LaserManager();

		//! destructor
		virtual ~LaserManager();

		//! serializes the entity to/from a PropertyStream
		virtual void Serialize(PropertyStream& stream);

		//! called during the initialization of the entity
		virtual void Init();

		//! called during the update of the entity		
		virtual void Update();

		//! adds a laser
		Laser* AddLaser(const Laser::Params& params);

		//! clears all lasers
		void Clear();

		//! returns the lasers list
		inline const std::list<Laser*>& GetLasers() const { return m_Lasers; }

	private:

		Reference<MemoryPool> m_Pool;
		std::list<Laser*> m_Lasers;

		// properties
		u32 m_PoolSize;
	};
}

#endif // _LASER_MANAGER_H_INCLUDED_


