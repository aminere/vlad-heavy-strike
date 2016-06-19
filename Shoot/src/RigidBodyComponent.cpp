/*	

Amine Rehioui
Created: January 14th 2012

*/

#include "Shoot.h"

#include "RigidBodyComponent.h"



namespace shoot
{
	DEFINE_OBJECT(RigidBodyComponent);
	
	//! constructor
	RigidBodyComponent::RigidBodyComponent()
		: m_vVelocity(Vector3::Zero)
		// properties
		, m_fMass(1.0f)		
	{		
	}

	//! destructor
	RigidBodyComponent::~RigidBodyComponent()
	{		
	}

	//! Reads/Writes struct properties from/to a stream
	void RigidBodyComponent::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);

		stream.Serialize(PT_Float, "Mass", &m_fMass);
		stream.SerializeArray("Shapes", &m_aShapes, PT_Reference);
	}

	//! Initializes the component
	void RigidBodyComponent::Init()
	{
		super::Init();

		// TODO: restrict component type depending on entity, and enforce it in the editor
		SHOOT_ASSERT(m_pParent->IsA(Entity3D::TypeID), "CollisionComponent parent must be an Entity3D");
	}

	//! checks collision with another component
	void RigidBodyComponent::CheckCollisions(CollisionComponent* pOther)
	{
		if(RigidBodyComponent* pOtherRigidBody = DYNAMIC_CAST(pOther, RigidBodyComponent))
		{
			std::vector<CollisionShape::CollisionInfo> infos;

			Entity3D* pParent = static_cast<Entity3D*>(GetParent());
			Entity3D* pOtherParent = static_cast<Entity3D*>(pOther->GetParent());
			CollisionShapeTransform component1Transform(pParent->GetTransformationMatrix(), pParent->GetAbsoluteScale());
			CollisionShapeTransform component2Transform(pOtherParent->GetTransformationMatrix(), pOtherParent->GetAbsoluteScale());

			for(u32 i=0; i<GetShapeCount(); ++i)
			{
				if(CollisionShape* pShape1 = GetShape(i))
				{
					for(u32 j=0; j<pOtherRigidBody->GetShapeCount(); ++j)
					{
						if(CollisionShape* pShape2 = pOtherRigidBody->GetShape(j))
						{
							CollisionShape::CollisionInfo info;
							if(pShape1->CollidesWith(pShape2, component1Transform, component2Transform, info))
							{
								infos.push_back(info);
							}
						}
					}
				}
			}

			if(!infos.empty())
			{
				OnCollision(pOther, infos);
				pOther->OnCollision(this, infos);
			}
		}
	}
}

