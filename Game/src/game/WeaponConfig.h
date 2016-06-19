/* 

Amine Rehioui
Created: April 27th 2013

*/

#ifndef _WEAPON_CONFIG_H_INCLUDED_
#define _WEAPON_CONFIG_H_INCLUDED_

namespace shoot
{
	//! weapon config
	class WeaponConfig : public Object
	{
		DECLARE_OBJECT(WeaponConfig, Object)
	public:

		//! weapon types
		enum E_Type
		{
			T_Pulse,
			T_Laser,
			T_Pellet,
			T_Count
		};

		//! constructor
		WeaponConfig();

		//! serializes the entity to/from a PropertyStream
		virtual void Serialize(PropertyStream& stream);

		f32 m_fBulletSpeed;
		f32 m_fBulletLife;
		f32 m_fBulletSize;
		f32 m_fShootFrequency;
		f32 m_BulletDamage;
	};

	//! pulse config
	class PulseConfig : public WeaponConfig
	{
		DECLARE_OBJECT(PulseConfig, WeaponConfig)
	};

	//! laser config
	class LaserConfig : public WeaponConfig
	{
		DECLARE_OBJECT(LaserConfig, WeaponConfig)
	public:

		//! constructor
		LaserConfig();

		//! serializes the entity to/from a PropertyStream
		void Serialize(PropertyStream& stream);

		f32 m_fLaserLength;
	};

	//! pellet config
	class PelletConfig : public WeaponConfig
	{
		DECLARE_OBJECT(PelletConfig, WeaponConfig)

		//! constructor
		PelletConfig();

		//! serializes the entity to/from a PropertyStream
		void Serialize(PropertyStream& stream);

		f32 m_fMinDistToEnemy;
		f32 m_fHomingFactor;
		f32 m_fMaxHomingAngle;
	};
}

#endif // _WEAPON_CONFIG_H_INCLUDED_

