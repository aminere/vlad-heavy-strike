/* 

Amine Rehioui
Created: October 10th 2011

*/

#include "ShootTest.h"

#include "PlayerFSM.h"

#include "RigidBodyComponent.h"

#include "MeshEntity.h"

#include "ExplosionVisitor.h"

#include "AIManager.h"

#include "SequenceVisitor.h"

namespace shoot
{
	//! Initializes the state table
	void PlayerFSM::Init()
	{
		AddState(snew PlayerStateIdle(), State_Idle);
		AddState(snew PlayerStateExploding(), State_Exploding);
		AddState(snew PlayerStateFollowingPath(), State_FollowingPath);
		AddState(snew PlayerStateRespawning(), State_Respawning);
		SwitchState(State_Idle);
	}

	// Player State_Idle functions
	void PlayerStateIdle::Begin(shoot::s32 iPreviousState)
	{
	}

	void PlayerStateIdle::Update()
	{
		if(!m_pActor->IsImmune())
		{
			// apply base motion
			m_pActor->Translate(m_pActor->m_vBaseSpeed*g_fDeltaTime);
			if(m_pActor->m_vBaseSpeed.Y > 0.0f)
			{
				m_pActor->m_SkyBox->SetRotation(m_pActor->m_vSkyBoxRotation);
				m_pActor->m_vSkyBoxRotation.X += m_pActor->m_fSkyBoxRotationSpeed*g_fDeltaTime;
			}
		}

		m_pActor->CommonUpdate();
	}

	void PlayerStateIdle::End(shoot::s32 iNextState)
	{
	}

	// Player State_Exploding functions
	void PlayerStateExploding::Begin(shoot::s32 iPreviousState)
	{
		f32 fDuration = SFXManager::Instance()->Explode(m_pActor);
		m_pActor->GetMeshEntity()->GetComponent<RigidBodyComponent>()->SetEnabled(false);
		m_pActor->GetMeshEntity()->SetRotation(Vector3::Zero);	
		m_pActor->m_fTilt = 0.0f;
		m_pActor->m_Shield->SetVisible(false);
		m_pActor->m_bPreviousTargetPosValid = false;
		m_fTimer = fDuration;
	}

	void PlayerStateExploding::Update()
	{
		if(m_fTimer > 0.0f)
		{
			m_fTimer -= g_fDeltaTime;			
		}
		else
		{
			m_pFSM->SwitchState(PlayerFSM::State_Respawning);
		}
	}

	void PlayerStateExploding::End(shoot::s32 iNextState)
	{
	}

	// Player State_FollowingPath functions
	void PlayerStateFollowingPath::Begin(s32 iPreviousState)
	{
	}

	void PlayerStateFollowingPath::Update()
	{
		m_pActor->CommonUpdate();

		if(m_pActor->m_fFollowPathTime < m_pActor->m_fFollowPathDuration)
		{
			f32 fProgress = m_pActor->m_fFollowPathTime/m_pActor->m_fFollowPathDuration;
			Vector3 vPosition = m_pActor->m_PathToFollow->GetPosition(fProgress);
			m_pActor->SetPosition(vPosition+Vector3::Create(0.0f, 0.1f, 0.0f));
			m_pActor->m_fFollowPathTime += g_fDeltaTime;
		}
		else
		{
			Vector3 vPosition = m_pActor->m_PathToFollow->GetPosition(1.0f);
			m_pActor->SetPosition(vPosition+Vector3::Create(0.0f, 0.1f, 0.0f));
			m_pActor->m_PathToFollow.Clear();
			m_pFSM->SwitchState(PlayerFSM::State_Idle);
		}
	}

	void PlayerStateFollowingPath::End(s32 iNextState)
	{
	}

	// PlayerStateRespawning functions
	void PlayerStateRespawning::Begin(s32 iPreviousState)
	{
		Entity* pFadeElement = Engine::Instance()->GetContext(1)->GetChildByName("FadeElement");
		pFadeElement->GetComponent<SequenceVisitor>()->Visit(pFadeElement);
	}

	void PlayerStateRespawning::Update()
	{
	}

	void PlayerStateRespawning::End(s32 iNextState)
	{
	}
}
