/* 

Amine Rehioui
Created: August 20th 2013

*/

#include "ShootTest.h"

#include "Boss4FSM.h"

#include "ConversationManager.h"
#include "GameManager.h"

#include "AIManager.h"
#include "DelayVisitor.h"
#include "SequenceVisitor.h"
#include "RemoveEntityVisitor.h"

namespace shoot
{
	//! Initializes the state table
	void Boss4FSM::Init()
	{
		AddState(snew Boss4StateIntro(), State_Intro);
		AddState(snew Boss4StateIdle(), State_Idle);
		AddState(snew Boss4StateLasers(), State_Lasers);
		AddState(snew Boss4StateMissiles(), State_Missiles);
		AddState(snew Boss4StateRetreat(), State_Retreat);
		AddState(snew Boss4StateExploding(), State_Exploding);
		AddState(snew Boss4StateInactive(), State_Inactive);
		SwitchState(State_Intro);
	}	

	// Boss4StateIntro
	void Boss4StateIntro::Begin(shoot::s32 iPreviousState)
	{
	}

	void Boss4StateIntro::Update()
	{
		m_pFSM->SwitchState(Boss4FSM::State_Idle);
	}

	void Boss4StateIntro::End(shoot::s32 iNextState)
	{
	}

	// Boss4StateIdle
	void Boss4StateIdle::Begin(shoot::s32 iPreviousState)
	{
		m_fTimer = m_pActor->m_bLaserPhase ? m_pActor->m_fLaserPhaseDelay : m_pActor->m_fMissilePhaseDelay;
		Player::Instance()->SetInBossFight(true);
	}

	void Boss4StateIdle::Update()
	{
		if(m_pActor->GetNumLaserCannons() > 0 || m_pActor->GetNumMissileCannons() > 0)
		{
			if(!m_pActor->m_Tower.IsValid())
			{
				m_pActor->m_Tower = static_cast<Actor*>(m_pActor->GetChildByType("BasicTower"));
			}

			if(m_fTimer < 0.0f)
			{
				if(m_pActor->m_bLaserPhase && m_pActor->GetNumLaserCannons() > 0)
				{
					m_pFSM->SwitchState(Boss4FSM::State_Lasers);
				}
				else
				{
					m_pFSM->SwitchState(Boss4FSM::State_Missiles);
				}

				m_pActor->m_bLaserPhase = !m_pActor->m_bLaserPhase;
			}
			m_fTimer -= g_fDeltaTime;
		}
		else
		{
			if(Actor* pTower = m_pActor->m_Tower.Get())
			{
				pTower->SetImmune(false);
			}
			else
			{
				if(m_pActor->m_bDestroyWhenDone)
				{
					m_pFSM->SwitchState(Boss4FSM::State_Exploding);
				}
				else
				{
					m_pFSM->SwitchState(Boss4FSM::State_Retreat);
				}
			}
		}
	}

	void Boss4StateIdle::End(shoot::s32 iNextState)
	{
	}

	// Boss4StateLasers
	void Boss4StateLasers::Begin(shoot::s32 iPreviousState)
	{
		m_fTimer = m_pActor->m_fLaserPhaseDuration;
		m_fShootTimer = -1.0f;

		for(u32 i=0; i<m_pActor->m_sNumLaserCannons; ++i)
		{
			Actor* pCannon = m_pActor->m_LaserCannons[i].Get();
			if(pCannon && pCannon->GetHitPoints() >= 0.0f)
			{
				m_vOriginalCannonPos[i] = pCannon->GetPosition();				
			}
			else
			{
				m_vOriginalCannonPos[i] = Vector3::Zero;
			}
		}
	}

	void Boss4StateLasers::Update()
	{
		if(m_pActor->GetNumLaserCannons() > 0)
		{
			if(m_fTimer > 0.0f)
			{
				if(m_fShootTimer < 0.0f)
				{
					// SHOOT
					for(u32 i=0; i<m_pActor->m_sNumLaserCannons; ++i)
					{
						Actor* pCannon = m_pActor->m_LaserCannons[i].Get();
						if(pCannon && pCannon->GetHitPoints() >= 0.0f)
						{							
							Vector3 vCannonPos = pCannon->GetTransformationMatrix().GetTranslation();
							Laser::Params params;
							Vector3 vOffset = Vector3::Create(0.0f, -m_pActor->m_fLaserOffset, 0.0f);							
							params.vStart = vCannonPos + vOffset;
							params.vDir = Vector3::Create(0.0f, -1.0f, 0.0f);
							params.fSpeed = m_pActor->m_fLaserSpeed;
							params.fMaxLength = m_pActor->m_fLaserLength;
							params.fLife = m_pActor->m_fLaserLife;
							params.fThickness = m_pActor->m_fLaserThickness;
							params.damage = 9999.0f;
							SFXManager::Instance()->GetEnemyLaserManager()->AddLaser(params);

							pCannon->SetPosition(m_vOriginalCannonPos[i] + Vector3::Create(0.0f, -.5f, 0.0f));
						}
					}
					m_fShootTimer = m_pActor->m_fLaserFrequency;					
				}
				else
				{
					// update cannons position
					for(u32 i=0; i<m_pActor->m_sNumLaserCannons; ++i)
					{
						Actor* pCannon = m_pActor->m_LaserCannons[i].Get();
						if(pCannon && pCannon->GetHitPoints() >= 0.0f)
						{
							Vector3 vPosition = pCannon->GetPosition();
							vPosition = Math::Damp(vPosition, m_vOriginalCannonPos[i], g_fDeltaTime, .3f);
							pCannon->SetPosition(vPosition);
						}
					}
				}

				m_fShootTimer -= g_fDeltaTime;
				m_fTimer -= g_fDeltaTime;
			}
			else
			{
				for(u32 i=0; i<m_pActor->m_sNumLaserCannons; ++i)
				{
					Actor* pCannon = m_pActor->m_LaserCannons[i].Get();
					if(pCannon && pCannon->GetHitPoints() >= 0.0f)
					{						
						pCannon->SetPosition(m_vOriginalCannonPos[i]);
					}
				}

				m_pFSM->SwitchState(Boss4FSM::State_Idle);				
			}
		}
		else
		{
			m_pFSM->SwitchState(Boss4FSM::State_Idle);
		}
	}

	void Boss4StateLasers::End(shoot::s32 iNextState)
	{
	}
	
	// Boss4StateMissiles
	void Boss4StateMissiles::Begin(shoot::s32 iPreviousState)
	{
		m_fTimer = m_pActor->m_fMissilePhaseDuration;
		m_fShootTimer = -1.0f;

		for(u32 i=0; i<m_pActor->m_sNumMissileCannons; ++i)
		{
			Actor* pCannon = m_pActor->m_MissileCannons[i].Get();
			if(pCannon && pCannon->GetHitPoints() >= 0.0f)
			{
				m_vOriginalCannonPos[i] = pCannon->GetPosition();				
			}
			else
			{
				m_vOriginalCannonPos[i] = Vector3::Zero;
			}
		}
	}

	void Boss4StateMissiles::Update()
	{
		if(m_pActor->GetNumMissileCannons() > 0)
		{
			if(m_fTimer > 0.0f)
			{
				if(m_fShootTimer < 0.0f)
				{
					// SHOOT
					for(u32 i=0; i<m_pActor->m_sNumMissileCannons; ++i)
					{
						Actor* pCannon = m_pActor->m_MissileCannons[i].Get();
						if(pCannon && pCannon->GetHitPoints() >= 0.0f)
						{							
							Vector3 vPosition = pCannon->GetTransformationMatrix().GetTranslation();			
							Vector3 vPlayerMeshPos = Player::Instance()->GetMeshEntity()->GetTransformationMatrix().GetTranslation();
							Vector3 vToPlayer = (vPlayerMeshPos-vPosition).Normalize();			

							Bullet::BulletParams params;
							params.vPosition = vPosition + Vector3::Create(0.0f, -m_pActor->m_fMissileOffset, 0.0f);
							params.vDirection = Vector3::Create(0.0f, -1.0f, 0.0f);
							params.fSpeed = m_pActor->m_fMissileSpeed;
							params.fLife = m_pActor->m_fMissileLife;
							params.fRadius = m_pActor->m_fMissileSize/2.0f;
							params.type = Bullet::T_Pellet;
							params.damage = 35.0f;
							PelletManager* pManager = SFXManager::Instance()->GetEnemyPelletManager();
							pManager->SetMinDistToEnemy(m_pActor->m_fMissileMinDist);
							pManager->SetHomingFactor(m_pActor->m_fMissileHomingFactor);
							pManager->SetMaxHomingAngle(m_pActor->m_fMissileMaxHomingAngle);
							pManager->AddBullet(params);

							pCannon->SetPosition(m_vOriginalCannonPos[i] + Vector3::Create(0.0f, -.5f, 0.0f));
						}
					}
					m_fShootTimer = m_pActor->m_fLaserFrequency;					
				}
				else
				{
					// update cannons position
					for(u32 i=0; i<m_pActor->m_sNumMissileCannons; ++i)
					{
						Actor* pCannon = m_pActor->m_MissileCannons[i].Get();
						if(pCannon && pCannon->GetHitPoints() >= 0.0f)
						{
							Vector3 vPosition = pCannon->GetPosition();
							vPosition = Math::Damp(vPosition, m_vOriginalCannonPos[i], g_fDeltaTime, .3f);
							pCannon->SetPosition(vPosition);
						}
					}
				}

				m_fShootTimer -= g_fDeltaTime;
				m_fTimer -= g_fDeltaTime;
			}
			else
			{
				for(u32 i=0; i<m_pActor->m_sNumMissileCannons; ++i)
				{
					Actor* pCannon = m_pActor->m_MissileCannons[i].Get();
					if(pCannon && pCannon->GetHitPoints() >= 0.0f)
					{
						pCannon->SetPosition(m_vOriginalCannonPos[i]);
					}
				}

				m_pFSM->SwitchState(Boss4FSM::State_Idle);				
			}
		}
		else
		{
			m_pFSM->SwitchState(Boss4FSM::State_Idle);
		}
	}

	void Boss4StateMissiles::End(shoot::s32 iNextState)
	{
	}
	
	// Boss4StateRetreat
	void Boss4StateRetreat::Begin(shoot::s32 iPreviousState)
	{
		m_pActor->GetMeshEntity()->AddComponent(static_cast<Component*>(m_pActor->m_Retreat->Copy()));

		ConversationManager::Instance()->AddPhrase(ConversationManager::T_Enemy, "I WILL BE BACK!", 3.5f);
		ConversationManager::Instance()->Start();
		Player::Instance()->SetInBossFight(false);
	}

	void Boss4StateRetreat::Update()
	{
		if(!ConversationManager::Instance()->IsActive())
		{
			GameManager::Instance()->OnLevelFinished();
		}
	}

	void Boss4StateRetreat::End(shoot::s32 iNextState)
	{
	}
	
	// Boss4StateExploding
	void Boss4StateExploding::Begin(shoot::s32 iPreviousState)
	{
		m_fTimer = SFXManager::Instance()->Explode(m_pActor);

		DelayVisitor* pDelay = snew DelayVisitor();
		pDelay->SetDuration(m_fTimer+.2f);
		SequenceVisitor* pSequence = snew SequenceVisitor();
		pSequence->AddVisitor(pDelay);
		pSequence->AddVisitor(snew RemoveEntityVisitor());
		m_pActor->AddComponent(pSequence);
				
		Player::Instance()->SetInBossFight(false);
		m_pActor->GetMeshEntity()->GetComponent<RigidBodyComponent>()->SetEnabled(false);
	}

	void Boss4StateExploding::Update()
	{
		Player::Instance()->SetBaseSpeed(Vector3::Create(0.0f, 4.0f, 0.0f));

		if(m_fTimer > 0.0f)
		{
			m_fTimer -= g_fDeltaTime;
		}
		else
		{
			GameManager::Instance()->OnLevelFinished();
			m_pFSM->SwitchState(Boss4FSM::State_Inactive);
		}
	}

	void Boss4StateExploding::End(shoot::s32 iNextState)
	{
	}
}

