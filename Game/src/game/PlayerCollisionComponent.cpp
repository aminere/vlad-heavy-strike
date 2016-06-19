/* 

Amine Rehioui
Created: February 5th 2012

*/

#include "ShootTest.h"

#include "PlayerCollisionComponent.h"
#include "EnvironmentCollisionComponent.h"

#include "Item.h"

namespace shoot
{
	DEFINE_OBJECT(PlayerCollisionComponent);

	//! constructor
	PlayerCollisionComponent::PlayerCollisionComponent()
	{

	}

	//! Reads/Writes struct properties from/to a stream
	void PlayerCollisionComponent::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);
	}

	//! called when a collision with another collision component is detected
	void PlayerCollisionComponent::OnCollision(CollisionComponent* pOther, std::vector<CollisionShape::CollisionInfo>& infos)
	{
		Player* pPlayer = Player::Instance();
		if(Item* pItem = DYNAMIC_CAST(pOther->GetParent(), Item))
		{
			pPlayer->OnItemCollected(pItem);
			pItem->GetParent()->RemoveChild(pItem);
		}
		else
		{
			pPlayer->OnShot(infos[0].vIntersection, pOther, 1.0f);
		}
	}
}

