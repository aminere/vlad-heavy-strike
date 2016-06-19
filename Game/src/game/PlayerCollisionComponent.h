/* 

Amine Rehioui
Created: February 5th 2012

*/

#ifndef _PLAYER_COLLISION_COMPONENT_H_INCLUDED_
#define _PLAYER_COLLISION_COMPONENT_H_INCLUDED_

#include "RigidBodyComponent.h"

namespace shoot
{
	//! Player collision component class
	class PlayerCollisionComponent : public RigidBodyComponent
	{
	public:
		
		DECLARE_OBJECT(PlayerCollisionComponent, RigidBodyComponent);

		//! constructor
		PlayerCollisionComponent();

		//! Reads/Writes struct properties from/to a stream
		void Serialize(PropertyStream& stream);

		//! called when a collision with another collision component is detected
		void OnCollision(CollisionComponent* pOther, std::vector<CollisionShape::CollisionInfo>& infos);
	};
}

#endif // _PLAYER_COLLISION_COMPONENT_H_INCLUDED_

