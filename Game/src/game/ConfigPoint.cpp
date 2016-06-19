/* 

Amine Rehioui
Created: February 22nd 2013

*/

#include "ShootTest.h"

#include "ConfigPoint.h"

#include "AIManager.h"

#include "PlayerCamera.h"

#include "GameManager.h"

namespace shoot
{
	DEFINE_OBJECT(ConfigPoint);

	//! Constructor
	ConfigPoint::ConfigPoint()
		: m_vSkyBoxRotation(Vector3::Zero)
		, m_Score(0)
		// properties
		, m_fPlayerMotionWidth(20.0f)
		, m_fSpawnWidth(20.0f)
		, m_fPlayerCameraFollowAmount(0.4f)
		, m_vBaseSpeed(Vector3::Create(0.0f, 4.0, 0.0f))
		, m_fFollowPathDuration(4.0f)
		, m_bCameraFollowOnXAxis(true)
		, m_bCameraFollowOnYAxis(false)
		, m_bAlwaysShowShield(false)
		, m_bCanRespawnHere(true)
	{
	}

	//! serializes the entity to/from a PropertyStream
	void ConfigPoint::Serialize(PropertyStream& stream)
	{
		Object::Serialize(stream);

		stream.Serialize(PT_Vec3, "Position", &m_vPosition);

		stream.Serialize(PT_Float, "PlayerMotionWidth", &m_fPlayerMotionWidth);
		stream.Serialize(PT_Float, "SpawnWidth", &m_fSpawnWidth);
		stream.Serialize(PT_Float, "PlayerCameraFollowAmount", &m_fPlayerCameraFollowAmount);
		stream.Serialize(PT_Vec3, "BaseSpeed", &m_vBaseSpeed);
		stream.Serialize(PT_Float, "FollowPathDuration", &m_fFollowPathDuration);
		stream.Serialize(PT_Color, "SkyBoxColor", &m_SkyBoxColor);
		stream.Serialize(PT_Bool, "CameraFollowOnXAxis", &m_bCameraFollowOnXAxis);
		stream.Serialize(PT_Bool, "CameraFollowOnYAxis", &m_bCameraFollowOnYAxis);
		stream.Serialize(PT_Bool, "AlwaysShowShield", &m_bAlwaysShowShield);
		stream.Serialize(PT_Bool, "CanRespawnHere", &m_bCanRespawnHere);

		m_bLocalTransformationMatrixDirty = true;
	}

	//! returns true if reached by the player
	bool ConfigPoint::IsReached()
	{
		Player* pPlayer = Player::Instance();
		PlayerCamera* pPlayerCamera = PlayerCamera::Instance();
		Vector3 vPlayerBasePos = pPlayer->GetPosition();
		const Matrix44& transform =  GetTransformationMatrix();
		
		f32 fDeltaPos = -1.0f;
		if(pPlayer->GetBaseSpeed().Y > 0.0f)
		{
			fDeltaPos = vPlayerBasePos.Y - transform.GetTranslation().Y;
		}
		else
		{
			if(pPlayer->GetBaseSpeed().X > 0.0f)
			{
				fDeltaPos = vPlayerBasePos.X - transform.GetTranslation().X;
			}
			else
			{
				fDeltaPos = transform.GetTranslation().X - vPlayerBasePos.X;
			}
		}

		return (fDeltaPos > 0.0f);
	}


	//! applies the checkpoint settings
	void ConfigPoint::Apply()
	{
		Player* pPlayer = Player::Instance();		
		pPlayer->SetSpawnWidth(m_fSpawnWidth);
		pPlayer->SetMotionWidth(m_fPlayerMotionWidth);		
		pPlayer->SetBaseSpeed(m_vBaseSpeed);
		pPlayer->SetAlwaysShowShield(m_bAlwaysShowShield);

		PlayerCamera* pPlayerCamera = PlayerCamera::Instance();		
		pPlayerCamera->SetFollowAmount(m_fPlayerCameraFollowAmount);
		pPlayerCamera->SetFollowOnXAxis(m_bCameraFollowOnXAxis);
		pPlayerCamera->SetFollowOnYAxis(m_bCameraFollowOnYAxis);
			
		if(m_SkyBoxColor != Color::White)
		{
			SFXManager::Instance()->SetSkyBoxColor(m_SkyBoxColor);
		}

		if(Path* pPath = static_cast<Path*>(GetChildByType("Path")))
		{
			Path* pPathCopy = static_cast<Path*>(pPath->Copy());
			pPathCopy->Translate(GetTransformationMatrix().GetTranslation());
			pPlayer->FollowPath(pPathCopy, m_fFollowPathDuration);
		}

		m_vSkyBoxRotation = pPlayer->GetSkyBoxRotation();
		m_Score = GameManager::Instance()->GetScore();
	}
}

