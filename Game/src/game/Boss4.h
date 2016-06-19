/* 

Amine Rehioui
Created: August 20th 2013

*/

#ifndef _BOSS4_H_INCLUDED_
#define _BOSS4_H_INCLUDED_

namespace shoot
{
	//! Boss4 class
	class Boss4 : public Actor
	{
		friend class FSMState<Boss4>;

	public:
		
		DECLARE_OBJECT(Boss4, Actor);

		//! constructor
		Boss4();

		//! serializes the entity to/from a PropertyStream
		void Serialize(PropertyStream& stream);

		//! called during the initialization of the entity
		void Init();

		//! called during the update of the entity		
		void Update();

		//! returns the number of laser cannons
		u32 GetNumLaserCannons() const;

		//! returns the number of missile cannons
		u32 GetNumMissileCannons() const;

	// public for FSM
		
		const static u32 m_sNumLaserCannons = 2;
		const static u32 m_sNumMissileCannons = 2;

		Handle<Actor> m_LaserCannons[m_sNumLaserCannons];
		Handle<Actor> m_MissileCannons[m_sNumMissileCannons];
		Handle<Actor> m_Tower;
		bool m_bLaserPhase;

		// properties
		f32 m_fLaserThickness;
		f32 m_fLaserOffset;
		f32 m_fLaserSpeed;
		f32 m_fLaserLength;
		f32 m_fLaserFrequency;
		f32 m_fLaserLife;
		f32 m_fLaserPhaseDelay;
		f32 m_fLaserPhaseDuration;
				
		f32 m_fMissileOffset;
		f32 m_fMissileSpeed;		
		f32 m_fMissileFrequency;
		f32 m_fMissileLife;
		f32 m_fMissileSize;
		f32 m_fMissileMinDist;
		f32 m_fMissileHomingFactor;
		f32 m_fMissileMaxHomingAngle;
		f32 m_fMissilePhaseDelay;
		f32 m_fMissilePhaseDuration;

		bool m_bDestroyWhenDone;
		Reference<Component> m_Retreat;
	};
}

#endif // _BOSS4_H_INCLUDED_


