/* 

Amine Rehioui
Created: October 26th 2013

*/

#include "ShootTest.h"

#include "KamikazeTower.h"

#include "MeshEntity.h"

#include "AIManager.h"
#include "ItemManager.h"
#include "GameManager.h"

namespace shoot
{
	DEFINE_OBJECT(KamikazeTower);
	DEFINE_OBJECT(KamikazeTowerSettings);

	//! constructor
	KamikazeTowerSettings::KamikazeTowerSettings()
		: m_fHomingFactor(1.0f)
		, m_fHomingDuration(1.5f)
		, m_fSpeed(25.0f)
	{
	}

	//! serializes the entity to/from a PropertyStream
	void KamikazeTowerSettings::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);

		stream.Serialize(PT_Float, "HomingFactor", &m_fHomingFactor);
		stream.Serialize(PT_Float, "HomingDuration", &m_fHomingDuration);
		stream.Serialize(PT_Float, "Speed", &m_fSpeed);		
	}

	//! constructor
	KamikazeTower::KamikazeTower()
		: m_bKamikazeMode(false)
		, m_vDirection(Vector3::Create(0.0f, -1.0f, 0.0f))
		, m_fTimer(1.5f)
	{
	}

	//! called during the initialization of the entity
	void KamikazeTower::Init()
	{
		super::Init();

		if(KamikazeTowerSettings* pSettings = static_cast<KamikazeTowerSettings*>(m_Settings.Get()))
		{
			m_fTimer = pSettings->m_fHomingDuration;
		}

		m_Platform = static_cast<Entity3D*>(GetChildByName("Platform"));
	}

	//! called during the update of the entity	
	void KamikazeTower::Update()
	{
		super::Update();

		if(m_HitPoints < 0
		&& GameManager::Instance()->IsOutOfPlayfield(m_Platform->GetTransformationMatrix().GetTranslation()))
		{
			GetParent()->RemoveChild(this);
			return;
		}

		if(m_bKamikazeMode)
		{
			KamikazeTowerSettings* pSettings = static_cast<KamikazeTowerSettings*>(m_Settings.Get());

			if(m_fTimer > 0.0f)
			{
				Vector3 vPosition = m_Platform->GetTransformationMatrix().GetTranslation();
				Vector3 vPlayerMeshPos = Player::Instance()->GetMeshEntity()->GetTransformationMatrix().GetTranslation();

				f32 fInterpolator = pSettings->m_fHomingFactor*g_fDeltaTime;
				fInterpolator = Math::Clamp(fInterpolator, 0.0f, 1.0f);
				Vector3 vDirectionToPlayer = (vPlayerMeshPos-vPosition).Normalize();
				m_vDirection = ((vDirectionToPlayer-m_vDirection)*fInterpolator + m_vDirection).Normalize();
				f32 fAngle = Math::ATan2(-m_vDirection.X, -m_vDirection.Y)*Math::RadToDegFactor;
				m_Platform->SetRotation(Vector3::Create(0.0f, 0.0f, fAngle));

				m_fTimer -= g_fDeltaTime;
			}		

			m_Platform->Translate(m_vDirection*pSettings->m_fSpeed*g_fDeltaTime);
		}
	}

	//! called when the actor has been shot
	bool KamikazeTower::OnShot(const Vector3& vPosition, CollisionComponent* pCollisionComponent, f32 damage)
	{
		SFXManager::Instance()->OnActorShot(this, vPosition);

		if(!m_bKamikazeMode)
		{
			Vector3 vPlayerPos = Player::Instance()->GetMeshEntity()->GetTransformationMatrix().GetTranslation();
			Vector3 vActorPos = GetMeshEntity()->GetTransformationMatrix().GetTranslation();
			if(Math::FAbs(vPlayerPos.Y - vActorPos.Y) < m_fDamageDist)
			{
				m_HitPoints -= damage;

				if(m_HitPoints < 0)
				{
					SFXManager::Instance()->Explode(this);
					AIManager::Instance()->OnEnemyDestroyed(this);
					ItemManager::Instance()->OnActorKilled(this);

					m_bKamikazeMode = true;
					return true;
				}
			}
		}

		return false;
	}
}

