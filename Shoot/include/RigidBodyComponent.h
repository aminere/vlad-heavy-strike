/* 

Amine Rehioui
Created: January 14th 2012

*/

#ifndef _RIGID_BODY_COMPONENT_H_INCLUDED_
#define _RIGID_BODY_COMPONENT_H_INCLUDED_

#include "CollisionComponent.h"

namespace shoot
{
	//! RigidBody Component class
	class RigidBodyComponent : public CollisionComponent
	{
		DECLARE_OBJECT(RigidBodyComponent, CollisionComponent);

	public:

		//! constructor
		RigidBodyComponent();

		//! destructor
		virtual ~RigidBodyComponent();

		//! Reads/Writes struct properties from/to a stream
		virtual void Serialize(PropertyStream& stream);

		//! Initializes the component
		virtual void Init();

		//! checks collision with another component
		virtual void CheckCollisions(CollisionComponent* pOther);

		//! returns the number of collision shapes
		size_t GetShapeCount() const { return m_aShapes.GetSize(); }

		//! returns a collision shape
		CollisionShape* GetShape(u32 index) { return m_aShapes[index].Get(); }

		//! sets the velocity
		inline void SetVelocity(const Vector3& vVelocity) { m_vVelocity = vVelocity; }

		//! returns the velocity
		inline const Vector3& GetVelocity() const { return m_vVelocity; }

	private:
		
		Vector3 m_vVelocity;

		// properties
		f32 m_fMass;
		Array< Reference<CollisionShape> > m_aShapes;
	};
}

#endif // _RIGID_BODY_COMPONENT_H_INCLUDED_

