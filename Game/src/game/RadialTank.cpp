/* 

Amine Rehioui
Created: August 8th 2013

*/

#include "ShootTest.h"

#include "RadialTank.h"

namespace shoot
{
	DEFINE_OBJECT(RadialTank);
	DEFINE_OBJECT(RadialTankSettings);
	
	//! constructor
	RadialTankSettings::RadialTankSettings()
		: m_fSpeed(4.0f)
		, m_fBurstFrequency(.4f)
		, m_fPauseTime(1.0f)
		, m_BurstCount(3)
		, m_fBulletSpeed(10.0f)
		, m_fBulletLife(3.0f)
		, m_fAngleStep(25.0f)
		, m_vBulletOffset(Vector3::Create(0.0f, -2.54f, 0.0f))
	{
	}

	//! serializes the entity to/from a PropertyStream
	void RadialTankSettings::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);

		stream.Serialize(PT_Float, "Speed", &m_fSpeed);
		stream.Serialize(PT_Float, "BurstFrequency", &m_fBurstFrequency);
		stream.Serialize(PT_UInt, "BurstCount", &m_BurstCount);
		stream.Serialize(PT_Float, "PauseTime", &m_fPauseTime);
		stream.Serialize(PT_Float, "BulletSpeed", &m_fBulletSpeed);
		stream.Serialize(PT_Float, "BulletLife", &m_fBulletLife);
		stream.Serialize(PT_Float, "AngleStep", &m_fAngleStep);		
		stream.Serialize(PT_Vec3, "BulletOffset", &m_vBulletOffset);
	}

	//! constructor
	RadialTank::RadialTank()
		: m_fTimer(-1.0f)
		, m_bBurstMode(true)
		, m_BurstCount(0)
	{
	}

	//! serializes the entity to/from a PropertyStream
	void RadialTank::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);
	}

	//! called during the initialization of the entity
	void RadialTank::Init()
	{
		super::Init();

		if(Player* pPlayer = Player::Instance())
		{
			Vector3 vPosition = pPlayer->GetPosition() + GetSpawningPoint();
			SetAbsolutePosition(vPosition);
		}
	}

	//! called during the update of the entity	
	void RadialTank::Update()
	{
		super::Update();

		if(m_HitPoints < 0)
		{
			return;
		}

		RadialTankSettings* pSettings = static_cast<RadialTankSettings*>(m_Settings.Get());

		if(m_bBurstMode)
		{
			if(m_fTimer < 0.0f)
			{
				// shoot
				Vector3 vBulletPos = GetTransformationMatrix().GetTranslation() + pSettings->m_vBulletOffset;
				f32 fAngleStep = pSettings->m_fAngleStep * Math::DegToRadFactor;
				Bullet::BulletParams params;
				for(f32 f = 0.0f; f <= Math::PI; f += fAngleStep)
				{
					params.vDirection = Vector3::Create(Math::Cos(f), -Math::Sin(f), 0.0f);
					params.fSpeed = pSettings->m_fBulletSpeed;
					params.vPosition = vBulletPos;
					params.fLife = pSettings->m_fBulletLife;
					params.fRadius = .5f;

					SFXManager::Instance()->GetEnemyPulseManager()->AddBullet(params);
				}

				m_fTimer = pSettings->m_fBurstFrequency;
				++m_BurstCount;
			}

			if(m_BurstCount == pSettings->m_BurstCount)
			{
				m_fTimer = pSettings->m_fPauseTime;
				m_bBurstMode = false;
			}
		}
		else
		{
			if(m_fTimer < 0.0f)
			{
				m_bBurstMode = true;
				m_BurstCount = 0;
			}
		}

		m_fTimer -= g_fDeltaTime;		
		Translate(Vector3::Create(0.0f, -pSettings->m_fSpeed*g_fDeltaTime, 0.0f));		
	}
}

