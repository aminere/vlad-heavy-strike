/* 

Amine Rehioui
Created: January 8th 2013

*/

#include "ShootTest.h"

#include "BasicShooter.h"

#include "MeshEntity.h"

namespace shoot
{
	DEFINE_OBJECT(BasicShooter);
	DEFINE_OBJECT(BasicShooterSettings);

	//! constructor
	BasicShooterSettings::BasicShooterSettings()
		: m_fBulletFrequency(1.0f)
		, m_fBulletSpeed(20.0f)
		, m_fBulletLife(4.0f)
		, m_fFirstShotDelay(0.5f)
	{
	}

	//! serializes the entity to/from a PropertyStream
	void BasicShooterSettings::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);

		stream.Serialize(PT_Float, "BulletFrequency", &m_fBulletFrequency);
		stream.Serialize(PT_Float, "BulletSpeed", &m_fBulletSpeed);
		stream.Serialize(PT_Float, "BulletLife", &m_fBulletLife);
		stream.Serialize(PT_Float, "FirstShotDelay", &m_fFirstShotDelay);		
	}

	//! constructor
	BasicShooter::BasicShooter()
		: m_fFirstShotTimer(0.0f)
	{
	}

	//! called during the initialization of the entity
	void BasicShooter::Init()
	{
		super::Init();

		if(BasicShooterSettings* pSettings = static_cast<BasicShooterSettings*>(m_Settings.Get()))
		{
			m_fFirstShotTimer = pSettings->m_fFirstShotDelay;
		}
	}

	//! called during the update of the entity	
	void BasicShooter::Update()
	{
		super::Update();

		if(m_HitPoints < 0)
		{
			return;
		}

		// aim
		Vector3 vPosition = GetTransformationMatrix().GetTranslation();
		Vector3 vPlayerMeshPos = Player::Instance()->GetMeshEntity()->GetTransformationMatrix().GetTranslation();
		Vector3 vDirection = (vPlayerMeshPos-vPosition).Normalize();				
		f32 fAngle = Math::ATan2(-vDirection.X, -vDirection.Y)*Math::RadToDegFactor;
		SetRotation(Vector3::Create(0.0f, 0.0f, fAngle));

		// shoot
		if(!m_Timer.IsRunning() && m_fFirstShotTimer < 0.0f)
		{
			BasicShooterSettings* pSettings = static_cast<BasicShooterSettings*>(m_Settings.Get());
			if(vPosition.Y > vPlayerMeshPos.Y)
			{
				f32 fBulletOffset = m_Mesh->GetMesh()->GetBoundingBox().Size().X/2.0f;

				Bullet::BulletParams params;
				params.vDirection = vDirection;
				params.fSpeed = pSettings->m_fBulletSpeed;
				params.vPosition = vPosition+vDirection*fBulletOffset;			
				params.fLife = pSettings->m_fBulletLife;
				params.fRadius = .5f;

				SFXManager::Instance()->GetEnemyPulseManager()->AddBullet(params);
				m_Timer.Start(pSettings->m_fBulletFrequency);
			}
		}

		m_fFirstShotTimer -= g_fDeltaTime;
	}
}

