/* 

Amine Rehioui
Created: April 27th 2013

*/

#include "ShootTest.h"

#include "WeaponConfig.h"

namespace shoot
{
	DEFINE_OBJECT(WeaponConfig);
	DEFINE_OBJECT(PulseConfig);
	DEFINE_OBJECT(LaserConfig);
	DEFINE_OBJECT(PelletConfig);

	//! constructor
	WeaponConfig::WeaponConfig()
		: m_fBulletSpeed(50.0f)
		, m_fBulletLife(1.3f)
		, m_fShootFrequency(0.04f)
		, m_fBulletSize(4.0f)
		, m_BulletDamage(1.0f)
	{
	}

	//! serializes the entity to/from a PropertyStream
	void WeaponConfig::Serialize(PropertyStream& stream)
	{
		stream.Serialize(PT_Float, "BulletSpeed", &m_fBulletSpeed);
		stream.Serialize(PT_Float, "BulletLife", &m_fBulletLife);
		stream.Serialize(PT_Float, "ShootFrequency", &m_fShootFrequency);
		stream.Serialize(PT_Float, "BulletSize", &m_fBulletSize);
		stream.Serialize(PT_Float, "BulletDamage", &m_BulletDamage);	
	}

	//! constructor
	LaserConfig::LaserConfig()
		: m_fLaserLength(4.0f)
	{
	}

	//! serializes the entity to/from a PropertyStream
	void LaserConfig::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);
		stream.Serialize(PT_Float, "LaserLength", &m_fLaserLength);
	}

	//! constructor
	PelletConfig::PelletConfig()
		: m_fMinDistToEnemy(5.0f)
		, m_fHomingFactor(5.0f)
		, m_fMaxHomingAngle(45.0f)
	{
	}

	//! serializes the entity to/from a PropertyStream
	void PelletConfig::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);

		stream.Serialize(PT_Float, "MinDistToEnemy", &m_fMinDistToEnemy);
		stream.Serialize(PT_Float, "HomingFactor", &m_fHomingFactor);
		stream.Serialize(PT_Float, "MaxHomingAngle", &m_fMaxHomingAngle);
	}
}

