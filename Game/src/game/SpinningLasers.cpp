/* 

Amine Rehioui
Created: July 26th 2013

*/

#include "ShootTest.h"

#include "SpinningLasers.h"

#include "Utils.h"

namespace shoot
{
	DEFINE_OBJECT(SpinningLasers);
	DEFINE_OBJECT(SpinningLasersSettings);
	
	//! constructor
	SpinningLasersSettings::SpinningLasersSettings()
		: m_fRotationSpeed(180.0f)
		, m_fShootFrequency(1.5f)
		, m_fLaserDuration(1.0f)
		, m_fLaserOffset(2.0f)
		, m_fLaserSpeed(30.0f)
		, m_fLaserLength(10.0f)
		, m_fDisappearOffset(10.0f)
		, m_fHaltTime(.3f)
		, m_Damage(50.0f)
	{
	}

	//! serializes the entity to/from a PropertyStream
	void SpinningLasersSettings::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);
		stream.Serialize(PT_Float, "RotationSpeed", &m_fRotationSpeed);
		stream.Serialize(PT_Float, "ShootFrequency", &m_fShootFrequency);
		stream.Serialize(PT_Float, "LaserDuration", &m_fLaserDuration);
		stream.Serialize(PT_Float, "LaserOffset", &m_fLaserOffset);
		stream.Serialize(PT_Float, "LaserSpeed", &m_fLaserSpeed);
		stream.Serialize(PT_Float, "LaserLength", &m_fLaserLength);
		stream.Serialize(PT_Float, "DisappearOffset", &m_fDisappearOffset);
		stream.Serialize(PT_Float, "HaltTime", &m_fHaltTime);
		stream.Serialize(PT_Float, "Damage", &m_Damage);
	}

	//! constructor
	SpinningLasers::SpinningLasers()
		: m_fLaserTimer(-1.0f)
		, m_fHaltTimer(-1.0f)
	{
	}

	//! destructor
	SpinningLasers::~SpinningLasers()
	{
	}

	//! called during the initialization of the entity
	void SpinningLasers::Init()
	{
		super::Init();

		if(SpinningLasersSettings* pSettings = static_cast<SpinningLasersSettings*>(m_Settings.Get()))
		{
			m_fLaserTimer = pSettings->m_fShootFrequency;
		}
	}

	//! called during the update of the entity	
	void SpinningLasers::Update()
	{
		super::Update();

		SpinningLasersSettings* pSettings = static_cast<SpinningLasersSettings*>(m_Settings.Get());
		if(GetTransformationMatrix().GetTranslation().Y+pSettings->m_fDisappearOffset < Player::Instance()->GetPosition().Y)
		{
			GetParent()->RemoveChild(this);
			return;
		}


		if(m_fLaserTimer < 0.0f)
		{
			// shoot
			Vector3 vDirections[4] = 
			{
				Vector3::Create(1.0f, 0.0f, 0.0f),
				Vector3::Create(-1.0f, 0.0f, 0.0f),
				Vector3::Create(0.0f, 1.0f, 0.0f),
				Vector3::Create(0.0f, -1.0f, 0.0f),
			};

			for(u32 i=0; i<4; ++i)
			{
				Vector3 vCannonDir = vDirections[i];
				vCannonDir.Rotate(GetRotation()*Math::DegToRadFactor);
				Vector3 vStart = GetTransformationMatrix().GetTranslation() + vCannonDir*pSettings->m_fLaserOffset;

				Laser::Params params;
				params.vStart = vStart;
				params.vDir = vCannonDir;
				params.fSpeed = pSettings->m_fLaserSpeed;
				params.fMaxLength = pSettings->m_fLaserLength;
				params.fLife = pSettings->m_fLaserDuration;
				params.damage = pSettings->m_Damage;
				params.fThickness = .55f;
				SFXManager::Instance()->GetEnemyLaserManager()->AddLaser(params);
			}

			m_fLaserTimer = pSettings->m_fShootFrequency;
			m_fHaltTimer = pSettings->m_fHaltTime;
		}
		else
		{
			m_fLaserTimer -= g_fDeltaTime;
		}

		if(m_fHaltTimer < 0.0f)
		{
			Vector3 vRotation = GetRotation();
			vRotation.Z += pSettings->m_fRotationSpeed * g_fDeltaTime;
			SetRotation(vRotation);
		}
		else
		{
			m_fHaltTimer -= g_fDeltaTime;
		}
	}	
}

