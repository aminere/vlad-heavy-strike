/* 

Amine Rehioui
Created: August 5th 2013

*/

#include "ShootTest.h"

#include "Squad.h"

#include "AIManager.h"

namespace shoot
{
	DEFINE_OBJECT(Squad);
	DEFINE_OBJECT(SquadSettings);

	//! serializes the entity to/from a PropertyStream
	void Squad::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);
	}

	//! called during the initialization of the entity
	void Squad::Init()
	{
		if(ActorSettings* pSettings = m_Settings.Get())
		{
			SHOOT_ASSERT(pSettings->IsA(SquadSettings::TypeID), "Settings not of type SquadSettings");

			Vector3 vPosition = Player::Instance()->GetPosition() + GetSpawningPoint();
			SetAbsolutePosition(vPosition);

			s32 path = Random::GetInt(0, 1);
			GetChildrenByType("BasicFighter", m_Fighters);
			for(u32 i=0; i<m_Fighters.size(); ++i)
			{
				BasicFighter* pFighter = static_cast<BasicFighter*>(m_Fighters[i].Get());
				pFighter->SetForcePath(path);
				((Actor*)pFighter)->Init(pSettings);
				AIManager::Instance()->OnEnemySpawned(pFighter);
			}

			m_bInitialized = true;
		}
	}
}

