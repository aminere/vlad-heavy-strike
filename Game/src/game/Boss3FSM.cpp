/* 

Amine Rehioui
Created: August 13th 2013

*/

#include "ShootTest.h"

#include "Boss3FSM.h"

#include "ConversationManager.h"
#include "GameManager.h"
#include "PlayerCamera.h"

#include "Quaternion.h"

#include "AIManager.h"
#include "DelayVisitor.h"
#include "SequenceVisitor.h"
#include "RemoveEntityVisitor.h"

namespace shoot
{
	//! Initializes the state table
	void Boss3FSM::Init()
	{
		AddState(snew Boss3StateIntro(), State_Intro);
		AddState(snew Boss3StateIdle(), State_Idle);
		AddState(snew Boss3StateSweep(), State_Sweep);
		AddState(snew Boss3StateCoreShot(), State_CoreShot);
		AddState(snew Boss3StateRetreat(), State_Retreat);
		AddState(snew Boss3StateExploding(), State_Exploding);
		AddState(snew Boss3StateInactive(), State_Inactive);
		SwitchState(State_Intro);
	}	

	// Boss3StateIntro
	void Boss3StateIntro::Begin(shoot::s32 iPreviousState)
	{
	}

	void Boss3StateIntro::Update()
	{
		m_pFSM->SwitchState(Boss3FSM::State_Idle);
	}

	void Boss3StateIntro::End(shoot::s32 iNextState)
	{
	}

	// Boss3StateIdle
	void Boss3StateIdle::Begin(shoot::s32 iPreviousState)
	{
		m_fTimer = m_pActor->m_bSweepPhase ? m_pActor->m_fSweepFrequency : m_pActor->m_fCoreShotFrequency;
		Player::Instance()->SetInBossFight(true);
	}

	void Boss3StateIdle::Update()
	{
		m_pActor->UpdateRadials();		
	
		if(m_pActor->m_bSweepPhase 
		&& m_pActor->m_Cannon.IsValid()
		&& m_pActor->m_Cannon->GetHitPoints() >= 0.0f)
		{
			if(m_fTimer < 0.0f)
			{
				m_pFSM->SwitchState(Boss3FSM::State_Sweep);
				m_pActor->m_bSweepPhase = !m_pActor->m_bSweepPhase;
			}				
		}
		else if(m_pActor->m_Core.IsValid() 
			 && m_pActor->m_Core->GetHitPoints() >= 0.0f)
		{
			if(m_fTimer < 0.0f)
			{
				bool bRadialsLeft = m_pActor->GetClosestRadial() != NULL;
				m_pActor->m_Core->SetImmune(bRadialsLeft);
				m_pFSM->SwitchState(Boss3FSM::State_CoreShot);
				m_pActor->m_bSweepPhase = !m_pActor->m_bSweepPhase;
			}
		}
		else
		{
			if(m_pActor->m_bDestroyWhenDone)
			{
				m_pFSM->SwitchState(Boss3FSM::State_Exploding);
			}
			else
			{
				m_pFSM->SwitchState(Boss3FSM::State_Retreat);
			}
		}

		m_fTimer -= g_fDeltaTime;
	}

	void Boss3StateIdle::End(shoot::s32 iNextState)
	{
	}

	// Boss3StateSweep
	void Boss3StateSweep::Begin(shoot::s32 iPreviousState)
	{
		m_fLaserTimer = -1.0f;
		m_fInterpolator = 0.0f;
		m_vSrcPos = m_pActor->m_Cannon->GetPosition();
		m_vDestPos = m_vSrcPos + Vector3::Create(0.0f, -m_pActor->m_fCannonDestOffset, 0.0f);
		m_State = S_Deploy;
		m_pActor->m_Cannon->SetImmune(true);
	}

	void Boss3StateSweep::Update()
	{
		m_pActor->UpdateRadials();

		switch(m_State)
		{
		case S_Deploy:
		case S_Undeploy:
			if(m_fInterpolator < 1.0f)
			{
				Vector3 vPosition = Math::Lerp(m_vSrcPos, m_vDestPos, m_fInterpolator);
				m_pActor->m_Cannon->SetPosition(vPosition);
				m_fInterpolator += g_fDeltaTime/m_pActor->m_fCannonAnimDuration;

				if(m_State == S_Undeploy)
				{
					// restore cannon rotation
					f32 fRotation = Math::Damp(m_pActor->m_Cannon->GetRotation().Z, 0.0f, g_fDeltaTime, 0.3f);
					m_pActor->m_Cannon->SetRotation(Vector3::Create(0.0f, 0.0f, fRotation));
				}
			}
			else
			{
				m_pActor->m_Cannon->SetPosition(m_vDestPos);
				if(m_State == S_Deploy)
				{
					m_fTimer = m_pActor->m_fSweepDuration;					
					m_State = S_Sweep;
				}
				else
				{
					m_State = S_Done;
				}
			}
			break;

		case S_Sweep:
			if(m_fTimer > 0.0f)
			{
				// aim
				Player* pPlayer = Player::Instance();
				Vector3 vPosition = m_pActor->m_Cannon->GetTransformationMatrix().GetTranslation();			
				Vector3 vPlayerMeshPos = pPlayer->GetMeshEntity()->GetTransformationMatrix().GetTranslation();
				Vector3 vToPlayer = (vPlayerMeshPos-vPosition).Normalize();
				f32 fDestAngle = Math::ATan2(vToPlayer.X, -vToPlayer.Y)*Math::RadToDegFactor;				
				f32 fRotation = Math::Damp(m_pActor->m_Cannon->GetRotation().Z, -fDestAngle, g_fDeltaTime, m_pActor->m_fLaserAimDuration);				
				m_pActor->m_Cannon->SetRotation(Vector3::Create(0.0f, 0.0f, fRotation));

				// shoot
				if(m_fLaserTimer < 0.0f)
				{
					Vector3 vCannonPos = m_pActor->m_Cannon->GetTransformationMatrix().GetTranslation();
					Laser::Params params;
					Vector3 vOffset = Vector3::Create(0.0f, -m_pActor->m_fLaserOffset*4.0f, 0.0f);
					Vector3 vRotatedOffset = Quaternion(Vector3::Create(0.0f, 0.0f, 1.0f), -fRotation*Math::DegToRadFactor).Rotate(vOffset);
					params.vStart = vCannonPos + vRotatedOffset;
					params.vDir = vRotatedOffset.Normalize();
					params.fSpeed = m_pActor->m_fLaserSpeed;
					params.fMaxLength = m_pActor->m_fLaserLength;
					params.fLife = m_pActor->m_fLaserLife;
					params.fThickness = m_pActor->m_fLaserThickness;
					params.damage = 55.0f;
					SFXManager::Instance()->GetEnemyLaserManager()->AddLaser(params);
					m_fLaserTimer = m_pActor->m_fLaserFrequency;
				}

				m_fLaserTimer -= g_fDeltaTime;
				m_fTimer -= g_fDeltaTime;
			}
			else
			{
				m_fInterpolator = 0.0f;
				m_vSrcPos = m_pActor->m_Cannon->GetPosition();
				m_vDestPos = m_vSrcPos + Vector3::Create(0.0f, m_pActor->m_fCannonDestOffset, 0.0f);				
				m_State = S_Undeploy;
			}
			break;

		case S_Done:
			m_pFSM->SwitchState(Boss3FSM::State_Idle);
			break;
		}
	}

	void Boss3StateSweep::End(shoot::s32 iNextState)
	{
		m_pActor->m_Cannon->SetImmune(false);
	}

	// Boss3StateCoreShot
	void Boss3StateCoreShot::Begin(shoot::s32 iPreviousState)
	{
		m_State = S_Deploy;
		m_fInterpolator = 0.0f;
		m_fSrcAngle = 0.0f;
		m_fDestAngle = 90.0f;
	}

	void Boss3StateCoreShot::Update()
	{
		if(!m_pActor->m_Core.IsValid())
		{
			if(m_pActor->m_bDestroyWhenDone)
			{
				m_pFSM->SwitchState(Boss3FSM::State_Exploding);
			}
			else
			{
				m_pFSM->SwitchState(Boss3FSM::State_Retreat);
			}
			return;
		}

		m_pActor->UpdateRadials();

		switch(m_State)
		{
		case S_Deploy:
		case S_Undeploy:
			if(m_fInterpolator < 1.0f)
			{
				f32 fAngle = Math::Lerp(m_fSrcAngle, m_fDestAngle, m_fInterpolator);
				m_pActor->m_Lid1->SetRotation(Vector3::Create(-fAngle, 0.0f, 0.0f));
				m_pActor->m_Lid2->SetRotation(Vector3::Create(fAngle, 0.0f, 0.0f));
				m_fInterpolator += g_fDeltaTime/m_pActor->m_fCoreAnimDuration;
			}
			else
			{
				m_pActor->m_Lid1->SetRotation(Vector3::Create(-m_fDestAngle, 0.0f, 0.0f));
				m_pActor->m_Lid2->SetRotation(Vector3::Create(m_fDestAngle, 0.0f, 0.0f));
				if(m_State == S_Deploy)
				{
					m_State = S_Shoot;
					m_fTimer = m_pActor->m_fCoreShotDuration;
					m_fShootTimer = -1.0f;
				}
				else
				{
					m_pFSM->SwitchState(Boss3FSM::State_Idle);
				}
			}
			break;

		case S_Shoot:
			if(m_fTimer > 0.0f)
			{
				if(m_fShootTimer < 0.0f)
				{
					Vector3 vPosition = m_pActor->m_Core->GetTransformationMatrix().GetTranslation();			
					Vector3 vPlayerMeshPos = Player::Instance()->GetMeshEntity()->GetTransformationMatrix().GetTranslation();
					Vector3 vToPlayer = (vPlayerMeshPos-vPosition).Normalize();			

					Bullet::BulletParams params;
					params.vPosition = vPosition;
					params.vDirection = Vector3::Create(0.0f, 0.0f, 1.0f);
					params.fSpeed = m_pActor->m_fCoreShotSpeed;
					params.fLife = m_pActor->m_fCoreShotBulletLife;
					params.fRadius = m_pActor->m_fCoreShotBulletSize/2.0f;
					params.type = Bullet::T_Pellet;
					PelletManager* pManager = SFXManager::Instance()->GetEnemyPelletManager();
					pManager->SetMinDistToEnemy(m_pActor->m_fCoreShotMinDist);
					pManager->SetHomingFactor(m_pActor->m_fCoreShotHomingFactor);
					pManager->SetMaxHomingAngle(m_pActor->m_fCoreShotMaxHomingAngle);
					pManager->AddBullet(params);

					m_fShootTimer = m_pActor->m_fCoreShotBulletFrequency;
				}

				m_fShootTimer -= g_fDeltaTime;
			}
			else
			{
				m_fInterpolator = 0.0f;
				m_fSrcAngle = 90.0f;
				m_fDestAngle = 0.0f;
				m_State = S_Undeploy;
			}
			m_fTimer -= g_fDeltaTime;
			break;
		}
	}

	void Boss3StateCoreShot::End(shoot::s32 iNextState)
	{
	}
	
	// Boss3StateRetreat
	void Boss3StateRetreat::Begin(shoot::s32 iPreviousState)
	{
		m_pActor->GetMeshEntity()->AddComponent(static_cast<Component*>(m_pActor->m_Retreat->Copy()));

		ConversationManager::Instance()->AddPhrase(ConversationManager::T_Enemy, "I WILL BE BACK!", 3.5f);
		ConversationManager::Instance()->Start();

		Player::Instance()->SetInBossFight(false);
	}

	void Boss3StateRetreat::Update()
	{
		if(!ConversationManager::Instance()->IsActive())
		{
			GameManager::Instance()->OnLevelFinished();
		}
	}

	void Boss3StateRetreat::End(shoot::s32 iNextState)
	{
	}
	
	// Boss3StateExploding
	void Boss3StateExploding::Begin(shoot::s32 iPreviousState)
	{
		m_fTimer = SFXManager::Instance()->Explode(m_pActor);

		DelayVisitor* pDelay = snew DelayVisitor();
		pDelay->SetDuration(m_fTimer+.2f);
		SequenceVisitor* pSequence = snew SequenceVisitor();
		pSequence->AddVisitor(pDelay);
		pSequence->AddVisitor(snew RemoveEntityVisitor());
		m_pActor->AddComponent(pSequence);
				
		Player::Instance()->SetInBossFight(false);
	}

	void Boss3StateExploding::Update()
	{
		Player::Instance()->SetBaseSpeed(Vector3::Create(0.0f, 4.0f, 0.0f));

		if(m_fTimer > 0.0f)
		{
			m_fTimer -= g_fDeltaTime;
		}
		else
		{
			m_pFSM->SwitchState(Boss3FSM::State_Inactive);
		}
	}

	void Boss3StateExploding::End(shoot::s32 iNextState)
	{
	}	
}
