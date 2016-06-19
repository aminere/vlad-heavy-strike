/* 

Amine Rehioui
Created: May 10th 2013

*/

#include "ShootTest.h"

#include "Boss1FSM.h"
#include "RigidBodyComponent.h"
#include "ConversationManager.h"
#include "GameManager.h"
#include "AIManager.h"
#include "DelayVisitor.h"
#include "SequenceVisitor.h"
#include "RemoveEntityVisitor.h"

namespace shoot
{
	//! Initializes the state table
	void Boss1FSM::Init()
	{
		AddState(snew Boss1StateIntro(), State_Intro);
		AddState(snew Boss1StateDualCannons(), State_DualCannons);
		AddState(snew Boss1StateSpinAttack(), State_SpinAttack);
		AddState(snew Boss1StatePostSpinSetup(), State_PostSpinSetup);
		AddState(snew Boss1StateRetreat(), State_Retreat);
		AddState(snew Boss1StateExploding(), State_Exploding);
		AddState(snew Boss1StateInactive(), State_Inactive);
		SwitchState(State_Intro);
	}

	// Boss1StateIntro
	void Boss1StateIntro::Begin(shoot::s32 iPreviousState)
	{		
	}

	void Boss1StateIntro::Update()
	{
		m_pFSM->SwitchState(Boss1FSM::State_DualCannons);
	}

	void Boss1StateIntro::End(shoot::s32 iNextState)
	{
	}

	// Boss1StateDualCannons
	void Boss1StateDualCannons::Begin(shoot::s32 iPreviousState)
	{
		m_CurrentCannon = 1;
		m_fGlobalTimer = m_pActor->m_fDualShootDuration;
		m_fActiveTimer = m_pActor->m_fCannonActiveTime;
		m_bActive = true;
		m_fShootTimer = -1.0f;
		Player::Instance()->SetInBossFight(true);

		Boss1::CannonInfo& cannon1 = m_pActor->m_aCannons[m_pActor->m_CurrentSide*2+0];
		Boss1::CannonInfo& cannon2 = m_pActor->m_aCannons[m_pActor->m_CurrentSide*2+1];
		cannon1.m_Entity->GetMeshEntity()->GetComponent<RigidBodyComponent>()->SetEnabled(true);
		cannon2.m_Entity->GetMeshEntity()->GetComponent<RigidBodyComponent>()->SetEnabled(true);		
	}

	void Boss1StateDualCannons::Update()
	{
		Boss1::CannonInfo& cannon1 = m_pActor->m_aCannons[m_pActor->m_CurrentSide*2+0];
		Boss1::CannonInfo& cannon2 = m_pActor->m_aCannons[m_pActor->m_CurrentSide*2+1];
		if(!cannon1.m_Entity.IsValid() && !cannon2.m_Entity.IsValid())
		{
			if(m_pActor->m_bDestroyWhenDone && m_pActor->m_CurrentSide >= 3)
			{
				m_pFSM->SwitchState(Boss1FSM::State_Exploding);
			}
			else
			{
				m_pFSM->SwitchState(Boss1FSM::State_SpinAttack);
			}
		}
		else
		{
			// aim
			Player* pPlayer = Player::Instance();
			Vector3 vPosition = m_pActor->GetTransformationMatrix().GetTranslation();			
			Vector3 vPlayerMeshPos = pPlayer->GetMeshEntity()->GetTransformationMatrix().GetTranslation();
			Vector3 vToPlayer = (vPlayerMeshPos-vPosition).Normalize();			
			f32 fDestAngle = Math::ATan2(vToPlayer.X, -vToPlayer.Y)*Math::RadToDegFactor;
			f32 fDestRotation = 90.0f*m_pActor->m_CurrentSide-fDestAngle;
			f32 fRotation = Math::Damp(m_pActor->GetMeshEntity()->GetRotation().Z, fDestRotation, g_fDeltaTime, m_pActor->m_fAimDuration);
			m_pActor->GetMeshEntity()->SetRotation(Vector3::Create(0.0f, 0.0f, fRotation));

			if(m_bActive)
			{
				if(m_fActiveTimer > 0.0f)
				{
					if(m_fShootTimer < 0.0f)
					{
						m_CurrentCannon = 1-m_CurrentCannon;
						Boss1::CannonInfo& cannon = m_CurrentCannon ? cannon2 : cannon1;
						if(cannon.m_Entity.IsValid()
						&& cannon.m_Entity->GetHitPoints() >= 0.0f)
						{
							Vector3 vCannonDir = cannon.m_Entity->GetPosition();
							(Math::FAbs(vCannonDir.X) < Math::FAbs(vCannonDir.Y)) ? (vCannonDir.X = 0.0f) : (vCannonDir.Y = 0.0f);
							vCannonDir.Normalize();
							cannon.m_Entity->SetPosition(cannon.vOriginalPos+vCannonDir*m_pActor->m_fCannonOffset);

							// shoot
							Vector3 vBulletPos = cannon.m_Entity->GetTransformationMatrix().GetTranslation()
											   + Vector3::Create(0.0f, m_pActor->m_fBulletOffset, 0.0f).Rotate(Vector3::Create(.0f, .0f, (fRotation-90.0f*m_pActor->m_CurrentSide)*Math::DegToRadFactor));

							Bullet::BulletParams params;
							params.vDirection = vToPlayer;
							params.fSpeed = m_pActor->m_fBulletSpeed;
							params.vPosition = vBulletPos;
							params.fLife = m_pActor->m_fBulletLife;
							params.fRadius = m_pActor->m_fBulletSize/2.0f;							
							SFXManager::Instance()->GetBossPulseManager()->AddBullet(params);
						}
						m_fShootTimer = m_pActor->m_fCannonFrequency;					
					}
					else
					{
						Boss1::CannonInfo& cannon = m_CurrentCannon ? cannon2 : cannon1;
						if(cannon.m_Entity.IsValid())
						{
							Vector3 vPosition = (cannon.m_Entity->GetPosition());
							vPosition = Math::Damp(vPosition, cannon.vOriginalPos, g_fDeltaTime, m_pActor->m_fPullbackDuration);
							cannon.m_Entity->SetPosition(vPosition);						
						}
						m_fShootTimer -= g_fDeltaTime;
					}
					m_fActiveTimer -= g_fDeltaTime;
				}
				else
				{	
					if(cannon1.m_Entity.IsValid())
					{
						cannon1.m_Entity->SetPosition(cannon1.vOriginalPos);
					}
					if(cannon2.m_Entity.IsValid())
					{
						cannon2.m_Entity->SetPosition(cannon2.vOriginalPos);
					}
					m_fInactiveTimer = m_pActor->m_fCannonInactiveTime;
					m_bActive = false;
				}
			}
			else
			{
				if(m_fInactiveTimer > 0.0f)
				{
					m_fInactiveTimer -= g_fDeltaTime;
				}
				else
				{
					m_fActiveTimer = m_pActor->m_fCannonActiveTime;
					m_bActive = true;
				}
			}
		}
	}

	void Boss1StateDualCannons::End(shoot::s32 iNextState)
	{
		Player::Instance()->SetInBossFight(false);
	}	

	// Boss1StateSpinAttack
	void Boss1StateSpinAttack::Begin(shoot::s32 iPreviousState)
	{
		u32 attack = Random::GetInt(0, 1);
		Component* pSpinAttack = static_cast<Component*>(attack ? m_pActor->m_SpinAttackLeft->Copy() : m_pActor->m_SpinAttackRight->Copy());
		m_pActor->GetMeshEntity()->AddComponent(pSpinAttack);

		for(u32 i=0; i<m_pActor->m_aCannons.size(); ++i)
		{
			if(Actor* pCannon = m_pActor->m_aCannons[i].m_Entity.Get())
			{
				pCannon->GetMeshEntity()->GetComponent<RigidBodyComponent>()->SetEnabled(true);
			}
		}
	}

	void Boss1StateSpinAttack::Update()
	{
	}

	void Boss1StateSpinAttack::End(shoot::s32 iNextState)
	{
		for(u32 i=0; i<m_pActor->m_aCannons.size(); ++i)
		{
			if(Actor* pCannon = m_pActor->m_aCannons[i].m_Entity.Get())
			{
				pCannon->GetMeshEntity()->GetComponent<RigidBodyComponent>()->SetEnabled(false);
			}
		}
	}

	// Boss1StatePostSpinSetup
	void Boss1StatePostSpinSetup::Begin(shoot::s32 iPreviousState)
	{
		if(m_pActor->m_CurrentSide < 3)
		{
			++m_pActor->m_CurrentSide;
			m_fSrcRotation = m_pActor->GetMeshEntity()->GetRotation().Z;
			m_fDestRotation = 90.0f*m_pActor->m_CurrentSide;
			m_fInterpolator = 0.0f;
			m_bActive = true;
		}
		else
		{
			m_bActive = false;
		}
	}

	void Boss1StatePostSpinSetup::Update()
	{
		if(m_bActive)
		{
			Vector3 vRotation = m_pActor->GetMeshEntity()->GetRotation();
			if(m_fInterpolator < 1.0f)
			{
				vRotation.Z = Math::Lerp(m_fSrcRotation, m_fDestRotation, m_fInterpolator);
				m_pActor->GetMeshEntity()->SetRotation(vRotation);
				m_fInterpolator += g_fDeltaTime/m_pActor->m_fSideSwitchDuration;
			}
			else
			{
				vRotation.Z = m_fDestRotation;
				m_pActor->GetMeshEntity()->SetRotation(vRotation);
				m_pFSM->SwitchState(Boss1FSM::State_DualCannons);
			}			
		}
		else
		{
			// All cannons destroyed
			m_pFSM->SwitchState(Boss1FSM::State_Retreat);
		}
	}

	void Boss1StatePostSpinSetup::End(shoot::s32 iNextState)
	{
	}	

	// Boss1StateRetreat
	void Boss1StateRetreat::Begin(shoot::s32 iPreviousState)
	{
		m_pActor->GetMeshEntity()->AddComponent(static_cast<Component*>(m_pActor->m_Retreat->Copy()));

		ConversationManager::Instance()->AddPhrase(ConversationManager::T_Enemy, "I WILL BE BACK!", 3.5f);
		ConversationManager::Instance()->Start();
	}

	void Boss1StateRetreat::Update()
	{
		if(!ConversationManager::Instance()->IsActive())
		{
			GameManager::Instance()->OnLevelFinished();
		}
	}

	void Boss1StateRetreat::End(shoot::s32 iNextState)
	{
	}

	// Boss1StateExploding
	void Boss1StateExploding::Begin(shoot::s32 iPreviousState)
	{
		m_fTimer = SFXManager::Instance()->Explode(m_pActor);

		DelayVisitor* pDelay = snew DelayVisitor();
		pDelay->SetDuration(m_fTimer+.2f);
		SequenceVisitor* pSequence = snew SequenceVisitor();
		pSequence->AddVisitor(pDelay);
		pSequence->AddVisitor(snew RemoveEntityVisitor());
		m_pActor->AddComponent(pSequence);
		
		m_pActor->GetMeshEntity()->GetSubMeshes()[0]->GetComponent<RigidBodyComponent>()->SetEnabled(false);
	}

	void Boss1StateExploding::Update()
	{
		Player::Instance()->SetBaseSpeed(Vector3::Create(0.0f, 4.0f, 0.0f));

		if(m_fTimer > 0.0f)
		{
			m_fTimer -= g_fDeltaTime;
		}
		else
		{			
			m_pFSM->SwitchState(Boss1FSM::State_Inactive);
		}
	}

	void Boss1StateExploding::End(shoot::s32 iNextState)
	{
	}
}
