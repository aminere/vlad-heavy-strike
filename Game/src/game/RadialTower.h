/* 

Amine Rehioui
Created: July 21st 2013

*/

#ifndef _RADIAL_TOWER_H_INCLUDED_
#define _RADIAL_TOWER_H_INCLUDED_

#include "Timer.h"

namespace shoot
{
	//! RadialTower settings
	class RadialTowerSettings : public ActorSettings
	{
		DECLARE_OBJECT(RadialTowerSettings, ActorSettings);

	public:

		//! constructor
		RadialTowerSettings();

		//! serializes the entity to/from a PropertyStream
		void Serialize(PropertyStream& stream);

		f32 m_fAngleStep;
		f32 m_fBulletFrequency;
		f32 m_fFirstShotDelay;
		f32 m_fBulletSpeed;
		f32 m_fBulletLife;
		f32 m_fBulletOffset;
		u32 m_Sides;

		//! sides
		enum E_Side
		{
			S_Left	= 1 << 0,
			S_Right = 1 << 1,
			S_Front = 1 << 2,
			S_Back	= 1 << 3
		};
	};

	//! RadialTower class
	class RadialTower : public Actor
	{
	public:
		
		DECLARE_OBJECT(RadialTower, Actor);

		//! constructor
		RadialTower();

		//! called during the initialization of the entity
		virtual void Init();

		//! called during the update of the entity		
		virtual void Update();

	private:

		f32 m_fFirstShotTimer;
		Timer m_Timer;
	};
}

#endif // _RADIAL_TOWER_H_INCLUDED_


