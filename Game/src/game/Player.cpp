/* 

Amine Rehioui
Created: October 10th 2011

*/

#include "ShootTest.h"
#include "PlayerFSM.h"
#include "MeshEntity.h"
#include "PlayerCannons.h"
#include "Path.h"
#include "InputManager.h"
#include "Item.h"
#include "PlayerCamera.h"
#include "AIManager.h"
#include "GameManager.h"
#include "DamageComponent.h"
#include "ParticleGenerator.h"
#include "EventManager.h"

namespace shoot
{
	DEFINE_OBJECT(Player);

	//! static vars
	Player* Player::ms_pInstance = NULL;

	//! Constructor
	Player::Player()
		: m_fTilt(0.0f)
		, m_vLocalSpeed(Vector3::Zero)
		, m_vSkyBoxRotation(Vector3::Zero)
		, m_vPreviousTargetPos(Vector3::Zero)
		, m_bInBossFight(false)
		, m_fHealth(100.0f)
		, m_fNoDamageTimer(0.0f)
		, m_fFullHealthTimer(0.0f)
		, m_bHealthChanging(false)
		, m_bAlwaysShowShield(false)
		, m_bPreviousTargetPosValid(false)
		// properties
		, m_fAcceleration(2.5f)
		, m_fTouchAcceleration(250.0f)
		, m_fDecelerationFactor(0.92f)
		, m_MotionBounds(Vector3::Create(-10.0f, -2.0f, -0.5f), Vector3::Create(10.0f, 30.0f, 0.5f))
		, m_vBaseSpeed(Vector3::Create(0.0f, 2.0, 0.0f))		
		, m_fMaxTilt(50.0f)
		, m_fTiltDuration(0.2f)
		, m_fSkyBoxRotationSpeed(4.0f)
		, m_fClampDuration(.2f)
		, m_fRegenPerSecond(25.0f)
		, m_fRegenDelay(1.0f)
		, m_fHideHealthBarTime(1.0f)
		, m_fContactDamagePerSecond(100.0f)
		, m_FullHealthColor(Color::Blue)
		, m_LowHealthColor(Color::Red)
		, m_fRespawnSpectateTime(1.0f)
		, m_fSpawnWidth(20.0f)
	{
		SHOOT_ASSERT(!ms_pInstance, "Multiple Player instances detected");
		ms_pInstance = this;
	}

	//! destructor
	Player::~Player()
	{
		if(IsInitialized())
		{
			EventManager::Instance()->UnregisterListener<TriggerEvent>(this);
		}

		ms_pInstance = NULL;
	}

	//! serializes the entity to/from a PropertyStream
	void Player::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);
		
		stream.Serialize(PT_Float, "Acceleration", &m_fAcceleration);
		stream.Serialize(PT_Float, "TouchAcceleration", &m_fTouchAcceleration);
		stream.Serialize(PT_Float, "DecelerationFactor", &m_fDecelerationFactor);
		stream.Serialize(PT_AABBox3D, "MotionBounds", &m_MotionBounds);
		stream.Serialize(PT_Vec3, "BaseSpeed", &m_vBaseSpeed);		
		stream.Serialize(PT_Float, "MaxTilt", &m_fMaxTilt);		
		stream.Serialize(PT_Float, "TiltDuration", &m_fTiltDuration);
		stream.Serialize(PT_Float, "SkyBoxRotationSpeed", &m_fSkyBoxRotationSpeed);
		stream.Serialize(PT_Float, "ClampDuration", &m_fClampDuration);
		stream.Serialize(PT_Float, "RegenPerSecond", &m_fRegenPerSecond);		
		stream.Serialize(PT_Float, "RegenDelay", &m_fRegenDelay);
		stream.Serialize(PT_Float, "HideHealthBarTime", &m_fHideHealthBarTime);
		stream.Serialize(PT_Float, "ContactDamagePerSecond", &m_fContactDamagePerSecond);
		stream.Serialize(PT_Color, "FullHealthColor", &m_FullHealthColor);
		stream.Serialize(PT_Color, "LowHealthColor", &m_LowHealthColor);
		stream.Serialize(PT_Float, "RespawnSpectateTime", &m_fRespawnSpectateTime);
		stream.Serialize(PT_Float, "SpawnWidth", &m_fSpawnWidth);
	}

	//! called during the initialization of the entity
	void Player::Init()
	{
		m_pFSM = snew PlayerFSM(this);

		super::Init();
		
		m_Cannons = static_cast<PlayerCannons*>(GetChildByType(PlayerCannons::GetStaticClassName()));
		m_RigidBody = m_Mesh->GetComponent<RigidBodyComponent>();
		m_SkyBox = static_cast<Entity3D*>(GetRoot()->GetChildByType("SkyBoxEntity"));
		m_Shield = GetChildByName("Shield");
		m_ShieldColor = m_FullHealthColor;
		m_ShieldColor.A = 0.0f;

		if(m_Shield.IsValid())
		{
			m_ShieldMaterial = m_Shield->GetChildByType("SubMesh")->GetComponent<GraphicComponent>()->GetMaterial();
			m_ShieldBBMaterial = m_Shield->GetChildByType("BillBoardEntity")->GetComponent<GraphicComponent>()->GetMaterial();
			m_Shield->SetVisible(false);
		}

		EventManager::Instance()->RegisterListener<TriggerEvent>(this, &HandleTriggerEvent);
	}

	//! sets the motion width
	void Player::SetMotionWidth(f32 fWidth)
	{
		f32 fCurrentWidth = m_MotionBounds.Size().X;
		f32 fRatio = fWidth/fCurrentWidth;
		m_MotionBounds = m_MotionBounds * Vector3::Create(fRatio, 1.0f, 1.0f);
	}

	//! shoots
	void Player::Shoot()
	{
		if(m_Cannons.IsValid())
		{
			m_Cannons->Shoot();
		}
	}

	//! called when the actor has been shot
	bool Player::OnShot(const Vector3& vPosition, CollisionComponent* pCollisionComponent, f32 damage)
	{
		SFXManager::Instance()->OnActorShot(this, vPosition);

#ifndef _RETAIL_
		if(!(InputManager::Instance()->IsKeyPressed(InputManager::KT_A)
		 && InputManager::Instance()->IsKeyPressed(InputManager::KT_B)))
#endif
		{
			m_fNoDamageTimer = 0.0f;
			m_fFullHealthTimer = 0.0f;
			m_bHealthChanging = true;

			bool bSolidContact = pCollisionComponent->IsA(EnvironmentCollisionComponent::TypeID)
							  || pCollisionComponent->IsA(RigidBodyComponent::TypeID);

			if(bSolidContact)
			{
				DamageComponent* pDamage = DYNAMIC_CAST(pCollisionComponent, DamageComponent);
				EnvironmentCollisionComponent* pEnv = DYNAMIC_CAST(pCollisionComponent, EnvironmentCollisionComponent);
				f32 fDamagePerSecond = pDamage ? pDamage->GetDamagePerSecond() : (pEnv ? 175.0f : m_fContactDamagePerSecond);
				m_fHealth -= fDamagePerSecond*g_fDeltaTime;

				if(pDamage && pDamage->GetExplodeOnContact())
				{
					pDamage->SetEnabled(false);
					if(Actor* pOtherActor = pDamage->GetParent()->GetAncestor<Actor>())
					{
						pOtherActor->OnShot(vPosition, NULL, 9999.0f);
					}
				}
			}
			else
			{
				m_fHealth -= damage;
			}

			m_Shield->SetVisible(true);

			if(m_fHealth < 0.0f)
			{
				m_pFSM->SwitchState(PlayerFSM::State_Exploding);
				return true;
			}
		}

		return false;
	}

	//! called when an item has been collected
	void Player::OnItemCollected(Item* pItem)
	{
		switch(pItem->GetType())
		{
		case Item::T_PulseUp:
		case Item::T_LaserUp:
		case Item::T_PelletUp:
			if(m_Cannons.IsValid())
			{
				m_Cannons->OnWeaponUpgrade((WeaponConfig::E_Type)pItem->GetType());
			}
			break;

		case Item::T_Gold:
			GameManager::Instance()->IncrScore(1);
			break;
		}
	}

	//! follows a path
	void Player::FollowPath(Path* pPath, f32 fDuration)
	{
		m_PathToFollow = pPath;
		m_fFollowPathTime = 0.0f;
		m_fFollowPathDuration = fDuration;
		m_pFSM->SwitchState(PlayerFSM::State_FollowingPath);
	}

	//! common update
	void Player::CommonUpdate()
	{
		// check checkpoints
		ConfigPoint* pNextCheckpoint = AIManager::Instance()->GetNextCheckpoint();
		if(pNextCheckpoint && pNextCheckpoint->IsReached())
		{
			AIManager::Instance()->IncrCheckpoint();
		}

		m_Mesh->Translate(m_vLocalSpeed);

		if(InputManager::Instance()->IsTouchJustPressed())
		{
			GetTouchTarget(m_vPreviousTargetPos);
			m_bPreviousTargetPosValid = true;
		}
		else if(InputManager::Instance()->IsTouchPressed())
		{
			if(m_bPreviousTargetPosValid)
			{
				Vector3 vTarget;
				if(GetTouchTarget(vTarget))
				{
					m_vLocalSpeed = vTarget - m_vPreviousTargetPos;
					m_vPreviousTargetPos = vTarget;
				}
			}
			else
			{
				GetTouchTarget(m_vPreviousTargetPos);
				m_bPreviousTargetPosValid = true;
			}
		}
		else
		{
			m_vLocalSpeed *= m_fDecelerationFactor;
		}

		// clamp player position
		Vector3 vPosition = m_Mesh->GetPosition();
		f32 fClampedX = Math::Clamp(vPosition.X, m_MotionBounds.Min().X, m_MotionBounds.Max().X);
		vPosition.Y = Math::Clamp(vPosition.Y, m_MotionBounds.Min().Y, m_MotionBounds.Max().Y);
		vPosition.X = Math::Damp(vPosition.X, fClampedX, g_fDeltaTime, m_fClampDuration);		
		m_Mesh->SetPosition(vPosition);

		if(AIManager::Instance()->IsEnnemyDetected() || m_bInBossFight)
		{
			Shoot();
		}

		// apply tilt
		f32 fTilt = Math::FAbs(m_vLocalSpeed.X) > 0.1f ? Math::FSign(m_vLocalSpeed.X) : 0.0f;
		m_fTilt = Math::Damp(m_fTilt, m_fMaxTilt*fTilt, g_fDeltaTime, m_fTiltDuration);
		m_Mesh->SetRotation(Vector3::Create(m_vRotation.X, -m_fTilt, m_vRotation.Z));

		// update health
		if(m_bHealthChanging)
		{
			UpdateShieldColor();

			if(m_fHealth < 100.0f)
			{
				if(m_fNoDamageTimer > m_fRegenDelay)
				{
					m_fHealth += m_fRegenPerSecond*g_fDeltaTime;
				}
			}
			else
			{
				m_fFullHealthTimer += g_fDeltaTime;
				if(m_fFullHealthTimer > m_fHideHealthBarTime && !m_bAlwaysShowShield)
				{
					m_Shield->SetVisible(false);
					m_bHealthChanging = false;
				}
			}
			m_fNoDamageTimer += g_fDeltaTime;
		}

		// update ridig body
		m_RigidBody->SetVelocity(m_vLocalSpeed+(m_vBaseSpeed*g_fDeltaTime));
	}

	//! update the shield color
	void Player::UpdateShieldColor()
	{
		Color color = Math::Lerp(m_LowHealthColor, m_FullHealthColor, m_fHealth/100.0f);		
		if(m_bAlwaysShowShield)
		{
			color.A = 1.0f;
		}
		else
		{
			color.A = (m_fHealth < 100.0f) ? 1.0f : 0.0f;
		}
		m_ShieldColor = Math::Damp(m_ShieldColor, color, g_fDeltaTime, .5f);
		m_ShieldMaterial->SetColor(m_ShieldColor);
		m_ShieldBBMaterial->SetColor(m_ShieldColor);
	}

	//! gets the touch target
	bool Player::GetTouchTarget(Vector3& vTarget) const
	{
		Vector3 rayStart = Vector3::Zero, rayDir = Vector3::One;
		Vector2 vTouchPos = InputManager::Instance()->GetRawTouchState().vPosition;
		const Vector2& vScreenSize = GraphicsDriver::Instance()->GetViewPort().Size();
		PlayerCamera::Instance()->Get3DRayFromScreenCoords(rayStart, rayDir, Point(s32(vTouchPos.X), s32(vTouchPos.Y)), vScreenSize);
		Plane plane(Vector3::Create(0.0f, 0.0f, 1.0f), 0.0f);
		if(plane.IntersectWithRay(rayStart, rayDir, &vTarget))
		{
			vTarget -= PlayerCamera::Instance()->GetPosition();
			vTarget.Z = 0.0f;
			return true;
		}
		return false;
	}

	//! called when the player has just respawned
	void Player::OnRespawn()
	{
		if(Component* pExplosion = GetMeshEntity()->GetComponentByType("ExplosionVisitor"))
		{
			GetMeshEntity()->RemoveComponent(pExplosion, true);
		}

		for(u32 i=0; i<m_Mesh->GetChildCount(); ++i)
		{
			m_Mesh->GetChild(i)->SetVisible(true);
		}

		// resume particle generation
		std::vector< Handle<Entity> > particleEmitters;
		GetChildrenByType("ParticleGenerator", particleEmitters);
		for(u32 i=0; i<particleEmitters.size(); ++i)
		{
			static_cast<ParticleGenerator*>(particleEmitters[i].Get())->SetActive(true);
		}

		m_Cannons->Reset();
		AIManager::Instance()->OnRespawn();
		GetMeshEntity()->SetPosition(Vector3::Zero);
		PlayerCamera::Instance()->Reset();
		SFXManager::Instance()->Clear();

		m_fHealth = 100.0f;
		m_bHealthChanging = false;
		m_ShieldColor = m_FullHealthColor;
		m_ShieldColor.A = 0.0f;
		m_Shield->SetVisible(false);
		m_vLocalSpeed = Vector3::Zero;
	}

	//! sets bossfight status
	void Player::SetInBossFight(bool bInBossFight) 
	{ 
		m_bInBossFight = bInBossFight;
	}

	//! sets shield display status
	void Player::SetAlwaysShowShield(bool bAlwaysShow)
	{
		if(m_bAlwaysShowShield != bAlwaysShow)
		{
			m_Shield->SetVisible(true);
			m_bHealthChanging = true;
			m_fFullHealthTimer = 0.0f;
			m_bAlwaysShowShield = bAlwaysShow;
		}
	}

	//! sets the skybox rotation
	void Player::SetSkyBoxRotation(const Vector3& vRotation)
	{
		m_vSkyBoxRotation = vRotation;
		m_SkyBox->SetRotation(m_vSkyBoxRotation);
	}

	//! set the spawn width
	void Player::SetSpawnWidth(f32 fWidth) 
	{
		m_fSpawnWidth = fWidth;
		m_aSpawningPoints.clear();
	}

	//! shoot event listeners
	void Player::HandleTriggerEvent(void* pListener, Event* pEvent)
	{
		TriggerEvent* pTrigger = static_cast<TriggerEvent*>(pEvent);
		Player* pPlayer = Player::Instance();
		if(pTrigger->Name == "INGAME_TRANSITION")
		{
			SHOOT_ASSERT(pPlayer->m_pFSM->GetState() == PlayerFSM::State_Respawning, "Invalid Player FSM state");
			pPlayer->OnRespawn();
		}
		else if(pTrigger->Name == "INGAME_TRANSITION_OVER")
		{
			SHOOT_ASSERT(pPlayer->m_pFSM->GetState() == PlayerFSM::State_Respawning, "Invalid Player FSM state");
			pPlayer->m_pFSM->SwitchState(PlayerFSM::State_Idle);
			pPlayer->m_Mesh->GetComponent<RigidBodyComponent>()->SetEnabled(true);
			AIManager::Instance()->GetCurrentCheckpoint()->Apply();
		}
	}
}

