/* 

Amine Rehioui
Created: November 05th 2011

*/

#include "ShootTest.h"

#include "AreaSpawner.h"

#include "AIManager.h"

namespace shoot
{
	DEFINE_OBJECT(AreaSpawner);

	//! Constructor
	AreaSpawner::AreaSpawner()
		: m_fTimer(99.0f)
		, m_bActive(false)
		// properties
		, m_fRadius(35.0f)
		, m_fFrequency(1.0f)
	{
	}

	//! serializes the entity to/from a PropertyStream
	void AreaSpawner::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);

		stream.SerializeReference("Actor", &m_Actor);
		stream.SerializeReference("ActorSettings", &m_ActorSettings);
		stream.Serialize(PT_Float, "Frequency", &m_fFrequency);
		stream.Serialize(PT_Float, "Radius", &m_fRadius);
	}

	//! called during the update of the entity	
	void AreaSpawner::Update()
	{
		Vector3 vPlayerBasePos = Player::Instance()->GetPosition();
		if(Math::FAbs(vPlayerBasePos.Y - GetTransformationMatrix().GetTranslation().Y) < m_fRadius)
		{
			if(!m_bActive)
			{
				m_fTimer = 99.0f;
				m_bActive = true;
			}

			Spawn(m_ActorSettings);
		}
		else
		{
			m_bActive = false;
		}
	}

	//! spawns the actors
	void AreaSpawner::Spawn(ActorSettings* pSettings)
	{
		if(m_fTimer > m_fFrequency)
		{
			Actor* pActor = static_cast<Actor*>(m_Actor.Get()->Copy());
			GetRoot()->AddChild(pActor);
			pActor->Init(pSettings);

			m_fTimer = 0.0f;
			AIManager::Instance()->OnEnemySpawned(pActor);
		}
		m_fTimer += g_fDeltaTime;
	}

	//! returns the bounding box
	AABBox3D AreaSpawner::GetBoundingBox() const
	{
		Vector3 vPosition = const_cast<AreaSpawner*>(this)->GetTransformationMatrix().GetTranslation();
		return AABBox3D(Vector3::Create(vPosition.X-m_fRadius, vPosition.Y-m_fRadius, -.5f),
					    Vector3::Create(vPosition.X+m_fRadius, vPosition.Y+m_fRadius, .5f));
	}
}

