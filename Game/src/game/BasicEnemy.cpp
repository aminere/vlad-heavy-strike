/* 

Amine Rehioui
Created: November 05th 2011

*/

#include "ShootTest.h"

#include "BasicEnemy.h"

namespace shoot
{
	DEFINE_OBJECT(BasicEnemy);
	DEFINE_OBJECT(BasicEnemySettings);	

	//! constructor
	BasicEnemySettings::BasicEnemySettings()
		: m_fHomingFactor(.5f)
		, m_fSpeed(16.0f)
	{
	}

	//! serializes the entity to/from a PropertyStream
	void BasicEnemySettings::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);

		stream.Serialize(PT_Float, "Speed", &m_fSpeed);
		stream.Serialize(PT_Float, "HomingFactor", &m_fHomingFactor);
	}

	//! called during the initialization of the entity
	void BasicEnemy::Init()
	{
		super::Init();

		if(Player* pPlayer = Player::Instance())
		{
			Vector3 vPosition = pPlayer->GetPosition() + GetSpawningPoint();
			SetAbsolutePosition(vPosition);
		}
	}

	//! called during the update of the entity	
	void BasicEnemy::Update()
	{
		super::Update();

		if(m_HitPoints < 0)
		{
			return;
		}

		BasicEnemySettings* pSettings = static_cast<BasicEnemySettings*>(m_Settings.Get());

		// orient towards player
		Vector3 vPlayerMeshPos = Player::Instance()->GetMeshEntity()->GetTransformationMatrix().GetTranslation();
		Vector3 vToPlayer = (vPlayerMeshPos-GetTransformationMatrix().GetTranslation()).Normalize();
		Vector3 vDirection = Math::Lerp(Vector3::Create(0.0f, -1.0f, 0.0f), vToPlayer, pSettings->m_fHomingFactor);
		vDirection.Y = Math::Clamp(vDirection.Y, -999.0f, -.5f);
		f32 fAngle = Math::ATan2(-vDirection.X, -vDirection.Y)*Math::RadToDegFactor;

		SetRotation(Vector3::Create(0.0f, 0.0f, fAngle));		
		Translate(vDirection*pSettings->m_fSpeed*g_fDeltaTime);

		// clamp position
		Vector3 vPosition = GetPosition();
		f32 fSpawnWidth = Player::Instance()->GetSpawnWidth();
		f32 fMinX = Player::Instance()->GetPosition().X-(fSpawnWidth/2.0f)+1.0f;
		f32 fMaxX = fMinX + (fSpawnWidth-2.0f);
		vPosition.X = Math::Clamp(vPosition.X, fMinX, fMaxX);
		SetPosition(vPosition);
	}
}

