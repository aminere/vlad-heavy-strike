/* 

Amine Rehioui
Created: August 18th 2013

*/

#include "ShootTest.h"

#include "LaserSwiper.h"

#include "Quaternion.h"

namespace shoot
{
	DEFINE_OBJECT(LaserSwiper);
	DEFINE_OBJECT(LaserSwiperSettings);
	
	//! constructor
	LaserSwiperSettings::LaserSwiperSettings()
		: m_fSpeed(4.0f)
		, m_fAimDuration(.3f)
		, m_fLaserThickness(1.0f)
		, m_fLaserOffset(1.0f)
		, m_fLaserSpeed(10.0f)
		, m_fLaserLength(10.0f)		
	{
	}

	//! serializes the entity to/from a PropertyStream
	void LaserSwiperSettings::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);
		
		stream.Serialize(PT_Float, "Speed", &m_fSpeed);
		stream.Serialize(PT_Float, "AimDuration", &m_fAimDuration);		
		stream.Serialize(PT_Float, "LaserThickness", &m_fLaserThickness);
		stream.Serialize(PT_Float, "LaserOffset", &m_fLaserOffset);
		stream.Serialize(PT_Float, "LaserSpeed", &m_fLaserSpeed);
		stream.Serialize(PT_Float, "LaserLength", &m_fLaserLength);
	}

	//! constructor
	LaserSwiper::LaserSwiper()
		: m_fLaserLengthSquared(0.0f)
		, m_pLaser(NULL)
		, m_fCurrentAngle(0.0f)
	{
	}

	//! destructor
	LaserSwiper::~LaserSwiper()
	{
		if(m_pLaser)
		{
			m_pLaser->fLife = 0.0f;
			m_pLaser = NULL;
		}
	}

	//! serializes the entity to/from a PropertyStream
	void LaserSwiper::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);
	}

	//! called during the initialization of the entity
	void LaserSwiper::Init()
	{
		super::Init();

		if(LaserSwiperSettings* pSettings = static_cast<LaserSwiperSettings*>(m_Settings.Get()))
		{
			m_fLaserLengthSquared = pSettings->m_fLaserLength * pSettings->m_fLaserLength;
		}

		if(Player* pPlayer = Player::Instance())
		{
			Vector3 vPosition = pPlayer->GetPosition() + GetSpawningPoint();
			SetAbsolutePosition(vPosition);
		}
	}

	//! called during the update of the entity	
	void LaserSwiper::Update()
	{
		super::Update();

		if(m_HitPoints < 0)
		{
			return;
		}

		LaserSwiperSettings* pSettings = static_cast<LaserSwiperSettings*>(m_Settings.Get());

		// aim
		Player* pPlayer = Player::Instance();
		Vector3 vPosition = GetTransformationMatrix().GetTranslation();			
		Vector3 vPlayerMeshPos = pPlayer->GetMeshEntity()->GetTransformationMatrix().GetTranslation();
		Vector3 vToPlayer = (vPlayerMeshPos-vPosition).Normalize();			
		f32 fDestAngle = Math::ATan2(vToPlayer.X, -vToPlayer.Y)*Math::RadToDegFactor;
		f32 fRotation = Math::Damp(GetRotation().Z, 180.0f-fDestAngle, g_fDeltaTime, pSettings->m_fAimDuration);
		m_fCurrentAngle = Math::Damp(m_fCurrentAngle, fDestAngle, g_fDeltaTime, pSettings->m_fAimDuration);
		SetRotation(Vector3::Create(0.0f, 0.0f, fRotation));

		f32 fDistSquared = (vPosition-vPlayerMeshPos).GetLengthSquared();
		if(fDistSquared < m_fLaserLengthSquared)
		{
			// swipe
			Vector3 vPosition = GetTransformationMatrix().GetTranslation();
			Vector3 vOffset = Vector3::Create(0.0f, -pSettings->m_fLaserOffset, 0.0f);
			Vector3 vRotatedOffset = Quaternion(Vector3::Create(0.0f, 0.0f, 1.0f), m_fCurrentAngle*Math::DegToRadFactor).Rotate(vOffset);

			if(!m_pLaser || m_pLaser->fLife < 0.0f)
			{
				// shoot
				Laser::Params params;					
				params.vStart = vPosition + vRotatedOffset;
				params.vDir = vRotatedOffset.Normalize();
				params.fSpeed = pSettings->m_fLaserSpeed;
				params.fMaxLength = pSettings->m_fLaserLength;
				params.fLife = 999.0f;
				params.bStatic = true;
				params.fThickness = pSettings->m_fLaserThickness;
				m_pLaser = SFXManager::Instance()->GetEnemyLaserManager()->AddLaser(params);
			}
			else
			{
				// update laser
				m_pLaser->vStart = vPosition + vRotatedOffset;
				m_pLaser->vDir = vRotatedOffset.Normalize();
			}
		}
		else
		{
			Translate(Vector3::Create(0.0f, -pSettings->m_fSpeed*g_fDeltaTime, 0.0f));

			if(m_pLaser)
			{
				m_pLaser->fLife = 0.0f;
				m_pLaser = NULL;
			}
		}
	}
}


