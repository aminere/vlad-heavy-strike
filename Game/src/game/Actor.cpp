/* 

Amine Rehioui
Created: October 10th 2011

*/

#include "ShootTest.h"

#include "MeshEntity.h"

#include "GameManager.h"
#include "PlayerCamera.h"
#include "AIManager.h"
#include "ItemManager.h"

namespace shoot
{
	DEFINE_OBJECT(Actor);
	DEFINE_OBJECT(ActorSettings);

	//! constructor
	ActorSettings::ActorSettings()
		: m_HitPoints(3)
	{
	}

	//! serializes the object to/from a PropertyStream
	void ActorSettings::Serialize(PropertyStream& stream)
	{
		stream.Serialize(PT_UInt, "HitPoints", &m_HitPoints);		
	}

	//! Constructor
	Actor::Actor()
		: m_pFSM(NULL)
		// properties
		, m_bCanDropItem(true)
		, m_bDestructible(true)
		, m_HitPoints(1.0f)
		, m_bPlayfieldCheck(true)
		, m_bImmune(false)
		, m_bIsBoss(false)
		, m_bShakeOnExplode(false)
		, m_bShakeOnShot(true)
		, m_bResetRotationOnExplode(false)
		, m_fExplosionScaleFactor(1.0f)
		, m_fDamageDist(25.0f)
		, m_bCanRespawn(true)
	{
	}

	//! Destructor
	Actor::~Actor()
	{
		sdelete(m_pFSM);
	}

	//! serializes the entity to/from a PropertyStream
	void Actor::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);

		stream.Serialize(PT_Bool, "CanDropItem", &m_bCanDropItem);
		stream.Serialize(PT_Bool, "Destructible", &m_bDestructible);
		stream.Serialize(PT_Float, "HitPoints", &m_HitPoints);
		stream.Serialize(PT_Bool, "PlayfieldCheck", &m_bPlayfieldCheck);
		stream.Serialize(PT_Bool, "Immune", &m_bImmune);
		stream.Serialize(PT_Bool, "IsBoss", &m_bIsBoss);
		stream.Serialize(PT_Bool, "ShakeOnExplode", &m_bShakeOnExplode);
		stream.Serialize(PT_Bool, "ShakeOnShot", &m_bShakeOnShot);		
		stream.Serialize(PT_Bool, "ResetRotationOnExplode", &m_bResetRotationOnExplode);
		stream.Serialize(PT_Float, "ExplosionScaleFactor", &m_fExplosionScaleFactor);
		stream.Serialize(PT_Float, "DamageDist", &m_fDamageDist);
		stream.Serialize(PT_Bool, "CanRespawn", &m_bCanRespawn);
	}

	//! called during the initialization of the entity
	void Actor::Init()		
	{
		super::Init();

		m_Mesh = static_cast<MeshEntity*>(GetChildByName("BaseMesh"));

		if(m_Settings.IsValid())
		{
			m_HitPoints = float(m_Settings->m_HitPoints);
		}

		if(m_pFSM)
		{
			m_pFSM->Init();
		}
	}

	void Actor::Update()
	{
		// remove if out of bounds and no explosion is active
		if(m_bPlayfieldCheck
		&& (this != Player::Instance())
		&& GameManager::Instance()->IsOutOfPlayfield(GetTransformationMatrix().GetTranslation())
		&& (m_HitPoints >= 0))
		{
			GetParent()->RemoveChild(this);
			AIManager::Instance()->OnEnemyDestroyed(this);
			return;
		}

		if(m_pFSM)
		{
			m_pFSM->Update();
		}
	}

	//! initializes the actors from an ActorSettings object
	void Actor::Init(ActorSettings* pSettings)
	{
		m_Settings = pSettings;

		Init();
	}

	//! called when the actor has been shot
	bool Actor::OnShot(const Vector3& vPosition, CollisionComponent* pCollisionComponent, f32 damage)
	{
		SFXManager::Instance()->OnActorShot(this, vPosition);
		
		if(!m_bImmune)
		{
			Vector3 vPlayerPos = Player::Instance()->GetMeshEntity()->GetTransformationMatrix().GetTranslation();
			Vector3 vActorPos = GetMeshEntity()->GetTransformationMatrix().GetTranslation();
			if(Math::FAbs(vPlayerPos.Y - vActorPos.Y) < m_fDamageDist)
			{
#ifndef _RETAIL_
				if(InputManager::Instance()->IsKeyPressed(InputManager::KT_A)
				&& InputManager::Instance()->IsKeyPressed(InputManager::KT_B))
				{
					m_HitPoints = -1.0f;
				}
#endif

				m_HitPoints -= damage;
				if(m_HitPoints < 0.0f)
				{
					SFXManager::Instance()->OnActorDestroyed(this);
					AIManager::Instance()->OnEnemyDestroyed(this);
					ItemManager::Instance()->OnActorKilled(this);
					return true;
				}
			}
		}

		return false;
	}

	//! returns a spawning point
	Vector3 Actor::GetSpawningPoint() const
	{
		f32 lateralLength = Player::Instance()->GetSpawnWidth();
		std::vector<u32>& spawningPoints = Player::Instance()->GetSpawningPoints();
		const f32 fSlotSize = 2.0f;
		if(spawningPoints.empty())
		{
			const u32 numSlots = u32(lateralLength/fSlotSize);
			for(u32 i=0; i<numSlots; ++i)
			{
				spawningPoints.push_back(i);
			}
		}
		u32 slot = Random::GetInt(0, spawningPoints.size()-1);
		f32 fSpawnPoint = -(lateralLength/2.0f)+1.0f+spawningPoints[slot]*fSlotSize;
		spawningPoints.erase(spawningPoints.begin()+slot);

		const f32 fSpawnForwardDisplacement = 4.0f;
		f32 forwardLength = Player::Instance()->GetMotionBounds().Size().Y+fSpawnForwardDisplacement;
		return Vector3::Create(fSpawnPoint, forwardLength, 0.0f);
	}
}

