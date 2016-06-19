/* 

Amine Rehioui
Created: November 05th 2011

*/

#include "ShootTest.h"

#include "FallingAsteroid.h"

namespace shoot
{
	DEFINE_OBJECT(FallingAsteroid);
	DEFINE_OBJECT(FallingAsteroidSettings);

	//! constructor
	FallingAsteroidSettings::FallingAsteroidSettings()
		: m_fHeight(30.0f)
		, m_fDestOffset(20.0f)
	{
	}

	//! serializes the entity to/from a PropertyStream
	void FallingAsteroidSettings::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);
		stream.Serialize(PT_Float, "Height", &m_fHeight);
		stream.Serialize(PT_Float, "DestOffset", &m_fDestOffset);
	}

	//! called during the initialization of the entity
	void FallingAsteroid::Init()
	{
		BasicEnemy::Init();

		if(FallingAsteroidSettings* pSettings = static_cast<FallingAsteroidSettings*>(m_Settings.Get()))
		{
			const f32 fSize = Random::GetFloat(pSettings->m_rSize.Min, pSettings->m_rSize.Max);
			const f32 fSizeRatio = (fSize-pSettings->m_rSize.Min)/(pSettings->m_rSize.Max-pSettings->m_rSize.Min);
			const f32 fAngle = Random::GetFloat(-pSettings->m_fAngleRange/2.0f, pSettings->m_fAngleRange/2.0f);				
			const f32 fSpeed = Random::GetFloat(pSettings->m_rSpeed.Min, pSettings->m_rSpeed.Max);
			
			Player* pPlayer = Player::Instance();
			m_HitPoints = Math::Lerp<f32, f32>(pSettings->m_rHitPoints.Min, pSettings->m_rHitPoints.Max, fSizeRatio);

			Vector3 vPosition = GetTransformationMatrix().GetTranslation();
			Vector3 vSrc = Vector3::Create(vPosition.X/2.0f, pPlayer->GetPosition().Y, pSettings->m_fHeight);			
			Vector3 vDest = Vector3::Create(vSrc.X*2.0f, vSrc.Y+pSettings->m_fDestOffset, 0.0f);
			m_vSpeed = ((vDest-vSrc).Normalize()*fSpeed).Rotate(Vector3::Create(0.0f, 0.0f, fAngle*Math::DegToRadFactor));
			m_vRotationOffset = Vector3::Create(Random::GetFloat(-pSettings->m_fMaxRotationOffset/2.0f, pSettings->m_fMaxRotationOffset/2.0f), 
												Random::GetFloat(-pSettings->m_fMaxRotationOffset/2.0f, pSettings->m_fMaxRotationOffset/2.0f),
												Random::GetFloat(-pSettings->m_fMaxRotationOffset/2.0f, pSettings->m_fMaxRotationOffset/2.0f));
			m_Mesh->SetScale(Vector3::One*fSize);
			m_Mesh->SetRotation(Vector3::Create(Random::GetFloat(-90.0f, 90.0f), Random::GetFloat(-90.0f, 90.0f), Random::GetFloat(-90.0f, 90.0f)));
			SetAbsolutePosition(vSrc);
		}
	}

	//! called during the update of the entity	
	void FallingAsteroid::Update()
	{
		Actor::Update();

		if(m_HitPoints < 0)
		{
			return;
		}

		Translate(m_vSpeed*g_fDeltaTime);
		m_Mesh->Rotate(m_vRotationOffset*g_fDeltaTime);
		if(GetPosition().Z < -static_cast<FallingAsteroidSettings*>(m_Settings.Get())->m_fHeight)
		{
			m_vSpeed.Y = 0.0f;
		}
	}
}

