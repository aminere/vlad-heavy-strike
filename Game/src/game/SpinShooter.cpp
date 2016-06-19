/* 

Amine Rehioui
Created: January 8th 2013

*/

#include "ShootTest.h"

#include "SpinShooter.h"

namespace shoot
{
	DEFINE_OBJECT(SpinShooter);
	DEFINE_OBJECT(SpinShooterSettings);

	//! constructor
	SpinShooterSettings::SpinShooterSettings()
		: m_fRotationSpeed(180.0f)
		, m_fBulletOffset(3.47f)
	{
	}

	//! serializes the entity to/from a PropertyStream
	void SpinShooterSettings::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);
		stream.Serialize(PT_Float, "RotationSpeed", &m_fRotationSpeed);
		stream.Serialize(PT_Float, "BulletOffset", &m_fBulletOffset);
	}

	//! called during the update of the entity	
	void SpinShooter::Update()
	{
		Actor::Update();

		if(m_HitPoints < 0)
		{
			return;
		}

		SpinShooterSettings* pSettings = static_cast<SpinShooterSettings*>(m_Settings.Get());
		f32 fRotationOffset = pSettings->m_fRotationSpeed*g_fDeltaTime;
		m_Mesh->Rotate(Vector3::Create(0.0f, 0.0f, fRotationOffset));

		// shoot
		if(!m_Timer.IsRunning() && m_fFirstShotTimer < 0.0f)
		{
			Vector3 vPosition = GetTransformationMatrix().GetTranslation();
			Vector3 vPlayerMeshPos = Player::Instance()->GetMeshEntity()->GetTransformationMatrix().GetTranslation();
			if(vPosition.Y > vPlayerMeshPos.Y)
			{
				f32 fRotation = (m_Mesh->GetRotation().Z+fRotationOffset)*Math::DegToRadFactor;
				Vector3 vDirection = Vector3::Create(Math::Sin(fRotation), Math::Cos(fRotation), 0.0f);				

				Bullet::BulletParams params;
				params.vDirection = vDirection;
				params.fSpeed = pSettings->m_fBulletSpeed;
				params.vPosition = vPosition+vDirection*pSettings->m_fBulletOffset;			
				params.fLife = pSettings->m_fBulletLife;
				params.fRadius = .5f;

				SFXManager::Instance()->GetEnemyPulseManager()->AddBullet(params);
				m_Timer.Start(pSettings->m_fBulletFrequency);
			}
		}

		Vector3 vSpeed = Vector3::Create(0.0f, -1.0f, 0.0f)*(pSettings->m_fSpeed*g_fDeltaTime);
		Translate(vSpeed);

		m_fFirstShotTimer -= g_fDeltaTime;
	}
}

