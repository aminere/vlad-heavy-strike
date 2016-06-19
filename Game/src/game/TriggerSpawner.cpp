/* 

Amine Rehioui
Created: March 3rd 2013

*/

#include "ShootTest.h"

#include "TriggerSpawner.h"

#include "AIManager.h"

namespace shoot
{
	DEFINE_OBJECT(TriggerSpawner);

	//! Constructor
	TriggerSpawner::TriggerSpawner()
		: m_bActive(true)
		// properties
		, m_fDistToPlayer(40.0f)
		, m_bCanReactivate(true)
	{
	}

	//! serializes the entity to/from a PropertyStream
	void TriggerSpawner::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);

		stream.SerializeReference("Actor", &m_Actor);
		stream.SerializeReference("ActorSettings", &m_ActorSettings);

		stream.Serialize(PT_Float, "DistToPlayer", &m_fDistToPlayer);
		stream.Serialize(PT_Bool, "CanReactivate", &m_bCanReactivate);
	}

	//! called during the update of the entity	
	void TriggerSpawner::Update()
	{
		if(!m_bActive)
		{
			return;
		}

		Vector3 vPlayerBasePos = Player::Instance()->GetPosition();
		if(vPlayerBasePos.Y > (GetTransformationMatrix().GetTranslation().Y-m_fDistToPlayer))
		{
			// spawn
			Actor* pActor = static_cast<Actor*>(m_Actor.Get()->Copy());
			AddChild(pActor);

			pActor->SetPosition(Vector3::Zero);
			pActor->Init(m_ActorSettings);

			m_bActive = false;
			AIManager::Instance()->OnEnemySpawned(pActor, this);
		}
	}

	//! returns the bounding box
	AABBox3D TriggerSpawner::GetBoundingBox() const
	{
		Vector3 vPosition = const_cast<TriggerSpawner*>(this)->GetTransformationMatrix().GetTranslation();
		return AABBox3D(Vector3::Create(vPosition.X-.5f, vPosition.Y-m_fDistToPlayer-.5f, -.5f),
						Vector3::Create(vPosition.X+.5f, vPosition.Y+.5f, .5f));
	}

	//! attempts to re-activates the trigger
	void TriggerSpawner::Reactivate()
	{
		if(m_bCanReactivate)
		{
			SetActive(true);
		}
	}
}

