/* 

Amine Rehioui
Created: March 12th 2013

*/

#ifndef _ENVIRONMENT_COLLISION_COMPONENT_H_INCLUDED_
#define _ENVIRONMENT_COLLISION_COMPONENT_H_INCLUDED_

#include "CollisionComponent.h"

#include "MeshResource.h"

namespace shoot
{
	//! EnvironmentCollisionComponent class
	class EnvironmentCollisionComponent : public CollisionComponent
	{
		DECLARE_OBJECT(EnvironmentCollisionComponent, CollisionComponent);

	public:

		//! constructor
		EnvironmentCollisionComponent();

		//! destructor
		virtual ~EnvironmentCollisionComponent();

		//! Reads/Writes struct properties from/to a stream
		virtual void Serialize(PropertyStream& stream);

		//! Initializes the component
		virtual void Init();

		//! checks collision with another component
		void CheckCollisions(CollisionComponent* pOther);

		//! Checks for a collision against the environment
		bool CheckCollision(const Vector3& vPosition, const Vector3& vVelocity, f32 fRadius, Vector3* pIntersection = NULL);

		//! returns the mesh
		inline MeshResource* GetMesh() const { return m_Mesh; }

#ifdef SHOOT_EDITOR
		//! CollisionInfo
		struct CollisionInfo
		{
			Vector3 vPosition;
			f32 fLife;
		};

		std::list<CollisionInfo> m_aCollisionInfos;
#endif // SHOOT_EDITOR

	private:

		// properties
		Reference<MeshResource> m_Mesh;
		bool m_bDynamic;
	};
}

#endif // _ENVIRONMENT_COLLISION_COMPONENT_H_INCLUDED_

