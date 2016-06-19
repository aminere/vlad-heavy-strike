/* 

Amine Rehioui
Created: May 18th 2013

*/

#include "ShootTest.h"

#include "ExplosionVisitor.h"
#include "SequenceVisitor.h"
#include "DelayVisitor.h"
#include "RemoveEntityVisitor.h"
#include "AudioManager.h"
#include "PlayerCamera.h"
#include "ParticleGenerator.h"

namespace shoot
{
	DEFINE_OBJECT(SFXManager);

	//! static vars
	SFXManager* SFXManager::ms_pInstance = NULL;

	//! constructor
	SFXManager::SFXManager()
		: m_bSkyBoxAnim(false)
		, m_fInterpolator(0.0f)
		, m_fExplosionTracker(0.0f)
		, m_fCameraShakeIntensity(.5f)
		, m_fCameraShakeFrequency(.02f)
		// properties
		, m_fExplosionIntensity(8.0f)
		, m_fExplosionDuration(1.3f)
		, m_fSkyBoxAnimDuration(1.5f)
	{
		SHOOT_ASSERT(!ms_pInstance, "Multiple SFXManager instances detected");
		ms_pInstance = this;
	}

	//! destructor
	SFXManager::~SFXManager()
	{
		ms_pInstance = NULL;
	}

	//! serializes the entity to/from a PropertyStream
	void SFXManager::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);

		stream.Serialize(PT_Float, "ExplosionIntensity", &m_fExplosionIntensity);
		stream.Serialize(PT_Float, "ExplosionDuration", &m_fExplosionDuration);
		stream.Serialize(PT_Float, "SkyBoxAnimDuration", &m_fSkyBoxAnimDuration);
		stream.Serialize(PT_Float, "CameraShakeIntensity", &m_fCameraShakeIntensity);
		stream.Serialize(PT_Float, "CameraShakeFrequency", &m_fCameraShakeFrequency);
		stream.SerializeArray("PreloadedObjects", &m_PreloadedObjects, PT_Reference);
		stream.SerializeArray("ExplosionInfos", &m_ExplosionInfos, PT_Struct);
	}

	//! called during the initialization of the entity
	void SFXManager::Init()
	{
		super::Init();

		m_Root = GetRoot();

		m_PlayerPulseManager = static_cast<PulseManager*>(GetChildByName("PlayerPulseManager"));
		m_PlayerLaserManager = static_cast<LaserManager*>(GetChildByName("PlayerLaserManager"));
		m_PlayerPelletManager = static_cast<PelletManager*>(GetChildByName("PlayerPelletManager"));
		m_PlayerSidePulseManager = static_cast<PulseManager*>(GetChildByName("PlayerSidePulseManager"));
		m_PlayerSideLaserManager = static_cast<LaserManager*>(GetChildByName("PlayerSideLaserManager"));
		m_PlayerSidePelletManager = static_cast<PelletManager*>(GetChildByName("PlayerSidePelletManager"));
		m_EnemyPulseManager = static_cast<PulseManager*>(GetChildByName("EnemyPulseManager"));
		m_BossPulseManager = static_cast<PulseManager*>(GetChildByName("BossPulseManager"));		
		m_EnemyLaserManager = static_cast<LaserManager*>(GetChildByName("EnemyLaserManager"));
		m_EnemyPelletManager = static_cast<PelletManager*>(GetChildByName("EnemyPelletManager"));
	
		if(Entity* pSkyBox = m_Root->GetChildByType("SkyBoxEntity"))
		{
			m_SkyBoxMaterial = pSkyBox->GetComponent<GraphicComponent>()->GetMaterial();
		}
	}

	//! called during the update of the entity
	void SFXManager::Update()
	{
		UpdateSkyBox();

		m_fExplosionTracker += g_fDeltaTime;
	}

	//! called when a bullet collided
	void SFXManager::OnBulletCollided(Bullet* pBullet)
	{
		const char* strHitEntity = pBullet->type == Bullet::T_Pulse ? "common/NoHitPulse.xml" : "common/NoHitPellet.xml";
		Object* pBaseEntity = GetPreloadedObject(strHitEntity);
		Entity3D* pEntity = static_cast<Entity3D*>(pBaseEntity->Copy());
		m_Root->AddChild(pEntity);
		pEntity->SetPosition(pBullet->vPosition);
		pEntity->Init();
	}

	//! called when a laser collided
	void SFXManager::OnLaserCollided(const Vector3& vIntersection)
	{
		Object* pBaseEntity = GetPreloadedObject("common/NoHitLaser.xml");
		Entity3D* pEntity = static_cast<Entity3D*>(pBaseEntity->Copy());
		m_Root->AddChild(pEntity);
		pEntity->SetPosition(vIntersection);
		pEntity->Init();
	}

	//! called when an actor was shot
	void SFXManager::OnActorShot(Actor* pActor, const Vector3& vPosition)
	{
		if(!pActor->IsImmune())
		{
			Object* pBaseEntity = GetPreloadedObject("common/HitEntity.xml");
			Entity3D* pEntity = static_cast<Entity3D*>(pBaseEntity->Copy());
			m_Root->AddChild(pEntity);
			pEntity->SetPosition(vPosition);
			pEntity->Init();

			if(pActor->GetShakeOnShot())
			{
				// apply shake
				if(Visitor* pShakeVisitor = static_cast<Visitor*>(pActor->GetMeshEntity()->GetComponent("ShakeVisitor")))
				{
					pShakeVisitor->Visit(pActor->GetMeshEntity());
				}
				else
				{
					pActor->GetMeshEntity()->AddComponent(static_cast<Component*>(GetPreloadedObject("common/ShakeVisitor.xml")->Copy()));
				}
			}
		}
	}

	//! called when an actor is destroyed
	void SFXManager::OnActorDestroyed(Actor* pActor)
	{
		f32 fDuration = Explode(pActor);
		if(fDuration > 0.0f)
		{
			DelayVisitor* pDelay = snew DelayVisitor();
			pDelay->SetDuration(fDuration);
			SequenceVisitor* pSequence = snew SequenceVisitor();
			pSequence->AddVisitor(pDelay);
			pSequence->AddVisitor(snew RemoveEntityVisitor());
			pActor->AddComponent(pSequence);
		}		
	}

	//! Makes an Actor explode
	f32 SFXManager::Explode(Actor* pActor)
	{
		if(Object* pBaseExplosionVisitor = GetExplosionVisitor(pActor->GetTemplatePath()))
		{
			bool bIsPlayer = (pActor == Player::Instance());
			bool bIsBoss = pActor->IsBoss();
			const char* strExplosionEntity = bIsPlayer ? "common/playerexplosionentity.xml" : (bIsBoss ? "5/bossexplosionentity.xml" : "common/ExplosionEntity.xml");
			Object* pBaseExplosionEntity = GetPreloadedObject(strExplosionEntity);

			ExplosionVisitor* pExplosionVisitor = static_cast<ExplosionVisitor*>(pBaseExplosionVisitor->Copy());
			Entity3D* pExplosionEntity = static_cast<Entity3D*>(pBaseExplosionEntity->Copy());		

			for(u32 i=0; i<pActor->GetMeshEntity()->GetComponentCount(); ++i)
			{
				if(Visitor* pVisitor = DYNAMIC_CAST(pActor->GetMeshEntity()->GetComponent(i), Visitor))
				{
					pVisitor->Leave();
				}
			}
			
			for(u32 i=0; i<pActor->GetMeshEntity()->GetChildCount(); ++i)
			{
				Entity* pChild = pActor->GetMeshEntity()->GetChild(i);
				if(!pChild->IsA(SubMesh::TypeID) 
				&& !pChild->IsA(ParticleGenerator::TypeID)
				&& pChild->GetName() != "Explosion")
				{
					pActor->GetMeshEntity()->GetChild(i)->SetVisible(false);
				}
			}

			// stop particle generation
			std::vector< Handle<Entity> > particleEmitters;
			pActor->GetChildrenByType("ParticleGenerator", particleEmitters);
			for(u32 i=0; i<particleEmitters.size(); ++i)
			{
				static_cast<ParticleGenerator*>(particleEmitters[i].Get())->SetActive(false);
			}

			// add explosion visitor
			pExplosionVisitor->SetDefaultIntensity(m_fExplosionIntensity);
			pExplosionVisitor->SetDefaultDuration(m_fExplosionDuration);
			pActor->GetMeshEntity()->AddComponent(pExplosionVisitor);

			// add explosion entity
			pExplosionEntity->SetScale(Vector3::One * pActor->GetExplosionScaleFactor());
			pActor->AddChild(pExplosionEntity);
			pExplosionEntity->SetPosition(pActor->GetMeshEntity()->GetPosition());
			f32 fRotationRange = bIsBoss ? 0.0f : 30.0f;
			pExplosionEntity->SetRotation(Vector3::Create(0.0f, Random::GetFloat(-fRotationRange, fRotationRange), 0.0f));

			AudioManager::Instance()->Play(bIsBoss ? AudioManager::S_LongExplosion : AudioManager::S_Explosion);
			
			// add camera shake
			f32 fExplosionDuration = (pExplosionVisitor->GetDuration() < 0.0f) ? m_fExplosionDuration : pExplosionVisitor->GetDuration();
			bool bMultipleExplosionsDetected = (m_fExplosionTracker < m_fExplosionDuration);
			bool bForceShake = bIsPlayer || bIsBoss || pActor->GetShakeOnExplode();
			f32 fForceShakeFactor = 2.0f;
			if(bMultipleExplosionsDetected || bForceShake)
			{
				f32 fIntFactor = bForceShake ? fForceShakeFactor : 1.0f;
				f32 fFreqFactor = bForceShake ? (1.0f/fForceShakeFactor) : 1.0f;
				PlayerCamera::Instance()->Shake(m_fCameraShakeIntensity*fIntFactor, m_fCameraShakeFrequency*fFreqFactor, fExplosionDuration/2.0f);
			}

			m_fExplosionTracker = 0.0f;
			if(pActor->GetResetRotationOnExplode())
			{
				pActor->SetRotation(Vector3::Zero);
			}
			return fExplosionDuration;
		}

		return -1.0f;
	}

	//! animates the skybox color
	void SFXManager::SetSkyBoxColor(const Color& color)
	{
		m_SrcSkyBoxColor = m_SkyBoxMaterial->GetColor();
		m_DestSkyBoxColor = color;
		m_fInterpolator = 0.0f;
		m_bSkyBoxAnim = true;
	}

	//! returns the ExplosionVisitor corresponding to a template
	Object* SFXManager::GetExplosionVisitor(const std::string& templatePath)
	{
		for(u32 i=0; i<m_ExplosionInfos.GetSize(); ++i)
		{
			for(u32 j=0; j<m_ExplosionInfos[i].m_TemplatePaths.GetSize(); ++j)
			{
				if(m_ExplosionInfos[i].m_TemplatePaths[j] == templatePath)
				{
					return m_ExplosionInfos[i].m_Explosion;
				}
			}
		}

		return NULL;
	}

	Object* SFXManager::GetPreloadedObject(const char* strPath)
	{
		for(u32 i=0; i<m_PreloadedObjects.GetSize(); ++i)
		{
			if(Object* pObject = m_PreloadedObjects[i].Get())
			{
				if(pObject->GetTemplatePath() == strPath)
				{
					return pObject;
				}
			}
		}
		return NULL;
	}

	//! updates the skybox
	void SFXManager::UpdateSkyBox()
	{
		if(m_bSkyBoxAnim)
		{
			if(m_fInterpolator < 1.0f)
			{
				Color color = Math::Lerp<Color>(m_SrcSkyBoxColor, m_DestSkyBoxColor, m_fInterpolator);
				m_SkyBoxMaterial->SetColor(color);
				m_fInterpolator += g_fDeltaTime/m_fSkyBoxAnimDuration;

			}
			else
			{
				m_SkyBoxMaterial->SetColor(m_DestSkyBoxColor);
				m_bSkyBoxAnim = false;
			}
		}
	}

	//! clears all effects
	void SFXManager::Clear()
	{
		m_PlayerPulseManager->Clear();
		m_PlayerLaserManager->Clear();
		m_PlayerPelletManager->Clear();
		m_PlayerSidePulseManager->Clear();
		m_PlayerSideLaserManager->Clear();
		m_PlayerSidePelletManager->Clear();
		m_EnemyPulseManager->Clear();
		m_BossPulseManager->Clear();		
		m_EnemyLaserManager->Clear();
		m_EnemyPelletManager->Clear();
	}
}
