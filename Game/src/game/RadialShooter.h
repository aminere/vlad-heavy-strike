/* 

Amine Rehioui
Created: July 14th 2013

*/

#ifndef _RADIAL_SHOOTER_H_INCLUDED_
#define _RADIAL_SHOOTER_H_INCLUDED_

#include "BasicShooter.h"

namespace shoot
{
	//! RadialShooter settings
	class RadialShooterSettings : public BasicShooterSettings
	{
		DECLARE_OBJECT(RadialShooterSettings, BasicShooterSettings);

	public:

		//! constructor
		RadialShooterSettings();

		//! serializes the entity to/from a PropertyStream
		void Serialize(PropertyStream& stream);

		u32 m_NumBullets;
		f32 m_fPauseDuration;
	};

	//! RadialShooter class
	class RadialShooter : public BasicShooter
	{
	public:
		
		DECLARE_OBJECT(RadialShooter, BasicShooter);

		//! constructor
		RadialShooter();

		//! called during the update of the entity		
		void Update();

	private:

		f32 m_fPauseTimer;
	};
}

#endif // _RADIAL_SHOOTER_H_INCLUDED_


