/* 

Amine Rehioui
Created: May 5th 2013

*/

#ifndef _BASIC_FIGHTER_H_INCLUDED_
#define _BASIC_FIGHTER_H_INCLUDED_

#include "Event.h"

namespace shoot
{
	//! BasicFighter settings
	class BasicFighterSettings : public ActorSettings
	{
		DECLARE_OBJECT(BasicFighterSettings, ActorSettings);

	public:

		//! constructor
		BasicFighterSettings();

		//! serializes the entity to/from a PropertyStream
		void Serialize(PropertyStream& stream);

		f32 m_fShootDuration;
		f32 m_fShootFrequency;
		f32 m_fBulletSpeed;
		f32 m_fBulletLife;
		f32 m_fHomingFactor;
	};

	//! BasicFighter class
	class BasicFighter : public Actor
	{
	public:
		
		DECLARE_OBJECT(BasicFighter, Actor);

		//! constructor
		BasicFighter();

		//! constructor
		virtual ~BasicFighter();

		//! serializes the entity to/from a PropertyStream
		void Serialize(PropertyStream& stream);

		//! called during the initialization of the entity
		virtual void Init();

		//! called during the update of the entity		
		virtual void Update();

		//! shoot event listeners
		static void HandleTriggerEvent(void* pListener, Event* pEvent);

		//! sets path to follow
		inline void SetForcePath(s32 path) { m_ForcePath = path; }

	private:

		Handle<Entity3D> m_ShootingPoint;
		Handle<Entity3D> m_LeftCannon;
		Handle<Entity3D> m_RightCannon;		
		f32 m_fShootTimer;
		f32 m_fShootDelay;
		u32 m_CurrentCannon;
		Vector3 m_vDirection;
		s32 m_ForcePath;
	};
}

#endif // _BASIC_FIGHTER_H_INCLUDED_


