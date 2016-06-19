/* 

Amine Rehioui
Created: November 18th 2011

*/

#include "ShootTest.h"

#include "PlayerCannons.h"

#include "AudioManager.h"

namespace shoot
{
	DEFINE_OBJECT(PlayerCannons);

	// weapon type size literals
	const char* const g_strWeaponTypeLiterals[] =
	{
		"Pulse",
		"Laser",
		"Pellet",
		0
	};

	// weapon level literals
	const char* const g_strWeaponLevelLiterals[] =
	{
		"Level_1",
		"Level_2",
		"Level_3",
		0
	};

	//! Constructor
	PlayerCannons::PlayerCannons()
		: m_CurrentCannon(0)
		, m_eWeaponType(WeaponConfig::T_Pulse)
		, m_WeaponLevel(Level_1)
		// properties
		, m_fPullbackOffset(.2f)
		, m_fBulletOffset(1.3f)
		, m_fBulletRadius(.5f)
	{
		for(u32 i=0; i<CT_Count; ++i)
		{
			m_State[i] = CS_Idle;
		}		
	}

	//! Destructor
	PlayerCannons::~PlayerCannons()
	{
	}

	//! serializes the entity to/from a PropertyStream
	void PlayerCannons::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);

		stream.Serialize(PT_Float, "PullbackOffset", &m_fPullbackOffset);
		stream.Serialize(PT_Float, "BulletOffset", &m_fBulletOffset);
		stream.Serialize(PT_Float, "BulletRadius", &m_fBulletRadius);
		stream.SerializeArray("Configs", &m_Configs, PT_Struct, WeaponConfig::T_Count);

		// TEMP for debugging
		if(IsInitialized())
		{
			stream.Serialize(PT_Enum, "WeaponType", &m_eWeaponType, ENUM_PARAMS1(g_strWeaponTypeLiterals));
			stream.Serialize(PT_Enum, "WeaponLevel", &m_WeaponLevel, ENUM_PARAMS1(g_strWeaponLevelLiterals));
		}
	}

	//! called during the initialization of the entity
	void PlayerCannons::Init()		
	{
		super::Init();

		m_Cannons[CT_Left] = static_cast<MeshEntity*>(GetChildByName("LeftCannon"));
		m_Cannons[CT_Right] = static_cast<MeshEntity*>(GetChildByName("RightCannon"));

		for(u32 i=0; i<CT_Count; ++i)
		{
			if(m_Cannons[i].IsValid())
			{
				m_vRestorePos[i] = m_Cannons[i]->GetPosition();
			}
		}

		// create configs if empty
		if(m_Configs.GetSize() == 0)
		{
#define ADD_WEAPON_CONFIG(weapon) \
			WeaponConfigs* p##weapon##Configs = snew WeaponConfigs(); \
			for(u32 i=0; i<Level_Count; ++i) \
				p##weapon##Configs->m_Levels.Add(snew Reference<WeaponConfig>(snew weapon##Config())); \
			m_Configs.Add(p##weapon##Configs);

			ADD_WEAPON_CONFIG(Pulse);
			ADD_WEAPON_CONFIG(Laser);
			ADD_WEAPON_CONFIG(Pellet);
		}
	}

	void PlayerCannons::Update()
	{
		UpdateState(g_fDeltaTime);		
	}

	//! shoots
	void PlayerCannons::Shoot()
	{
		if(!m_ShootTimer.IsRunning())
		{
			switch(m_eWeaponType)
			{
			case WeaponConfig::T_Pulse:
				switch(m_WeaponLevel)
				{
				case Level_1:
				case Level_2:
					ShootPulse();
					break;
				
				case Level_3:
					ShootPulse();
					ShootPulse();
					break;
				}
				AudioManager::Instance()->Play(AudioManager::S_Pulse);
				break;

			case WeaponConfig::T_Laser:
				ShootLaser();
				ShootLaser();
				AudioManager::Instance()->Play(AudioManager::S_Laser);
				break;

			case WeaponConfig::T_Pellet:
				ShootPellet();
				AudioManager::Instance()->Play(AudioManager::S_Pulse);
				break;
			}
			m_ShootTimer.Start(m_Configs[m_eWeaponType].m_Levels[m_WeaponLevel]->m_fShootFrequency);
		}
	}

	//! called on a weapon upgrade
	void PlayerCannons::OnWeaponUpgrade(WeaponConfig::E_Type eType)
	{
		if(eType == m_eWeaponType)
		{
			if(m_WeaponLevel < Level_Count-1)
			{
				m_WeaponLevel = (E_Level)(m_WeaponLevel+1);
			}			
		}

		m_ShootTimer.Stop();
		m_eWeaponType = eType;
	}

	//! resets the cannons
	void PlayerCannons::Reset()
	{
		m_WeaponLevel = Level_1;
	}

	//! switches the state
	void PlayerCannons::SwitchState(E_CannonType type, E_CannonState newState)
	{
		switch(newState)
		{
		case CS_PullBack:
			m_vDestPos[type] = m_vRestorePos[type]-Vector3::Create(0.0f, m_fPullbackOffset, 0.0f);
			m_Timers[type] = m_Configs[m_eWeaponType].m_Levels[m_WeaponLevel]->m_fShootFrequency/2.0f;
			break;

		case CS_Restore:
			m_vDestPos[type] = m_vRestorePos[type];
			m_Timers[type] = m_Configs[m_eWeaponType].m_Levels[m_WeaponLevel]->m_fShootFrequency/2.0f;
			break;
		}
		m_State[type] = newState;
	}

	//! updates the state
	void PlayerCannons::UpdateState(f32 fDeltaTime)
	{		
		for(u32 i=0; i<CT_Count; ++i)
		{
			Vector3 cannonPos = m_Cannons[i]->GetPosition();

			switch(m_State[i])
			{
			case CS_PullBack:
				cannonPos = Math::Damp(cannonPos, m_vDestPos[i], fDeltaTime, m_Configs[m_eWeaponType].m_Levels[m_WeaponLevel]->m_fShootFrequency/2.0f);
				m_Timers[i] -= fDeltaTime;
				if(m_Timers[i] < 0.0f)
				{
					cannonPos = m_vDestPos[i];
					SwitchState(E_CannonType(i), CS_Restore);
				}				
				break;

			case CS_Restore:					
				cannonPos = Math::Damp(cannonPos, m_vDestPos[i], fDeltaTime, m_Configs[m_eWeaponType].m_Levels[m_WeaponLevel]->m_fShootFrequency/2.0f);
				m_Timers[i] -= fDeltaTime;
				if(m_Timers[i] < 0.0f)
				{
					cannonPos = m_vDestPos[i];
					SwitchState(E_CannonType(i), CS_Idle);
				}
				break;
			}

			m_Cannons[i]->SetPosition(cannonPos);
		}
	}

	//! shoots a pulse
	void PlayerCannons::ShootPulse()
	{
		PulseConfig* pConfig = static_cast<PulseConfig*>(m_Configs[WeaponConfig::T_Pulse].m_Levels[m_WeaponLevel].Get());
		Vector3 vDirection = Vector3::Create(0.0f, 1.0f, 0.0f);
		Vector3 vCannonPos = m_Cannons[m_CurrentCannon]->GetTransformationMatrix().GetTranslation();
		vCannonPos.Z = 0.0f;

		Bullet::BulletParams params;
		params.vPosition = vCannonPos+vDirection*m_fBulletOffset;
		params.vDirection = vDirection;
		params.fSpeed = pConfig->m_fBulletSpeed;
		params.fLife = pConfig->m_fBulletLife;
		params.fRadius = pConfig->m_fBulletSize/2.0f;
		params.type = Bullet::T_Pulse;
		params.damage = pConfig->m_BulletDamage;
		SFXManager::Instance()->GetPlayerPulseManager()->AddBullet(params);		

		SwitchState(E_CannonType(m_CurrentCannon), CS_PullBack);
		m_CurrentCannon = (m_CurrentCannon+1)%CT_Count;
	}

	//! shoots a layer
	void PlayerCannons::ShootLaser()
	{
		LaserConfig* pConfig = static_cast<LaserConfig*>(m_Configs[WeaponConfig::T_Laser].m_Levels[m_WeaponLevel].Get());
		Vector3 vDirection = Vector3::Create(0.0f, 1.0f, 0.0f);
		Vector3 vCannonPos = m_Cannons[m_CurrentCannon]->GetTransformationMatrix().GetTranslation();
		vCannonPos.Z = 0.0f;

		Laser::Params params;
		params.vStart = vCannonPos+vDirection*m_fBulletOffset;
		params.vDir = vDirection;
		params.fSpeed = pConfig->m_fBulletSpeed;
		params.fLife = pConfig->m_fBulletLife;
		params.fMaxLength = pConfig->m_fLaserLength;
		params.damage = pConfig->m_BulletDamage;
		params.fThickness = pConfig->m_fBulletSize;
		SFXManager::Instance()->GetPlayerLaserManager()->AddLaser(params);		

		SwitchState(E_CannonType(m_CurrentCannon), CS_PullBack);
		m_CurrentCannon = (m_CurrentCannon+1)%CT_Count;
	}

	//! shoots a pellet
	void PlayerCannons::ShootPellet()
	{
		PelletConfig* pConfig = static_cast<PelletConfig*>(m_Configs[WeaponConfig::T_Pellet].m_Levels[m_WeaponLevel].Get());
		Vector3 vDirection = Vector3::Create(0.0f, 1.0f, 0.0f);
		Vector3 vCannonPos = m_Cannons[m_CurrentCannon]->GetTransformationMatrix().GetTranslation();
		vCannonPos.Z = 0.0f;

		Bullet::BulletParams params;
		params.vPosition = vCannonPos+vDirection*m_fBulletOffset;
		params.vDirection = vDirection;
		params.fSpeed = pConfig->m_fBulletSpeed;
		params.fLife = pConfig->m_fBulletLife;
		params.fRadius = pConfig->m_fBulletSize/2.0f;
		params.type = Bullet::T_Pellet;
		params.damage = pConfig->m_BulletDamage;
		PelletManager* pManager = SFXManager::Instance()->GetPlayerPelletManager();
		pManager->SetMinDistToEnemy(pConfig->m_fMinDistToEnemy);
		pManager->SetHomingFactor(pConfig->m_fHomingFactor);
		pManager->SetMaxHomingAngle(pConfig->m_fMaxHomingAngle);
		pManager->AddBullet(params);		

		SwitchState(E_CannonType(m_CurrentCannon), CS_PullBack);
		m_CurrentCannon = (m_CurrentCannon+1)%CT_Count;
	}
}

