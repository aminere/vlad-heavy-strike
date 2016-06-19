/* 

Amine Rehioui
Created: May 5th 2013

*/

#include "ShootTest.h"

#include "BasicFighter.h"

#include "EventManager.h"

#include "SequenceVisitor.h"
#include "FollowPathVisitor.h"

namespace shoot
{
	DEFINE_OBJECT(BasicFighter);
	DEFINE_OBJECT(BasicFighterSettings);
	
	//! constructor
	BasicFighterSettings::BasicFighterSettings()
		: m_fShootDuration(.5f)
		, m_fShootFrequency(.05f)
		, m_fBulletSpeed(40.0f)
		, m_fBulletLife(1.0f)
		, m_fHomingFactor(.5f)
	{
	}

	//! serializes the entity to/from a PropertyStream
	void BasicFighterSettings::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);
		stream.Serialize(PT_Float, "ShootDuration", &m_fShootDuration);
		stream.Serialize(PT_Float, "ShootFrequency", &m_fShootFrequency);
		stream.Serialize(PT_Float, "BulletSpeed", &m_fBulletSpeed);
		stream.Serialize(PT_Float, "BulletLife", &m_fBulletLife);
		stream.Serialize(PT_Float, "HomingFactor", &m_fHomingFactor);
	}

	//! constructor
	BasicFighter::BasicFighter()
		: m_fShootTimer(-1.0f)
		, m_fShootDelay(-1.0f)
		, m_CurrentCannon(0)
		, m_vDirection(Vector3::Zero)
		// properties
		, m_ForcePath(-1)
	{
	}

	//! destructor
	BasicFighter::~BasicFighter()
	{
		if(IsInitialized())
		{
			EventManager::Instance()->UnregisterListener<TriggerEvent>(this);			
		}
	}

	//! serializes the entity to/from a PropertyStream
	void BasicFighter::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);
	}

	//! called during the initialization of the entity
	void BasicFighter::Init()
	{
		super::Init();

		EventManager::Instance()->RegisterListener<TriggerEvent>(this, &HandleTriggerEvent);

		m_ShootingPoint = static_cast<Entity3D*>(GetChildByName("ShootingPoint"));
		m_LeftCannon = static_cast<Entity3D*>(GetChildByName("LeftCannon"));
		m_RightCannon = static_cast<Entity3D*>(GetChildByName("RightCannon"));
		
		s32 path = (m_ForcePath < 0) ? Random::GetInt(0, 1) : m_ForcePath;
		if(Path* pPath = static_cast<Path*>(GetChildByName(path ? "Path" : "Path2")))
		{
			if(Player* pPlayer = Player::Instance())
			{
				if(m_ForcePath < 0)
				{
					Vector3 vPosition = pPlayer->GetPosition() + GetSpawningPoint();
					SetAbsolutePosition(vPosition);
				}

				if(BasicFighterSettings* pSettings = static_cast<BasicFighterSettings*>(m_Settings.Get()))
				{
					// orient path towards player
					Vector3 vToPlayer = (pPlayer->GetMeshEntity()->GetTransformationMatrix().GetTranslation()-pPath->GetTransformationMatrix().GetTranslation()).Normalize();
					m_vDirection = Math::Lerp(Vector3::Create(0.0f, -1.0f, 0.0f), vToPlayer, pSettings->m_fHomingFactor);
					f32 fAngle = Math::ATan2(-m_vDirection.X, -m_vDirection.Y)*Math::RadToDegFactor;
					pPath->SetRotation(Vector3::Create(0.0f, 0.0f, fAngle));
				}

				if(SequenceVisitor* pSequence = m_Mesh->GetComponent<SequenceVisitor>())
				{
					FollowPathVisitor* pFollow = static_cast<FollowPathVisitor*>(pSequence->GetVisitor(0));
					pFollow->SetPath(pPath);
					pFollow->Visit(m_Mesh);
				}
			}
		}
	}

	//! called during the update of the entity	
	void BasicFighter::Update()
	{
		super::Update();

		if(m_HitPoints < 0)
		{
			return;
		}

		BasicFighterSettings* pSettings = static_cast<BasicFighterSettings*>(m_Settings.Get());
		if(m_fShootTimer < 0.0f)
		{
			if(m_Mesh->GetPosition().Y < m_ShootingPoint->GetPosition().Y)
			{
				m_fShootTimer = 0.0f;
			}
		}
		else if(m_fShootTimer < pSettings->m_fShootDuration)
		{
			if(m_fShootDelay < 0.0f)
			{
				Vector3 vBulletPos = m_CurrentCannon 
								     ? m_LeftCannon->GetTransformationMatrix().GetTranslation() 
								     : m_RightCannon->GetTransformationMatrix().GetTranslation();
				
				// shoot
				Bullet::BulletParams params;
				params.vDirection = m_vDirection;
				params.fSpeed = pSettings->m_fBulletSpeed;
				params.vPosition = vBulletPos;
				params.fLife = pSettings->m_fBulletLife;
				params.fRadius = .5f;

				SFXManager::Instance()->GetEnemyPulseManager()->AddBullet(params);

				m_CurrentCannon = 1-m_CurrentCannon;
				m_fShootDelay = pSettings->m_fShootFrequency;
			}
			m_fShootDelay -= g_fDeltaTime;
			m_fShootTimer += g_fDeltaTime;
		}
	}

	//! shoot event listeners
	void BasicFighter::HandleTriggerEvent(void* pListener, Event* pEvent)
	{
		TriggerEvent* pTrigger = static_cast<TriggerEvent*>(pEvent);
		BasicFighter* pBasicFighter = static_cast<BasicFighter*>(pListener);
		
		if(pBasicFighter->HasChild(static_cast<Entity*>(pTrigger->Target.Get())))
		{
			pBasicFighter->GetParent()->RemoveChild(pBasicFighter);
		}
	}
}

