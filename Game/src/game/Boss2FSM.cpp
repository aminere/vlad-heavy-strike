/* 

Amine Rehioui
Created: July 1st 2013

*/

#include "ShootTest.h"

#include "Boss2FSM.h"

#include "ConversationManager.h"
#include "GameManager.h"

#include "AIManager.h"
#include "DelayVisitor.h"
#include "SequenceVisitor.h"
#include "RemoveEntityVisitor.h"

namespace shoot
{
	//! Initializes the state table
	void Boss2FSM::Init()
	{
		AddState(snew Boss2StateIntro(), State_Intro);
		AddState(snew Boss2StatePrepareToHook(), State_PrepareToHook);
		AddState(snew Boss2StateHook(), State_Hook);
		AddState(snew Boss2StateIdle(), State_Idle);
		AddState(snew Boss2StateRetreat(), State_Retreat);
		AddState(snew Boss2StateExploding(), State_Exploding);
		AddState(snew Boss2StateInactive(), State_Inactive);		
		SwitchState(State_Intro);
	}

	//! returns the towers left
	u32 Boss2FSM::GetTowersLeft() const
	{
		if(m_pActor->m_aTowers.empty())
		{
			m_pActor->GetChildrenByType("BasicTower", m_pActor->m_aTowers);
		}

		u32 towersLeft = 0;
		for(u32 i=0; i<m_pActor->m_aTowers.size(); ++i)
		{
			if(m_pActor->m_aTowers[i].IsValid())
			{
				++towersLeft;
			}
		}
		return towersLeft;
	}

	// Boss2StateIntro
	void Boss2StateIntro::Begin(shoot::s32 iPreviousState)
	{
	}

	void Boss2StateIntro::Update()
	{
		m_pFSM->SwitchState(Boss2FSM::State_PrepareToHook);
	}

	void Boss2StateIntro::End(shoot::s32 iNextState)
	{
	}

	// Boss2StatePrepareToHook
	void Boss2StatePrepareToHook::Begin(shoot::s32 iPreviousState)
	{
		m_fTimer = m_pActor->m_fPrepareHookDuration;
		m_bWarning = false;
		Player::Instance()->SetInBossFight(true);
	}

	void Boss2StatePrepareToHook::Update()
	{
		Actor* pLeftHook = m_pActor->m_LeftHook;
		Actor* pRightHook = m_pActor->m_RightHook;

		if((pLeftHook && pLeftHook->GetHitPoints() >= 0.0f) || (pRightHook && pRightHook->GetHitPoints() >= 0.0f))		
		{
			Actor* pCurrentHook = m_pActor->m_CurrentHook ? pRightHook : pLeftHook;
			Actor* pOtherHook = m_pActor->m_CurrentHook ? pLeftHook : pRightHook;
			if(!pCurrentHook)
			{
				m_pActor->m_CurrentHook = 1-m_pActor->m_CurrentHook;
				pCurrentHook = pOtherHook;
				pOtherHook = NULL;
			}
			SHOOT_ASSERT(pCurrentHook, "Boss2 hook logic error");

			if(m_fTimer > 0.0f)
			{
				m_bWarning = (m_fTimer < m_pActor->m_fWarningDuration);
				pCurrentHook->SetImmune(m_bWarning);

				if(pCurrentHook->GetHitPoints() >= 0.0f)
				{
					Vector3 vRotation = pCurrentHook->GetRotation();
					f32 fSpinSpeed = m_bWarning ? m_pActor->m_fSpinSpeedFast : m_pActor->m_fSpinSpeedNormal;
					vRotation.Y += fSpinSpeed*g_fDeltaTime;
					pCurrentHook->SetRotation(vRotation);
				}

				// other hook
				if(pOtherHook && pOtherHook->GetHitPoints() >= 0.0f)
				{
					Vector3 vRotation = pOtherHook->GetRotation();
					vRotation.Y += m_pActor->m_fSpinSpeedNormal*g_fDeltaTime;
					pOtherHook->SetRotation(vRotation);
				}

				m_fTimer -= g_fDeltaTime;
			}
			else
			{
				if(pCurrentHook->GetHitPoints() >= 0.0f)
				{
					Vector3 vRotation = pCurrentHook->GetRotation();
					vRotation.Y = 0.0f;
					pCurrentHook->SetRotation(vRotation);
					m_pFSM->SwitchState(Boss2FSM::State_Hook);
				}
				else
				{
					m_fTimer = m_pActor->m_fWarningDuration;
				}
			}
		}
		else
		{
			if(!pLeftHook && !pRightHook)
			{
				m_pFSM->SwitchState(Boss2FSM::State_Idle);
			}
		}
	}

	void Boss2StatePrepareToHook::End(shoot::s32 iNextState)
	{
	}

	// Boss2StateHook
	void Boss2StateHook::Begin(shoot::s32 iPreviousState)
	{
		m_State = S_Orient1;
		m_Hook = m_pActor->m_CurrentHook ? m_pActor->m_RightHook : m_pActor->m_LeftHook;
		SHOOT_ASSERT(m_Hook.IsValid() && m_Hook->IsImmune() && (m_Hook->GetHitPoints() >= 0.0f), "Invalid Hook");
		m_Rope = static_cast<Entity3D*>(m_Hook->GetChildByName("Rope"));
		m_pActor->m_CurrentHook = 1-m_pActor->m_CurrentHook;
		Aim();
	}

	void Boss2StateHook::Update()
	{
		switch(m_State)
		{
		case S_Orient1:
		case S_Orient2:
			{
				if(m_fInterpolator < 1.0f)
				{
					Vector3 vRotation = m_Hook->GetRotation();
					vRotation.Z = Math::Lerp(m_fSrcRotation, m_fDestRotation, m_fInterpolator);
					m_Hook->SetRotation(vRotation);
					m_fInterpolator += g_fDeltaTime/m_pActor->m_fHookOrientDuration;
				}
				else
				{
					Vector3 vRotation = m_Hook->GetRotation();
					vRotation.Z = m_fDestRotation;
					m_Hook->SetRotation(vRotation);
					m_State = (m_State == S_Orient1) ? S_Hook : S_Done;
				}
			}
			break;

		case S_Hook:
			{
				Vector3 vPosition = m_Hook->GetMeshEntity()->GetPosition();				
				vPosition.Y -= m_pActor->m_fHookSpeed*g_fDeltaTime;
				if(vPosition.Y < -m_pActor->m_fHookRange)
				{
					vPosition.Y = -m_pActor->m_fHookRange;
					m_State = S_Retreat;
				}
				m_Hook->GetMeshEntity()->SetPosition(vPosition);
				Vector3 vScale = m_Rope->GetScale();
				vScale.Y = -vPosition.Y;
				m_Rope->SetScale(vScale);
			}
			break;

		case S_Retreat:
			{
				Vector3 vPosition = m_Hook->GetMeshEntity()->GetPosition();				
				vPosition.Y += m_pActor->m_fHookSpeedRetreat*g_fDeltaTime;				
				if(vPosition.Y > 0.0f)
				{
					vPosition.Y = 0.0f;
					m_State = S_Orient2;
					Aim();
				}
				m_Hook->GetMeshEntity()->SetPosition(vPosition);
				Vector3 vScale = m_Rope->GetScale();
				vScale.Y = -vPosition.Y;
				m_Rope->SetScale(vScale);
			}
			break;

		case S_Done:
			m_pFSM->SwitchState(Boss2FSM::State_PrepareToHook);
			break;
		}
	}

	void Boss2StateHook::End(shoot::s32 iNextState)
	{
		m_Hook->SetImmune(false);
	}

	//! aim
	void Boss2StateHook::Aim()
	{
		if(m_State == S_Orient1)
		{
			m_fSrcRotation = m_Hook->GetRotation().Z;

			Player* pPlayer = Player::Instance();
			Vector3 vPosition = m_Hook->GetTransformationMatrix().GetTranslation();			
			Vector3 vPlayerMeshPos = pPlayer->GetMeshEntity()->GetTransformationMatrix().GetTranslation();
			Vector3 vToPlayer = (vPlayerMeshPos-vPosition).Normalize();			
			f32 fDestAngle = Math::ATan2(vToPlayer.X, -vToPlayer.Y)*Math::RadToDegFactor;
			m_fDestRotation = Math::Clamp(-fDestAngle, -m_pActor->m_fHookMaxRotation, m_pActor->m_fHookMaxRotation);
		}
		else
		{
			m_fSrcRotation = m_Hook->GetRotation().Z;
			m_fDestRotation = 0.0f;
		}
		m_fInterpolator = 0.0f;
	}

	// Boss2StateIdle
	void Boss2StateIdle::Begin(shoot::s32 iPreviousState)
	{
	}

	void Boss2StateIdle::Update()
	{
		if(!static_cast<Boss2FSM*>(m_pFSM)->GetTowersLeft())
		{
			if(m_pActor->m_bDestroyWhenDone)
			{
				m_pFSM->SwitchState(Boss2FSM::State_Exploding);
			}
			else
			{
				m_pFSM->SwitchState(Boss2FSM::State_Retreat);
			}			
		}
	}

	void Boss2StateIdle::End(shoot::s32 iNextState)
	{
	}

	// Boss2StateRetreat
	void Boss2StateRetreat::Begin(shoot::s32 iPreviousState)
	{
		m_pActor->GetMeshEntity()->AddComponent(static_cast<Component*>(m_pActor->m_Retreat->Copy()));

		ConversationManager::Instance()->AddPhrase(ConversationManager::T_Enemy, "I WILL BE BACK!", 3.5f);
		ConversationManager::Instance()->Start();

		Player::Instance()->SetInBossFight(false);
	}

	void Boss2StateRetreat::Update()
	{
		if(!ConversationManager::Instance()->IsActive())
		{
			GameManager::Instance()->OnLevelFinished();
		}
	}

	void Boss2StateRetreat::End(shoot::s32 iNextState)
	{
	}

	// Boss1StateExploding
	void Boss2StateExploding::Begin(shoot::s32 iPreviousState)
	{
		m_fTimer = SFXManager::Instance()->Explode(m_pActor);

		DelayVisitor* pDelay = snew DelayVisitor();
		pDelay->SetDuration(m_fTimer+.2f);
		SequenceVisitor* pSequence = snew SequenceVisitor();
		pSequence->AddVisitor(pDelay);
		pSequence->AddVisitor(snew RemoveEntityVisitor());
		m_pActor->AddComponent(pSequence);
				
		Player::Instance()->SetInBossFight(false);
		m_pActor->GetComponent<RigidBodyComponent>()->SetEnabled(false);
	}

	void Boss2StateExploding::Update()
	{
		Player::Instance()->SetBaseSpeed(Vector3::Create(0.0f, 4.0f, 0.0f));

		if(m_fTimer > 0.0f)
		{
			m_fTimer -= g_fDeltaTime;
		}
		else
		{
			m_pFSM->SwitchState(Boss2FSM::State_Inactive);
		}
	}

	void Boss2StateExploding::End(shoot::s32 iNextState)
	{
	}	
}

