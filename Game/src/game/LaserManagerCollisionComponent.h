/* 

Amine Rehioui
Created: March 24th 2013

*/

#ifndef _LASER_MANAGER_COLLISION_COMPONENT_H_INCLUDED_
#define _LASER_MANAGER_COLLISION_COMPONENT_H_INCLUDED_

#include "CollisionComponent.h"

namespace shoot
{
	//! Laser Manager collision component class
	class LaserManagerCollisionComponent : public CollisionComponent
	{
	public:
		
		DECLARE_OBJECT(LaserManagerCollisionComponent, CollisionComponent);

		//! constructor
		LaserManagerCollisionComponent();

		//! Initializes the component
		void Init();

		//! checks collision with another component
		void CheckCollisions(CollisionComponent* pOther);

	private:

		//! called on a bullet collision
		bool OnLaserCollision(Laser* pLaser, const Vector3& vPosition, CollisionComponent* pComponent);
	};
}

#endif // _LASER_MANAGER_COLLISION_COMPONENT_H_INCLUDED_

