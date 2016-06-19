/* 

Amine Rehioui
Created: March 3rd 2013

*/

#include "ShootTest.h"

#include "RadialTower.h"

#include "MeshEntity.h"

namespace shoot
{
	DEFINE_OBJECT(RadialTower);
	DEFINE_OBJECT(RadialTowerSettings);

	//! constructor
	RadialTowerSettings::RadialTowerSettings()
		: m_fAngleStep(25.0f)
		, m_fBulletFrequency(1.5f)
		, m_fFirstShotDelay(1.0f)
		, m_fBulletSpeed(10.0f)
		, m_fBulletLife(2.5f)
		, m_fBulletOffset(1.3f)
		, m_Sides(S_Front)
	{
	}

	//! serializes the entity to/from a PropertyStream
	void RadialTowerSettings::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);

		stream.Serialize(PT_Float, "AngleStep", &m_fAngleStep);
		stream.Serialize(PT_Float, "BulletFrequency", &m_fBulletFrequency);
		stream.Serialize(PT_Float, "FirstShotDelay", &m_fFirstShotDelay);
		stream.Serialize(PT_Float, "BulletSpeed", &m_fBulletSpeed);
		stream.Serialize(PT_Float, "BulletLife", &m_fBulletLife);
		stream.Serialize(PT_Float, "BulletOffset", &m_fBulletOffset);

		#define SERIALIZE_SIDE(x) \
		bool bSide_##x = (m_Sides & S_##x) != 0; \
		if(stream.Serialize(PT_Bool, "Side_"#x, &bSide_##x)) { bSide_##x ? (m_Sides |= S_##x) : m_Sides &= ~S_##x; }

		SERIALIZE_SIDE(Left);
		SERIALIZE_SIDE(Right);
		SERIALIZE_SIDE(Front);
		SERIALIZE_SIDE(Back);
	}

	//! constructor
	RadialTower::RadialTower()
		: m_fFirstShotTimer(0.0f)
	{
	}

	//! called during the initialization of the entity
	void RadialTower::Init()
	{
		super::Init();

		if(RadialTowerSettings* pSettings = static_cast<RadialTowerSettings*>(m_Settings.Get()))
		{
			m_fFirstShotTimer = pSettings->m_fFirstShotDelay;
		}
	}

	//! called during the update of the entity	
	void RadialTower::Update()
	{
		super::Update();
		
		if(m_HitPoints < 0)
		{
			return;
		}

		if(!m_Timer.IsRunning() && m_fFirstShotTimer < 0.0f)
		{
			RadialTowerSettings* pSettings = static_cast<RadialTowerSettings*>(m_Settings.Get());
			Vector3 vPosition = GetTransformationMatrix().GetTranslation();
			Vector3 vPlayerMeshPos = Player::Instance()->GetMeshEntity()->GetTransformationMatrix().GetTranslation();
			f32 fAngleStep = pSettings->m_fAngleStep * Math::DegToRadFactor;
			f32 fStartAngle = 0.0f;
			f32 fEndAngle = Math::PI/2.0f;

			if(vPosition.Y > vPlayerMeshPos.Y || (pSettings->m_Sides & RadialTowerSettings::S_Back))
			{
				if(pSettings->m_Sides & RadialTowerSettings::S_Left)
				{
					if(pSettings->m_Sides & RadialTowerSettings::S_Front)
					{
						fStartAngle = -Math::PI/2.0f;
						fEndAngle = 0.0f;
					}
					else if(pSettings->m_Sides & RadialTowerSettings::S_Back)
					{
						fStartAngle = 0.0f;
						fEndAngle = Math::PI/2.0f;
					}
					else
					{
						fStartAngle = -Math::PI/2.0f;
						fEndAngle = Math::PI/2.0f;
					}
				}
				else if(pSettings->m_Sides & RadialTowerSettings::S_Right)
				{
					if(pSettings->m_Sides & RadialTowerSettings::S_Front)
					{
						fStartAngle = Math::PI;
						fEndAngle = Math::PI/2.0f*3.0f;
					}
					else if(pSettings->m_Sides & RadialTowerSettings::S_Back)
					{
						fStartAngle = Math::PI/2.0f;
						fEndAngle = Math::PI;
					}
					else
					{
						fStartAngle = Math::PI/2.0f;
						fEndAngle = Math::PI/2.0f*3.0f;
					}
				}
				else if(pSettings->m_Sides & RadialTowerSettings::S_Front)
				{
					if(pSettings->m_Sides & RadialTowerSettings::S_Left)
					{
						fStartAngle = -Math::PI/2.0f;
						fEndAngle = 0.0f;
					}
					else if(pSettings->m_Sides & RadialTowerSettings::S_Right)
					{
						fStartAngle = -Math::PI;
						fEndAngle = -Math::PI/2.0f;
					}
					else
					{
						fStartAngle = -Math::PI;
						fEndAngle = 0.0f;
					}
				}
				else if(pSettings->m_Sides & RadialTowerSettings::S_Back)
				{
					if(pSettings->m_Sides & RadialTowerSettings::S_Left)
					{
						fStartAngle = 0.0f;
						fEndAngle = Math::PI/2.0f;
					}
					else if(pSettings->m_Sides & RadialTowerSettings::S_Right)
					{
						fStartAngle =  Math::PI/2.0f;
						fEndAngle = Math::PI;
					}
					else
					{
						fStartAngle = 0.0f;
						fEndAngle = Math::PI;
					}
				}

				for(f32 f = fStartAngle; f <= fEndAngle; f += fAngleStep)
				{
					Bullet::BulletParams params;
					params.vDirection = Vector3::Create(Math::Cos(f), Math::Sin(f), 0.0f);
					params.fSpeed = pSettings->m_fBulletSpeed;
					params.vPosition = vPosition+params.vDirection*pSettings->m_fBulletOffset;
					params.fLife = pSettings->m_fBulletLife;
					params.fRadius = .5f;

					SFXManager::Instance()->GetEnemyPulseManager()->AddBullet(params);
				}

				m_Timer.Start(pSettings->m_fBulletFrequency);
			}
		}		

		m_fFirstShotTimer -= g_fDeltaTime;
	}
}

