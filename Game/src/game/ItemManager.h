/* 

Amine Rehioui
Created: April 22nd 2013

*/

#ifndef _ITEM_MANAGER_H_INCLUDED_
#define _ITEM_MANAGER_H_INCLUDED_

#include "Item.h"

namespace shoot
{
	// forwards
	class Actor;	

	//! item manager class
	class ItemManager : public Entity
	{
		DECLARE_OBJECT(ItemManager, Entity);

	public:

		//! constructor
		ItemManager();

		//! destructor
		virtual ~ItemManager();

		//! serializes the entity to/from a PropertyStream
		void Serialize(PropertyStream& stream);

		//! called during the initialization of the entity
		void Init();

		//! called during the update of the entity
		void Update();

		//! called when an actor has been killed
		void OnActorKilled(Actor* pActor);

		//! returns the instance
		inline static ItemManager* Instance()
		{
			return ms_pInstance;
		}

	private:

		//! spawns an item
		void SpawnItem(Item::E_Type type, const Vector3& vPosition);

		f32 m_fTimer;
		u32 m_KillStreakCount;
		u32 m_CurrentWeaponItem;
		Vector3 m_vItemSpawnPos;

		// properties
		f32 m_fFrequency;
		u32 m_KillStreak;
		Array< Reference<Item> > m_Items;		
			
		static ItemManager* ms_pInstance;
	};
}

#endif // _ITEM_MANAGER_H_INCLUDED_

