/* 

Amine Rehioui
Created: August 13th 2013

*/

#ifndef _BOSS3_H_INCLUDED_
#define _BOSS3_H_INCLUDED_

namespace shoot
{
	//! Boss3 class
	class Boss3 : public Actor
	{
		friend class FSMState<Boss3>;

	public:
		
		DECLARE_OBJECT(Boss3, Actor);

		//! constructor
		Boss3();

		//! serializes the entity to/from a PropertyStream
		void Serialize(PropertyStream& stream);

		//! called during the initialization of the entity
		void Init();

		//! called during the update of the entity		
		void Update();

	// public for FSM

		//! updates the radials
		void UpdateRadials();

		f32 m_fRadialTimer;
		bool m_bSweepPhase;
		std::vector< Handle<Actor> > m_Radials;
		Handle<Actor> m_Cannon;
		Handle<Actor> m_Core;
		Handle<Entity3D> m_Lid1;
		Handle<Entity3D> m_Lid2;

		// properties		
		f32 m_fRadialFrequency;
		f32 m_fSweepFrequency;
		f32 m_fCoreShotFrequency;
		f32 m_fAngleRange;
		f32 m_fAngleStep;
		f32 m_fBulletLife;
		f32 m_fBulletSpeed;
		f32 m_fBulletOffset;

		f32 m_fCannonAnimDuration;
		f32 m_fCannonDestOffset;
		f32 m_fLaserThickness;
		f32 m_fLaserOffset;
		f32 m_fLaserSpeed;
		f32 m_fLaserLength;
		f32 m_fLaserFrequency;
		f32 m_fLaserLife;
		f32 m_fLaserAimDuration;
		f32 m_fSweepDuration;

		f32 m_fCoreAnimDuration;
		f32 m_fCoreShotDuration;
		f32 m_fCoreShotSpeed;
		f32 m_fCoreShotBulletFrequency;
		f32 m_fCoreShotBulletLife;
		f32 m_fCoreShotBulletSize;
		f32 m_fCoreShotMinDist;
		f32 m_fCoreShotHomingFactor;
		f32 m_fCoreShotMaxHomingAngle;
		
		bool m_bDestroyWhenDone;
		Reference<Component> m_Retreat;

		//! returns the closest radial
		Actor* GetClosestRadial() const;
	};
}

#endif // _BOSS3_H_INCLUDED_


