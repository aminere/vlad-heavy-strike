/* 

Amine Rehioui
Created: May 1st 2013

*/

#include "ShootTest.h"

#include "AIManager.h"

#include "PlayerCamera.h"
#include "ConfigPoint.h"
#include "TriggerSpawner.h"
#include "GameManager.h"

namespace shoot
{
	DEFINE_OBJECT(AIManager);

	//! static vars
	AIManager* AIManager::ms_pInstance = NULL;

	//! constructor
	AIManager::AIManager()
		: m_CurrentCheckpoint(0)
	{
	}

	//! destructor
	AIManager::~AIManager()
	{
		ms_pInstance = NULL;
	}

	//! serializes the entity to/from a PropertyStream
	void AIManager::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);

		stream.SerializeArray("Checkpoints", &m_Checkpoints, PT_Link);
	}

	//! called during the initialization of the entity
	void AIManager::Init()
	{
		super::Init();
		SHOOT_ASSERT(!ms_pInstance, "Multiple AIManager instances detected");
		ms_pInstance = this;

		for(u32 i=0; i<m_Checkpoints.GetSize(); ++i)
		{
			m_Checkpoints[i].Init(this);
		}

#ifdef SHOOT_EDITOR
		if(Player* pPlayer = Player::Instance())
		{
			for(s32 i=s32(m_Checkpoints.GetSize())-1; i >= 0; --i)
			{
				if(m_Checkpoints[i].Get() && m_Checkpoints[i]->IsReached())
				{
					m_CurrentCheckpoint = i;
					Player::Instance()->SetBaseSpeed(GetCurrentCheckpoint()->GetBaseSpeed());					
					break;
				}
			}
		}
#endif
	}

	//! called during the update of the entity
	void AIManager::Update()
	{
	}

	//! called when an actor has been spawned
	void AIManager::OnEnemySpawned(Actor* pActor)
	{
		if(pActor->IsDestructible())
		{
			m_lDestructibleEnemies.push_back(Handle<Actor>(pActor));
		}
		else
		{
			m_lEnemies.push_back(Handle<Actor>(pActor));
		}
	}

	//! called when an actor has been spawned
	void AIManager::OnEnemySpawned(Actor* pActor, TriggerSpawner* pTrigger)
	{
		OnEnemySpawned(pActor);

		Handle<TriggerSpawner> trigger(pTrigger);
		SHOOT_ASSERT(std::find(m_lTriggers.begin(), m_lTriggers.end(), trigger) == m_lTriggers.end(), "Trying to register a trigger spawner twice");
		m_lTriggers.push_back(trigger);
	}

	//! called when an enemy has been destroyed
	void AIManager::OnEnemyDestroyed(Actor* pActor)
	{
		std::list< Handle<Actor> >& list = pActor->IsDestructible() ? m_lDestructibleEnemies : m_lEnemies;
		Handle<Actor> actor(pActor);
		std::list< Handle<Actor> >::iterator it = std::find(list.begin(), list.end(), actor);
		if(it != list.end())
		{
			list.erase(it);
		}		
	}

	//! called on player respawn
	void AIManager::OnRespawn()
	{
		for(std::list< Handle<Actor> >::iterator it = m_lDestructibleEnemies.begin(); it != m_lDestructibleEnemies.end(); ++it)
		{
			Actor* pActor = (*it).Get();
			if(pActor && pActor->GetCanRespawn())
			{
				Engine::Instance()->RemoveEntity(pActor);				
			}			
		}

		for(std::list< Handle<Actor> >::iterator it = m_lEnemies.begin(); it != m_lEnemies.end(); ++it)
		{
			Actor* pActor = (*it).Get();
			if(pActor && pActor->GetCanRespawn())
			{
				Engine::Instance()->RemoveEntity(pActor);
			}
		}

		for(std::list< Handle<TriggerSpawner> >::iterator it = m_lTriggers.begin(); it != m_lTriggers.end(); ++it)
		{
			if(TriggerSpawner* pTrigger = (*it).Get())
			{
				pTrigger->Reactivate();
			}
		}

		m_lDestructibleEnemies.clear();
		m_lEnemies.clear();
		m_lTriggers.clear();
				
		// find last possible respawn spot
		while(m_CurrentCheckpoint > 0 && !m_Checkpoints[m_CurrentCheckpoint].Get()->CanRespawnHere())
		{
			--m_CurrentCheckpoint;
		}

		ConfigPoint* pCurrentCP = m_Checkpoints[m_CurrentCheckpoint].Get();
		Player::Instance()->SetPosition(pCurrentCP->GetTransformationMatrix().GetTranslation());
		Player::Instance()->SetSkyBoxRotation(pCurrentCP->GetSkyBoxRotation());
		s32 restoredScore = pCurrentCP->GetScore()-1;
		restoredScore = Math::Clamp(restoredScore, 0, restoredScore);
		pCurrentCP->SetScore(restoredScore);
		GameManager::Instance()->SetScore(pCurrentCP->GetScore());
	}

	//! returns true if there are enemies to shoot at
	bool AIManager::IsEnnemyDetected() const
	{
		for(std::list< Handle<Actor> >::const_iterator it = m_lDestructibleEnemies.begin(); it != m_lDestructibleEnemies.end(); ++it)
		{
			if(Actor* pActor = (*it).Get())
			{
				Vector3 vPlayerBasePos = Player::Instance()->GetPosition();
				if(vPlayerBasePos.Y < pActor->GetTransformationMatrix().GetTranslation().Y)				
				{
					return true;
				}
			}
		}
		return false;
	}

	//! returns the next check point
	ConfigPoint* AIManager::GetNextCheckpoint() const
	{
		if(s32(m_CurrentCheckpoint) < s32(m_Checkpoints.GetSize())-1)
		{
			return m_Checkpoints[m_CurrentCheckpoint+1].Get();
		}
		return NULL;
	}

	//! increments the current checkpoint
	void AIManager::IncrCheckpoint()
	{
		++m_CurrentCheckpoint;
		m_Checkpoints[m_CurrentCheckpoint]->Apply();
	}
}

