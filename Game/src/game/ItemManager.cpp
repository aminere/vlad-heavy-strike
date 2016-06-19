/* 

Amine Rehioui
Created: April 22nd 2013

*/

#include "ShootTest.h"

#include "ItemManager.h"

#include "GameManager.h"

namespace shoot
{
	DEFINE_OBJECT(ItemManager);

	//! static vars
	ItemManager* ItemManager::ms_pInstance = NULL;

	//! constructor
	ItemManager::ItemManager()
		: m_fTimer(0.0f)
		, m_KillStreakCount(0)
		, m_CurrentWeaponItem(0)
		, m_vItemSpawnPos(Vector3::Zero)
		// properties
		, m_fFrequency(3.0f)
		, m_KillStreak(4)
	{
	}

	//! destructor
	ItemManager::~ItemManager()
	{
		ms_pInstance = NULL;
	}

	//! serializes the entity to/from a PropertyStream
	void ItemManager::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);

		stream.Serialize(PT_Float, "Frequency", &m_fFrequency);
		stream.Serialize(PT_UInt, "KillStreak", &m_KillStreak);
		stream.SerializeArray("Items", &m_Items, PT_Reference);
	}

	//! called during the initialization of the entity
	void ItemManager::Init()
	{
		super::Init();
		SHOOT_ASSERT(!ms_pInstance, "Multiple ItemManager instances detected");
		ms_pInstance = this;
	}

	//! called during the update of the entity
	void ItemManager::Update()
	{
		if(m_KillStreakCount >= m_KillStreak
		&& m_fTimer > m_fFrequency)
		{
			SpawnItem(static_cast<Item::E_Type>(m_CurrentWeaponItem), m_vItemSpawnPos);			
			m_CurrentWeaponItem = (m_CurrentWeaponItem+1)%Item::T_Gold;

			m_KillStreakCount = 0;
			m_fTimer = 0.0f;
		}
		else
		{
			m_fTimer += g_fDeltaTime;
		}
	}

	//! spawns an item
	void ItemManager::SpawnItem(Item::E_Type type, const Vector3& vPosition)
	{
		Item* pItem = static_cast<Item*>(m_Items[type]->Copy());
		pItem->SetPosition(vPosition);
		GetRoot()->AddChild(pItem);
		pItem->Init();
	}

	//! called when an actor has been killed
	void ItemManager::OnActorKilled(Actor* pActor)
	{ 
		if(pActor->CanDropItem())
		{
			m_vItemSpawnPos = pActor->GetMeshEntity()->GetTransformationMatrix().GetTranslation();
			m_vItemSpawnPos.Z = 0.0f;
			++m_KillStreakCount;

			SpawnItem(Item::T_Gold, m_vItemSpawnPos);
		}
		else
		{
			GameManager::Instance()->IncrScore(1);
		}

		//else
		//{
		//	Player* pPlayer = Player::Instance();
		//	f32 fForwardDistance = pPlayer->GetMotionBounds().Size().Y;
		//	m_vItemSpawnPos = pPlayer->GetPosition() + Vector3::Create(0.0f, fForwardDistance/2.0f, 0.0f);
		//}
	}
}

