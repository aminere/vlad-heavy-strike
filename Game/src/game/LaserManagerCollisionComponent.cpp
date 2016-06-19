/* 

Amine Rehioui
Created: March 24th 2013

*/

#include "ShootTest.h"

#include "LaserManagerCollisionComponent.h"

#include "LaserManager.h"

#include "CollisionUtils.h"

#include "EnvironmentCollisionComponent.h"

#include "RigidBodyComponent.h"

namespace shoot
{
	DEFINE_OBJECT(LaserManagerCollisionComponent);

	//! constructor
	LaserManagerCollisionComponent::LaserManagerCollisionComponent()
	{
		m_Priority = 2;
	}

	//! Initializes the component
	void LaserManagerCollisionComponent::Init()
	{
		super::Init();

		// TODO: restrict component type depending on entity, and enforce it in the editor
		SHOOT_ASSERT(m_pParent->IsA(LaserManager::TypeID), "LaserManagerCollisionComponent parent must be a LaserManager");
	}

	//! checks collision with another component
	void LaserManagerCollisionComponent::CheckCollisions(CollisionComponent* pOther)
	{
		LaserManager* pLaserManager = static_cast<LaserManager*>(GetParent());
		const std::list<Laser*>& lasers = pLaserManager->GetLasers();

		if(EnvironmentCollisionComponent* pEnvCol = DYNAMIC_CAST(pOther, EnvironmentCollisionComponent))
		{
			for(std::list<Laser*>::const_iterator it = lasers.begin(); it != lasers.end(); ++it)
			{
				Laser* pLaser = (*it);
				if(pLaser->bStatic)
				{
					continue;
				}

				Vector3 vEnd = pLaser->vStart + pLaser->vDir * pLaser->fLength;
				if(pEnvCol->CheckCollision(vEnd, pLaser->vDir*pLaser->fSpeed*g_fDeltaTime, m_fCollisionRadius))
				{
					if(OnLaserCollision(pLaser, vEnd, pOther))
					{
						break;
					}
				}
			}
		}
		else if(RigidBodyComponent* pRigidBody = DYNAMIC_CAST(pOther, RigidBodyComponent))
		{
			Entity3D* pOtherParent = static_cast<Entity3D*>(pOther->GetParent());
			CollisionShapeTransform otherTransform(pOtherParent->GetTransformationMatrix(), pOtherParent->GetAbsoluteScale());

			for(std::list<Laser*>::const_iterator it = lasers.begin(); it != lasers.end(); ++it)
			{
				Laser* pLaser = (*it);
				if(!pLaser->bStatic && pLaser->fLife <= 0.0f && pOther->GetParent()->GetAncestor<Actor>())
				{
					continue;
				}

				for(u32 i=0; i<pRigidBody->GetShapeCount(); ++i)
				{
					if(CollisionShape* pShape = pRigidBody->GetShape(i))
					{
						bool bCollided = false;
						Vector3 vEnd = pLaser->vStart + pLaser->vDir * pLaser->fLength;
						Vector3 vIntersection = vEnd;
						f32 fDistToIntersection = -1.0f;

						if(CollisionShapeBox* pBox = DYNAMIC_CAST(pShape, CollisionShapeBox))
						{
							bCollided = CollisionUtils::AABBox3DIntersectsWithSphere(pBox->GetBox()*otherTransform.GetScale()+otherTransform.GetMatrix().GetTranslation(),
																					 vEnd+pLaser->vDir*pLaser->fSpeed*g_fDeltaTime, m_fCollisionRadius);
							fDistToIntersection = pLaser->fLength;
						}
						else if(CollisionShapeSphere* pSphere = DYNAMIC_CAST(pShape, CollisionShapeSphere))
						{
							bCollided = CollisionUtils::SphereIntersectsWithRay(pSphere->GetCenter(otherTransform.GetMatrix()), pSphere->GetRadius()*otherTransform.GetScale().X, pLaser->vStart, pLaser->vDir, &(pLaser->fLength), &fDistToIntersection);
							if(!bCollided)
							{
								Vector3 vLateralAxe = (*it)->vDir.CrossProduct(EntityRenderer::Instance()->Get3DCamera()->GetUpVector());
								Vector3 vStartLeft = pLaser->vStart - vLateralAxe*pLaser->fThickness;
								bCollided = CollisionUtils::SphereIntersectsWithRay(pSphere->GetCenter(otherTransform.GetMatrix()), pSphere->GetRadius()*otherTransform.GetScale().X, vStartLeft, pLaser->vDir, &(pLaser->fLength), &fDistToIntersection);
								if(!bCollided)
								{
									Vector3 vStartRight = pLaser->vStart + vLateralAxe*pLaser->fThickness;
									bCollided = CollisionUtils::SphereIntersectsWithRay(pSphere->GetCenter(otherTransform.GetMatrix()), pSphere->GetRadius()*otherTransform.GetScale().X, vStartRight, pLaser->vDir, &(pLaser->fLength), &fDistToIntersection);
								}
							}
						}

						if(bCollided)
						{
							if(OnLaserCollision(pLaser, pLaser->vStart + pLaser->vDir*fDistToIntersection, pOther))
							{
								return;
							}
						}
					}
				}
			}
		}
	}

	//! called on a bullet collision
	bool LaserManagerCollisionComponent::OnLaserCollision(Laser* pLaser, const Vector3& vPosition, CollisionComponent* pComponent)
	{
		if(pLaser->fLife > 0.0f)
		{
			pLaser->fLife = 0.0f;
		}

		if(Actor* pActor = pComponent->GetParent()->GetAncestor<Actor>())
		{			
			if(pActor->IsImmune())
			{
				SFXManager::Instance()->OnLaserCollided(vPosition);
			}

			if(pActor->OnShot(vPosition, this, pLaser->damage))
			{
				pComponent->SetEnabled(false);
				return true;
			}
		}
		else
		{
			SFXManager::Instance()->OnLaserCollided(vPosition);
		}

		return false;
	}
}

