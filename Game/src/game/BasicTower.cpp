/* 

Amine Rehioui
Created: March 3rd 2013

*/

#include "ShootTest.h"

#include "BasicTower.h"

#include "MeshEntity.h"

namespace shoot
{
	DEFINE_OBJECT(BasicTower);
	DEFINE_OBJECT(BasicTowerSettings);

	//! constructor
	BasicTowerSettings::BasicTowerSettings()
		: m_fAimDuration(1.5f)
		, m_fAimOffset(2.0f)
		, m_fFirstShotDelay(1.0f)
		, m_fBulletSpeed(15.0f)
		, m_fBulletLife(4.0f)
		, m_vBulletOffset(Vector3::Create(0.0f, 2.0f, 0.0f))
		, m_fBulletSize(1.0f)
		, m_fBulletDamage(30.0f)
	{
	}

	//! serializes the entity to/from a PropertyStream
	void BasicTowerSettings::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);

		stream.Serialize(PT_Float, "AimDuration", &m_fAimDuration);
		stream.Serialize(PT_Float, "FirstShotDelay", &m_fFirstShotDelay);
		stream.Serialize(PT_Float, "BulletSpeed", &m_fBulletSpeed);
		stream.Serialize(PT_Float, "BulletLife", &m_fBulletLife);
		stream.Serialize(PT_Vec3, "BulletOffset", &m_vBulletOffset);
		stream.Serialize(PT_Float, "BulletSize", &m_fBulletSize);
		stream.Serialize(PT_Float, "BulletDamage", &m_fBulletDamage);
	}

	//! constructor
	BasicTower::BasicTower()
		: m_fFirstShotTimer(0.0f)
		, m_vKnownPlayerPos(Vector3::Zero)
	{
	}

	//! called during the initialization of the entity
	void BasicTower::Init()
	{
		super::Init();

		if(BasicTowerSettings* pSettings = static_cast<BasicTowerSettings*>(m_Settings.Get()))
		{
			m_fFirstShotTimer = pSettings->m_fFirstShotDelay;
		}

		if(Player* pPlayer = Player::Instance())
		{
			m_vKnownPlayerPos = pPlayer->GetMeshEntity()->GetTransformationMatrix().GetTranslation();
		}

		m_Core = static_cast<Entity3D*>(GetChildByName("Core"));
		if(m_Core.IsValid())
		{
			m_Core->SetUseRotationMatrix(true);
		}
	}

	//! called during the update of the entity	
	void BasicTower::Update()
	{
		super::Update();
		
		if(m_HitPoints < 0)
		{
			return;
		}

		Player* pPlayer = Player::Instance();
		Vector3 vPosition = m_Core->GetTransformationMatrix().GetTranslation();
		Vector3 vPlayerMeshPos = pPlayer->GetMeshEntity()->GetTransformationMatrix().GetTranslation();

		if(vPosition.Y > vPlayerMeshPos.Y)
		{
			// aim
			BasicTowerSettings* pSettings = static_cast<BasicTowerSettings*>(m_Settings.Get());

			Vector3 vAimPos = vPlayerMeshPos+Vector3::Create(0.0f, pSettings->m_fAimOffset, 0.0f);
			m_vKnownPlayerPos = Math::Damp(m_vKnownPlayerPos, vAimPos, g_fDeltaTime, pSettings->m_fAimDuration);
			Vector3 vDirection = (m_vKnownPlayerPos - vPosition).Normalize();
			Matrix44 lookAt;
			Camera* p3DCamera = EntityRenderer::Instance()->Get3DCamera();
			lookAt.BuildLookAtLH(Vector3::Zero, vDirection, Vector3::Create(0.0f, 0.0f, 1.0f));
			Matrix44 invLookAt;
			if(lookAt.GetInverse(invLookAt))
			{
				Matrix44 baseRotation;
				baseRotation.SetRotation(Vector3::Create(Math::PI/2.0f, 0.0f, 0.0f));
				Matrix44 rotation = baseRotation*invLookAt;
				m_Core->SetRotationMatrix(rotation);

				// shoot
				if(!m_Timer.IsRunning()	&& m_fFirstShotTimer < 0.0f)
				{
					Bullet::BulletParams params;
					Vector3 vBulletOffset = pSettings->m_vBulletOffset*m_Mesh->GetScale().Y;
					Vector3 vBulletLocalPos = rotation.TransformVect(vBulletOffset);

					params.vPosition = vPosition + vBulletLocalPos;
					params.vDirection = vDirection;
					params.fSpeed = pSettings->m_fBulletSpeed;
					params.fLife = pSettings->m_fBulletLife;
					params.fRadius = pSettings->m_fBulletSize/2.0f;
					params.damage = pSettings->m_fBulletDamage;

					SFXManager::Instance()->GetEnemyPulseManager()->AddBullet(params);

					m_Timer.Start(pSettings->m_fAimDuration);
				}
			}

			m_fFirstShotTimer -= g_fDeltaTime;
		}
	}
}

