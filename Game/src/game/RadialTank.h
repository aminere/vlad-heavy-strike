/* 

Amine Rehioui
Created: August 8th 2013

*/

#ifndef _RADIAL_TANK_H_INCLUDED_
#define _RADIAL_TANK_H_INCLUDED_

namespace shoot
{
	//! RadialTankSettings
	class RadialTankSettings : public ActorSettings
	{
		DECLARE_OBJECT(RadialTankSettings, ActorSettings);

	public:

		//! constructor
		RadialTankSettings();

		//! serializes the entity to/from a PropertyStream
		void Serialize(PropertyStream& stream);
		
		f32 m_fSpeed;
		f32 m_fPauseTime;
		f32 m_fBurstFrequency;
		u32 m_BurstCount;
		f32 m_fBulletSpeed;
		f32 m_fBulletLife;
		f32 m_fAngleStep;
		Vector3 m_vBulletOffset;
	};

	//! RadialTank
	class RadialTank : public Actor
	{
	public:
		
		DECLARE_OBJECT(RadialTank, Actor);

		//! constructor
		RadialTank();

		//! serializes the entity to/from a PropertyStream
		void Serialize(PropertyStream& stream);

		//! called during the initialization of the entity
		virtual void Init();

		//! called during the update of the entity		
		virtual void Update();
		
	private:

		f32 m_fTimer;
		bool m_bBurstMode;
		u32 m_BurstCount;
	};
}

#endif // _RADIAL_TANK_H_INCLUDED_


