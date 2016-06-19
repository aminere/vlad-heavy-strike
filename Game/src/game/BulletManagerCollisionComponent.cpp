/* 

Amine Rehioui
Created: February 5th 2012

*/

#include "ShootTest.h"

#include "BulletManagerCollisionComponent.h"

#include "BulletManager.h"

#include "CollisionUtils.h"

#include "EnvironmentCollisionComponent.h"

#include "RigidBodyComponent.h"

namespace shoot
{
	DEFINE_OBJECT(BulletManagerCollisionComponent);

	//! constructor
	BulletManagerCollisionComponent::BulletManagerCollisionComponent()
	{
		m_Priority = 2;
	}

	//! Initializes the component
	void BulletManagerCollisionComponent::Init()
	{
		super::Init();

		// TODO: restrict component type depending on entity, and enforce it in the editor
		SHOOT_ASSERT(m_pParent->IsA(BulletManager::TypeID), "BulletManagerCollisionComponent parent must be a BulletManager");
	}

	//! checks collision with another component
	void BulletManagerCollisionComponent::CheckCollisions(CollisionComponent* pOther)
	{
		BulletManager* pBulletManager = static_cast<BulletManager*>(GetParent());
		const std::list<Bullet*>& bullets = pBulletManager->GetBullets();

		if(EnvironmentCollisionComponent* pEnvCol = DYNAMIC_CAST(pOther, EnvironmentCollisionComponent))
		{
			for(std::list<Bullet*>::const_iterator it = bullets.begin(); it != bullets.end(); ++it)
			{
				Bullet* pBullet = (*it);
				if(pEnvCol->CheckCollision(pBullet->vPosition, pBullet->vDirection*pBullet->fSpeed*g_fDeltaTime, m_fCollisionRadius))
				{
					if(OnBulletCollision(pBullet, pEnvCol))
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

			for(std::list<Bullet*>::const_iterator it = bullets.begin(); it != bullets.end(); ++it)
			{
				Bullet* pBullet = (*it);
				for(u32 i=0; i<pRigidBody->GetShapeCount(); ++i)
				{
					if(CollisionShape* pShape = pRigidBody->GetShape(i))
					{
						bool bCollided = false;

						if(CollisionShapeBox* pBox = DYNAMIC_CAST(pShape, CollisionShapeBox))
						{
							bCollided = CollisionUtils::AABBox3DIntersectsWithSphere(pBox->GetBox()*otherTransform.GetScale()+otherTransform.GetMatrix().GetTranslation(), pBullet->vPosition+pBullet->vDirection*pBullet->fSpeed*g_fDeltaTime, pBullet->fRadius);
						}
						else if(CollisionShapeSphere* pSphere = DYNAMIC_CAST(pShape, CollisionShapeSphere))
						{
							bCollided = CollisionUtils::SphereIntersectsWithSphere(pBullet->vPosition+pBullet->vDirection*pBullet->fSpeed*g_fDeltaTime, pBullet->fRadius, 
																				   pSphere->GetCenter(otherTransform.GetMatrix()), pSphere->GetRadius()*otherTransform.GetScale().X);
						}

						if(bCollided)
						{
							if(OnBulletCollision(pBullet, pOther))
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
	bool BulletManagerCollisionComponent::OnBulletCollision(Bullet* pBullet, CollisionComponent* pComponent)
	{
		pBullet->fLife = 0.0f;
		if(Actor* pActor = pComponent->GetParent()->GetAncestor<Actor>())
		{
			if(pActor->IsImmune())
			{
				SFXManager::Instance()->OnBulletCollided(pBullet);
			}

			if(pActor->OnShot(pBullet->vPosition, this, pBullet->damage))
			{
				pComponent->SetEnabled(false);
				return true;
			}
		}
		else
		{
			SFXManager::Instance()->OnBulletCollided(pBullet);
		}

		return false;
	}
}

