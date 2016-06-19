/* 

Amine Rehioui
Created: June 19th 2013

*/

#ifndef _SPIN_SHOOTER_H_INCLUDED_
#define _SPIN_SHOOTER_H_INCLUDED_

#include "BasicShooter.h"

namespace shoot
{
	//! SpinShooter settings
	class SpinShooterSettings : public BasicShooterSettings
	{
		DECLARE_OBJECT(SpinShooterSettings, BasicShooterSettings);

	public:

		//! constructor
		SpinShooterSettings();

		//! serializes the entity to/from a PropertyStream
		void Serialize(PropertyStream& stream);

		f32 m_fRotationSpeed;
		f32 m_fBulletOffset;
	};

	//! SpinShooter class
	class SpinShooter : public BasicShooter
	{
	public:
		
		DECLARE_OBJECT(SpinShooter, BasicShooter);

		//! called during the update of the entity		
		void Update();
	};
}

#endif // _SPIN_SHOOTER_H_INCLUDED_


