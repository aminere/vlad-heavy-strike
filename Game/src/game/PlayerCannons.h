/* 

Amine Rehioui
Created: November 18th 2011

*/

#ifndef _PLAYER_CANNONS_H_INCLUDED_
#define _PLAYER_CANNONS_H_INCLUDED_

#include "MeshEntity.h"

#include "WeaponConfig.h"

#include "Timer.h"

namespace shoot
{
	// forwards
	class MeshEntity;

	//! PlayerCannons class
	class PlayerCannons : public Entity3D
	{
		DECLARE_OBJECT(PlayerCannons, Entity3D);
	public:

		//! weapon levels
		enum E_Level
		{			
			Level_1,
			Level_2,
			Level_3,
			Level_Count
		};		

		//! Constructor
		PlayerCannons();

		//! Destructor
		virtual ~PlayerCannons();

		//! serializes the entity to/from a PropertyStream
		virtual void Serialize(PropertyStream& stream);

		//! called during the initialization of the entity
		virtual void Init();

		//! called during the update of the entity		
		virtual void Update();

		//! shoots
		void Shoot();

		//! called on a weapon upgrade
		void OnWeaponUpgrade(WeaponConfig::E_Type eType);

		//! resets the cannons
		void Reset();

	private:

		//! config
		struct WeaponConfigs : public ISerializableStruct
		{
			//! Reads/Writes struct properties from/to a stream
			void Serialize(PropertyStream& stream)
			{
				stream.SerializeArray("Levels", &m_Levels, PT_Reference, Level_Count);
			}

			Array< Reference<WeaponConfig> > m_Levels;
		};

		// properties
		f32 m_fPullbackOffset;
		f32 m_fBulletOffset;
		f32 m_fBulletRadius;
		Array<WeaponConfigs> m_Configs;

		enum E_CannonType
		{
			CT_Left,
			CT_Right,
			CT_Count
		};

		enum E_CannonState
		{
			CS_Idle,			
			CS_PullBack,
			CS_Restore
		};

		//! switches the state
		void SwitchState(E_CannonType type, E_CannonState newState);

		//! updates the state
		void UpdateState(f32 fDeltaTime);

		//! shoots a pulse
		void ShootPulse();

		//! shoots a layer
		void ShootLaser();

		//! shoots a pellet
		void ShootPellet();

		Handle<MeshEntity> m_Cannons[CT_Count];
		E_CannonState m_State[CT_Count];		
		Vector3 m_vDestPos[CT_Count];
		Vector3 m_vRestorePos[CT_Count];
		f32 m_Timers[CT_Count];
		u32 m_CurrentCannon;
		Timer m_ShootTimer;
		E_Level m_WeaponLevel;

		WeaponConfig::E_Type m_eWeaponType;
	};
}

#endif // _PLAYER_CANNONS_H_INCLUDED_


