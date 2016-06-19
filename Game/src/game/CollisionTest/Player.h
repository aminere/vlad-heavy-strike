/* 

Amine Rehioui
Created: December 18th 2010

*/

#ifndef _PLAYER_H_INCLUDED_
#define _PLAYER_H_INCLUDED_

#include "MeshEntity.h"

#include "PostRenderer.h"

#include "Plane.h"

class Player : public shoot::MeshEntity
			 , public shoot::Post3DRenderer
{
	typedef shoot::MeshEntity super;

public:

	//! Macro to enable registration whithin the factory
	DEFINE_ENTITY(Player);

	//! Constructor
	Player();

	//! serializes the entity to/from a PropertyStream
	void Serialize(shoot::PropertyStream& stream);

	//! called during the initialization of the entity
	void Init();

	//! called during the update of the entity
	/** \param fDeltaTime: time since last Update */
	void Update(shoot::f32 fDeltaTime);

	//! called during the rendering of the entity
	void Render();

	// called during the unitialization of the entity
	void Uninit();

private:

	//! custom Post 3D rendering
	void RenderPost3D();

	shoot::MeshEntity* m_pEnvironment;
	shoot::MeshEntity* m_pIntersection1;

	shoot::Vector3D m_vVelocity;
	shoot::Vector3D m_vSlidingDirection;
	shoot::s32 m_iCurrentCollidingTriangle;

	//! checks if there is collision
	void CheckCollision(const shoot::Vector3D& vPosition, const shoot::Vector3D& vVelocity,
					    shoot::Vector3D& vNewPosition, shoot::Vector3D& vNewVelocity);

	//! returns true if the given triangle is a wall
	bool IsWall(const shoot::Plane& plane);

	//! returns true if the player is in the air
	bool IsInTheAir() const;

	//! returns a sliding direction along a plane
	shoot::Vector3D GetSlidingDirection(const shoot::Plane& plane);

	//! returns a new position not colliding with the given plane
	shoot::Vector3D GetNewPosition(const shoot::Vector3D& vPosition, const shoot::Vector3D& vIntersection, const shoot::Plane& plane);

	// properties
	shoot::f32 m_fGravity;
	shoot::f32 m_fMaxSpeed;	
	shoot::f32 m_fAcceleration;	
	shoot::f32 m_fRadius;
};

#endif // _PLAYER_H_INCLUDED_