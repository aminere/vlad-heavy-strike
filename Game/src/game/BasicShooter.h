/* 

Amine Rehioui
Created: January 8th 2013

*/

#ifndef _BASIC_SHOOTER_H_INCLUDED_
#define _BASIC_SHOOTER_H_INCLUDED_

#include "BasicEnemy.h"

#include "Timer.h"

namespace shoot
{
	//! BasicShooter settings
	class BasicShooterSettings : public BasicEnemySettings
	{
		DECLARE_OBJECT(BasicShooterSettings, BasicEnemySettings);

	public:

		//! constructor
		BasicShooterSettings();

		//! serializes the entity to/from a PropertyStream
		void Serialize(PropertyStream& stream);

		f32 m_fBulletFrequency;
		f32 m_fBulletSpeed;
		f32 m_fBulletLife;
		f32 m_fFirstShotDelay;
	};

	//! Base BasicShooter class
	class BasicShooter : public BasicEnemy
	{
	public:
		
		DECLARE_OBJECT(BasicShooter, BasicEnemy);

		//! constructor
		BasicShooter();

		//! called during the initialization of the entity
		virtual void Init();

		//! called during the update of the entity		
		virtual void Update();

	protected:

		Timer m_Timer;
		f32 m_fFirstShotTimer; 
	};
}

#endif // _BASIC_SHOOTER_H_INCLUDED_


