/* 

Amine Rehioui
Created: July 14th 2013

*/

#include "ShootTest.h"

#include "RadialShooter.h"

namespace shoot
{
	DEFINE_OBJECT(RadialShooter);
	DEFINE_OBJECT(RadialShooterSettings);

	//! constructor
	RadialShooterSettings::RadialShooterSettings()
		: m_NumBullets(10)
		, m_fPauseDuration(.5f)
	{
	}

	//! serializes the entity to/from a PropertyStream
	void RadialShooterSettings::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);
		stream.Serialize(PT_Int, "NumBullets", &m_NumBullets);
		stream.Serialize(PT_Float, "PauseDuration", &m_fPauseDuration);
	}

	//! constructor
	RadialShooter::RadialShooter()
		: m_fPauseTimer(-1.0f)
	{
	}

	//! called during the update of the entity	
	void RadialShooter::Update()
	{
		Actor::Update();

		if(m_HitPoints < 0)
		{
			return;
		}

		RadialShooterSettings* pSettings = static_cast<RadialShooterSettings*>(m_Settings.Get());

		if(m_fPauseTimer < 0.0f)
		{
			Vector3 vSpeed = Vector3::Create(0.0f, -1.0f, 0.0f)*(pSettings->m_fSpeed*g_fDeltaTime);
			Translate(vSpeed);
		}
		else
		{
			m_fPauseTimer -= g_fDeltaTime;
		}

		// shoot
		if(!m_Timer.IsRunning() && m_fFirstShotTimer < 0.0f)
		{
			Vector3 vPosition = GetTransformationMatrix().GetTranslation();
			Vector3 vPlayerMeshPos = Player::Instance()->GetMeshEntity()->GetTransformationMatrix().GetTranslation();
			if(vPosition.Y > vPlayerMeshPos.Y)
			{				
				f32 fBulletOffset = m_Mesh->GetMesh()->GetBoundingBox().Size().X*m_Mesh->GetAbsoluteScale().X/2.0f;
				f32 fAngleStep = (360.0f/pSettings->m_NumBullets)*Math::DegToRadFactor;
				f32 fAngle = 0.0f;
				
				for(u32 i=0; i<pSettings->m_NumBullets; ++i)
				{
					Bullet::BulletParams params;
					params.vDirection = Vector3::Create(Math::Cos(fAngle), Math::Sin(fAngle), 0.0f);
					params.fSpeed = pSettings->m_fBulletSpeed;
					params.vPosition = vPosition+params.vDirection*fBulletOffset;
					params.fLife = pSettings->m_fBulletLife;
					params.fRadius = .5f;
					fAngle += fAngleStep;

					SFXManager::Instance()->GetEnemyPulseManager()->AddBullet(params);
				}

				m_Timer.Start(pSettings->m_fBulletFrequency);
				m_fPauseTimer = pSettings->m_fPauseDuration;
			}
		}

		m_fFirstShotTimer -= g_fDeltaTime;
	}
}

