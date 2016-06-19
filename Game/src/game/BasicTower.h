/* 

Amine Rehioui
Created: March 3rd 2013

*/

#ifndef _BASIC_TOWER_H_INCLUDED_
#define _BASIC_TOWER_H_INCLUDED_

#include "Timer.h"

namespace shoot
{
	//! BasicTower settings
	class BasicTowerSettings : public ActorSettings
	{
		DECLARE_OBJECT(BasicTowerSettings, ActorSettings);

	public:

		//! constructor
		BasicTowerSettings();

		//! serializes the entity to/from a PropertyStream
		void Serialize(PropertyStream& stream);

		f32 m_fAimDuration;
		f32 m_fAimOffset;
		f32 m_fFirstShotDelay;
		f32 m_fBulletSpeed;
		f32 m_fBulletLife;
		Vector3 m_vBulletOffset;
		f32 m_fBulletSize;
		f32 m_fBulletDamage;
	};

	//! BasicTower class
	class BasicTower : public Actor
	{
	public:
		
		DECLARE_OBJECT(BasicTower, Actor);

		//! constructor
		BasicTower();

		//! called during the initialization of the entity
		virtual void Init();

		//! called during the update of the entity		
		virtual void Update();

	private:

		f32 m_fFirstShotTimer;
		Timer m_Timer;
		Handle<Entity3D> m_Core;
		Vector3 m_vKnownPlayerPos;
	};
}

#endif // _BASIC_TOWER_H_INCLUDED_


