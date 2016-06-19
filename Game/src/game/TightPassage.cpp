/* 

Amine Rehioui
Created: March 6th 2013

*/

#include "ShootTest.h"

#include "TightPassage.h"

#include "EventManager.h"

#include "Utils.h"

namespace shoot
{
	DEFINE_OBJECT(TightPassage);
	DEFINE_OBJECT(TightPassageSettings);

	u32 TightPassage::m_sNumCannons = 4;
	
	//! constructor
	TightPassageSettings::TightPassageSettings()
		: m_vLaserOffset(Vector3::Create(2.0f, 0.0f, 0.0f))
		, m_fLaserSpeed(40.0f)
		, m_fLaserDuration(1.0f)
	{
	}

	//! serializes the entity to/from a PropertyStream
	void TightPassageSettings::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);
		stream.Serialize(PT_Vec3, "LaserOffset", &m_vLaserOffset);
		stream.Serialize(PT_Float, "LaserSpeed", &m_fLaserSpeed);
		stream.Serialize(PT_Float, "LaserDuration", &m_fLaserDuration);
	}

	//! constructor
	TightPassage::TightPassage()
		: m_fLaserTimer(-1.0f)
	{
	}	

	//! destructor
	TightPassage::~TightPassage()
	{
		if(IsInitialized())
		{
			EventManager::Instance()->UnregisterListener<TriggerEvent>(this);
		}
	}

	//! called during the initialization of the entity
	void TightPassage::Init()
	{
		super::Init();

		EventManager::Instance()->RegisterListener<TriggerEvent>(this, &HandleTriggerEvent);

		m_aCannons.clear();
		for(u32 i=0; i<m_sNumCannons; ++i)
		{
			m_aCannons.push_back(static_cast<Entity3D*>(GetChildByName(std::string("Cannon") + Utils::ToString(i+1))));
		}
	}

	//! called during the update of the entity	
	void TightPassage::Update()
	{
		super::Update();

		if(GetTransformationMatrix().GetTranslation().Y+4.0f < Player::Instance()->GetPosition().Y)
		{
			GetParent()->RemoveChild(this);
			return;
		}

		if(m_fLaserTimer >= 0.0f)
		{
			m_fLaserTimer -= g_fDeltaTime;
		}
	}	

	//! shoot event listeners
	void TightPassage::HandleTriggerEvent(void* pListener, Event* pEvent)
	{
		TriggerEvent* pTrigger = static_cast<TriggerEvent*>(pEvent);
		TightPassage* pTightPassage = static_cast<TightPassage*>(pListener);
		
		if((pTightPassage->m_fLaserTimer < 0.0f)
		&& pTightPassage->HasChild(static_cast<Entity*>(pTrigger->Target.Get())))
		{
			TightPassageSettings* pSettings = static_cast<TightPassageSettings*>(pTightPassage->m_Settings.Get());
			pTightPassage->m_fLaserTimer = pSettings->m_fLaserDuration;

			for(u32 i=0; i<pTightPassage->m_aCannons.size(); i+=2)
			{
				Entity3D* pLeftCannon = pTightPassage->m_aCannons[i];
				Entity3D* pRightCannon = pTightPassage->m_aCannons[i+1];

				Vector3 vStart = pLeftCannon->GetTransformationMatrix().GetTranslation()+pSettings->m_vLaserOffset;
				Vector3 vEnd = pRightCannon->GetTransformationMatrix().GetTranslation()-pSettings->m_vLaserOffset;
				Laser::Params params;
				params.vStart = vStart;
				params.fMaxLength = (vEnd - vStart).GetLength();
				params.vDir = (vEnd - vStart) / params.fMaxLength;
				params.fSpeed = pSettings->m_fLaserSpeed;
				params.bStatic = true;
				params.fLife = pSettings->m_fLaserDuration;
				params.damage = 9999;
				SFXManager::Instance()->GetEnemyLaserManager()->AddLaser(params);
			}
		}
	}
}

