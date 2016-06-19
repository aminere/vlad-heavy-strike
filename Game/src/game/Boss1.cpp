/* 

Amine Rehioui
Created: May 9th 2013

*/

#include "ShootTest.h"

#include "Boss1.h"

#include "Boss1FSM.h"

#include "EventManager.h"

#include "RigidBodyComponent.h"

#include "Utils.h"

namespace shoot
{
	DEFINE_OBJECT(Boss1);

	u32 Boss1::m_sNumCannons = 8;

	//! constructor
	Boss1::Boss1()
		: m_CurrentSide(0)
		// properties
		, m_fCannonOffset(.5f)
		, m_fPullbackDuration(.2f)
		, m_fCannonFrequency(.2f)
		, m_fCannonActiveTime(2.0f)
		, m_fCannonInactiveTime(1.5f)
		, m_fDualShootDuration(5.0f)
		, m_fSideSwitchDuration(1.0f)
		, m_fBulletOffset(-4.0f)
		, m_fBulletSize(8.0f)
		, m_fBulletSpeed(10.0f)
		, m_fBulletLife(1.5f)
		, m_fAimDuration(.5f)
		, m_bDestroyWhenDone(false)
	{
	}

	//! destructor
	Boss1::~Boss1()
	{
		if(IsInitialized())
		{
			EventManager::Instance()->UnregisterListener<TriggerEvent>(this);
		}
	}

	//! serializes the entity to/from a PropertyStream
	void Boss1::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);
		
		stream.Serialize(PT_Float, "CannonOffset", &m_fCannonOffset);
		stream.Serialize(PT_Float, "PullbackDuration", &m_fPullbackDuration);
		stream.Serialize(PT_Float, "CannonFrequency", &m_fCannonFrequency);
		stream.Serialize(PT_Float, "CannonActiveTime", &m_fCannonActiveTime);
		stream.Serialize(PT_Float, "CannonInactiveTime", &m_fCannonInactiveTime);
		stream.Serialize(PT_Float, "DualShootDuration", &m_fDualShootDuration);
		stream.Serialize(PT_Float, "SideSwitchDuration", &m_fSideSwitchDuration);
		stream.Serialize(PT_Float, "BulletOffset", &m_fBulletOffset);
		stream.Serialize(PT_Float, "BulletSize", &m_fBulletSize);
		stream.Serialize(PT_Float, "BulletSpeed", &m_fBulletSpeed);
		stream.Serialize(PT_Float, "BulletLife", &m_fBulletLife);
		stream.Serialize(PT_Float, "AimDuration", &m_fAimDuration);
		
		stream.SerializeReference("SpinAttackLeft", &m_SpinAttackLeft);
		stream.SerializeReference("SpinAttackRight", &m_SpinAttackRight);		
		stream.SerializeReference("Retreat", &m_Retreat);

		stream.Serialize(PT_Bool, "DestroyWhenDone", &m_bDestroyWhenDone);
	}

	//! called during the initialization of the entity
	void Boss1::Init()
	{
		m_pFSM = snew Boss1FSM(this);
		super::Init();

		m_aCannons.clear();
		for(u32 i=0; i<m_sNumCannons; ++i)
		{
			if(Actor* pCannon = static_cast<Actor*>(GetChildByName(std::string("Cannon") + Utils::ToString(i+1))))
			{
				m_aCannons.push_back(CannonInfo(pCannon));
				pCannon->GetMeshEntity()->GetComponent<RigidBodyComponent>()->SetEnabled(false);
			}
		}

		EventManager::Instance()->RegisterListener<TriggerEvent>(this, &HandleTriggerEvent);		
	}

	//! called during the update of the entity	
	void Boss1::Update()
	{
		m_pFSM->Update();
	}

	//! shoot event listeners
	void Boss1::HandleTriggerEvent(void* pListener, Event* pEvent)
	{
		TriggerEvent* pTrigger = static_cast<TriggerEvent*>(pEvent);
		Boss1* pBoss1 = static_cast<Boss1*>(pListener);
		MeshEntity* pMesh = pBoss1->GetMeshEntity();
		if(pTrigger->Target.Get() == pMesh)
		{
			if(pTrigger->Name == "SPIN_OVER")
			{
				SHOOT_ASSERT(pBoss1->m_pFSM->GetState() == Boss1FSM::State_SpinAttack, "Invalid FSM state");
				pMesh->RemoveComponent(pMesh->GetComponent(pMesh->GetComponentCount()-1), true);
				pBoss1->m_pFSM->SwitchState(Boss1FSM::State_PostSpinSetup);
			}
			else
			{
				SHOOT_ASSERT(false, "Unhandled trigger");
			}
		}
	}
}

