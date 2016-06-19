/* 

Amine Rehioui
Created: February 5th 2012

*/

#ifndef _BULLET_MANAGER_COLLISION_COMPONENT_H_INCLUDED_
#define _BULLET_MANAGER_COLLISION_COMPONENT_H_INCLUDED_

#include "CollisionComponent.h"

namespace shoot
{
	//! Bullet Manager collision component class
	class BulletManagerCollisionComponent : public CollisionComponent
	{
	public:
		
		DECLARE_OBJECT(BulletManagerCollisionComponent, CollisionComponent);

		//! constructor
		BulletManagerCollisionComponent();

		//! Initializes the component
		void Init();

		//! checks collision with another component
		void CheckCollisions(CollisionComponent* pOther);

	private:

		//! called on a bullet collision
		bool OnBulletCollision(Bullet* pBullet, CollisionComponent* pComponent);
	};
}

#endif // _BULLET_MANAGER_COLLISION_COMPONENT_H_INCLUDED_

