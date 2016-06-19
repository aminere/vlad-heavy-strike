/* 

Amine Rehioui
Created: January 04th 2013

*/

#include "ShootTest.h"

#include "Asteroid.h"

#include "MeshEntity.h"

namespace shoot
{
	DEFINE_OBJECT(Asteroid);
	DEFINE_OBJECT(AsteroidSettings);

	//! constructor
	AsteroidSettings::AsteroidSettings()
		: m_rSize(0.6f, 2.0f)
		, m_rHitPoints(2.0f, 6.0f)
		, m_rSpeed(8.0f, 15.0f)
		, m_fAngleRange(60.0f)
		, m_fMaxRotationOffset(60.0f)
	{
	}

	//! serializes the entity to/from a PropertyStream
	void AsteroidSettings::Serialize(PropertyStream& stream)
	{
		stream.Serialize(PT_Range, "Size", &m_rSize);
		stream.Serialize(PT_Range, "HitPointsRange", &m_rHitPoints);
		stream.Serialize(PT_Range, "Speed", &m_rSpeed);
		stream.Serialize(PT_Float, "AngleRange", &m_fAngleRange);
		stream.Serialize(PT_Float, "MaxRotationOffset", &m_fMaxRotationOffset);		
	}

	//! Constructor
	Asteroid::Asteroid()
		: m_vSpeed(Vector3::Zero)
		, m_vRotationOffset(Vector3::Zero)
	{
	}

	//! called during the initialization of the entity
	void Asteroid::Init()
	{
		super::Init();

		if(AsteroidSettings* pSettings = static_cast<AsteroidSettings*>(m_Settings.Get()))
		{
			const f32 fSize = Random::GetFloat(pSettings->m_rSize.Min, pSettings->m_rSize.Max);
			const f32 fSizeRatio = (fSize-pSettings->m_rSize.Min)/(pSettings->m_rSize.Max-pSettings->m_rSize.Min);
			const f32 fAngle = Random::GetFloat(-pSettings->m_fAngleRange/2.0f, pSettings->m_fAngleRange/2.0f);				
			const f32 fSpeed = Random::GetFloat(pSettings->m_rSpeed.Min, pSettings->m_rSpeed.Max);
			
			Player* pPlayer = Player::Instance();
			m_HitPoints = Math::Lerp<f32, f32>(pSettings->m_rHitPoints.Min, pSettings->m_rHitPoints.Max, fSizeRatio);
			m_vSpeed = (Vector3::Create(0.0f, -1.0f, 0.0f)*fSpeed).Rotate(Vector3::Create(0.0f, 0.0f, fAngle*Math::DegToRadFactor));
			m_vRotationOffset = Vector3::Create(Random::GetFloat(-pSettings->m_fMaxRotationOffset/2.0f, pSettings->m_fMaxRotationOffset/2.0f), 
												Random::GetFloat(-pSettings->m_fMaxRotationOffset/2.0f, pSettings->m_fMaxRotationOffset/2.0f),
												Random::GetFloat(-pSettings->m_fMaxRotationOffset/2.0f, pSettings->m_fMaxRotationOffset/2.0f));
			m_Mesh->SetScale(Vector3::One*fSize);
			m_Mesh->SetRotation(Vector3::Create(Random::GetFloat(-90.0f, 90.0f), Random::GetFloat(-90.0f, 90.0f), Random::GetFloat(-90.0f, 90.0f)));
		}
	}

	//! called during the update of the entity
	void Asteroid::Update()
	{
		Actor::Update();

		if(m_HitPoints < 0)
		{
			return;
		}

		Translate(m_vSpeed*g_fDeltaTime);
		Rotate(m_vRotationOffset*g_fDeltaTime);
	}
}

