/* 

Amine Rehioui
Created: April 23nd 2013

*/

#include "ShootTest.h"

#include "Item.h"
#include "GameManager.h"
#include "PlayerFSM.h"

namespace shoot
{
	// type literals
	const char* const g_strItemTypes[] =
	{
		"PulseUp",
		"LaserUp",
		"PelletUp",
		"Gold",
		0
	};

	DEFINE_OBJECT(Item);

	//! constructor
	Item::Item()
		: m_fTimer(8.0f)
		// properties
		, m_eType(T_PulseUp)		
	{
	}

	//! serializes the entity to/from a PropertyStream
	void Item::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);
		stream.Serialize(PT_Enum, "ItemType", &m_eType, ENUM_PARAMS1(g_strItemTypes));
	}

	//! called during the update of the entity
	void Item::Update()
	{
		if(GameManager::Instance()->IsOutOfPlayfield(GetPosition())
		|| m_fTimer < 0.0f)
		{
			GetParent()->RemoveChild(this);
			return;
		}

		if(m_eType == T_Gold)
		{
			Player* pPlayer = Player::Instance();
			if(pPlayer->GetFSM()->GetState() != PlayerFSM::State_Exploding
			&& pPlayer->GetFSM()->GetState() != PlayerFSM::State_Respawning)
			{
				static f32 fDampFactor = 10.0f;
				Vector3 vPlayerPos = pPlayer->GetMeshEntity()->GetTransformationMatrix().GetTranslation();
				f32 fDistToPlayer = (vPlayerPos - GetTransformationMatrix().GetTranslation()).GetLength();
				f32 fDampDuration = (fDistToPlayer*fDistToPlayer)/fDampFactor;
				Vector3 vPosition = Math::Damp(m_vPosition, vPlayerPos, g_fDeltaTime, fDampDuration);
				SetPosition(vPosition);
			}
		}

		m_fTimer -= g_fDeltaTime;
	}
}

