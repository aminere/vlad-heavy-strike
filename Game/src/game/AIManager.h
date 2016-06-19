/* 

Amine Rehioui
Created: May 1st 2013

*/

#ifndef _AI_MANAGER_H_INCLUDED_
#define _AI_MANAGER_H_INCLUDED_

#include "ConfigPoint.h"

namespace shoot
{
	// forwards
	class Actor;	
	class TriggerSpawner;

	//! item manager class
	class AIManager : public Entity
	{
		DECLARE_OBJECT(AIManager, Entity);

	public:

		//! constructor
		AIManager();

		//! destructor
		virtual ~AIManager();

		//! serializes the entity to/from a PropertyStream
		void Serialize(PropertyStream& stream);

		//! called during the initialization of the entity
		void Init();

		//! called during the update of the entity
		void Update();

		//! called when an actor has been spawned
		void OnEnemySpawned(Actor* pActor);

		//! called when an actor has been spawned
		void OnEnemySpawned(Actor* pActor, TriggerSpawner* pTrigger);

		//! called when an enemy has been destroyed
		void OnEnemyDestroyed(Actor* pActor);

		//! returns the list of enemies
		std::list< Handle<Actor> >& GetEnemies() { return m_lDestructibleEnemies; }

		//! returns true if there are enemies to shoot at
		bool IsEnnemyDetected() const;

		//! called on player respawn
		void OnRespawn();

		//! returns the current check point
		inline ConfigPoint* GetCurrentCheckpoint() const { return m_Checkpoints[m_CurrentCheckpoint].Get(); }

		//! returns the next check point
		ConfigPoint* GetNextCheckpoint() const;

		//! increments the current checkpoint
		void IncrCheckpoint();

		//! returns the instance
		inline static AIManager* Instance()
		{
			return ms_pInstance;
		}

	private:

		std::list< Handle<Actor> > m_lDestructibleEnemies;	
		std::list< Handle<Actor> > m_lEnemies;
		std::list< Handle<TriggerSpawner> > m_lTriggers;
		u32 m_CurrentCheckpoint;

		static AIManager* ms_pInstance;

		// properties
		Array< Link<ConfigPoint> > m_Checkpoints;
	};
}

#endif // _AI_MANAGER_H_INCLUDED_

