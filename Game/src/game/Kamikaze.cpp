/* 

Amine Rehioui
Created: November 05th 2011

*/

#include "ShootTest.h"

#include "Kamikaze.h"

namespace shoot
{
	DEFINE_OBJECT(Kamikaze);
	DEFINE_OBJECT(KamikazeSettings);	

	//! constructor
	KamikazeSettings::KamikazeSettings()
		: m_fDuration(1.5f)
	{
	}

	//! serializes the entity to/from a PropertyStream
	void KamikazeSettings::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);
		stream.Serialize(PT_Float, "Duration", &m_fDuration);
	}

	//! constructor
	Kamikaze::Kamikaze()		
		: m_vDirection(Vector3::Create(0.0f, -1.0f, 0.0f))
		, m_fTimer(1.5f)
	{
	}

	//! called during the initialization of the entity
	void Kamikaze::Init()
	{
		super::Init();

		if(Player* pPlayer = Player::Instance())
		{
			Vector3 vPosition = pPlayer->GetPosition() + GetSpawningPoint();
			SetAbsolutePosition(vPosition);
		}

		if(KamikazeSettings* pSettings = static_cast<KamikazeSettings*>(m_Settings.Get()))
		{
			m_fTimer = pSettings->m_fDuration;
		}
	}

	//! called during the update of the entity	
	void Kamikaze::Update()
	{
		super::Update();

		if(m_HitPoints < 0)
		{
			return;
		}

		KamikazeSettings* pSettings = static_cast<KamikazeSettings*>(m_Settings.Get());

		if(m_fTimer > 0.0f)
		{
			Vector3 vPosition = GetTransformationMatrix().GetTranslation();
			Vector3 vPlayerMeshPos = Player::Instance()->GetMeshEntity()->GetTransformationMatrix().GetTranslation();

			f32 fInterpolator = pSettings->m_fHomingFactor*g_fDeltaTime;
			fInterpolator = Math::Clamp(fInterpolator, 0.0f, 1.0f);
			Vector3 vDirectionToPlayer = (vPlayerMeshPos-vPosition).Normalize();
			m_vDirection = ((vDirectionToPlayer-m_vDirection)*fInterpolator + m_vDirection).Normalize();

			f32 fAngle = Math::ATan2(-m_vDirection.X, -m_vDirection.Y)*Math::RadToDegFactor;
			SetRotation(Vector3::Create(0.0f, 0.0f, fAngle));

			m_fTimer -= g_fDeltaTime;
		}

		Translate(m_vDirection*pSettings->m_fSpeed*g_fDeltaTime);
	}
}

