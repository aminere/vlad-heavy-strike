/* 

Amine Rehioui
Created: October 16th 2011

*/

#include "ShootTest.h"

#include "PlayerCamera.h"

#include "MeshEntity.h"

namespace shoot
{
	DEFINE_OBJECT(PlayerCamera);

	//! static vars
	PlayerCamera* PlayerCamera::ms_pInstance = NULL;

	//! constructor
	PlayerCamera::PlayerCamera()
		: m_bIsShaking(false)
		, m_bFollowOnXAxis(true)
		, m_bFollowOnYAxis(false)
		// properties
		, m_vPositionOffset(Vector3::Create(0.0f, -4.5f, 34.0f))
		, m_vLookAtOffset(Vector3::Create(0.0f, 9.0f, 5.0f))
		, m_fFollowAmount(0.35f)
		, m_fFollowDuration(0.35f)
	{
		// camera shake
		std::vector<s32> noiseSigns;
		for(u32 i=0; i<MaxNoiseSamples/2; ++i)
		{
			noiseSigns.push_back(1);
			noiseSigns.push_back(-1);
		}
		for(u32 i=0; i<MaxNoiseSamples; ++i)
		{
			u32 signIndex = Random::GetInt(0, noiseSigns.size()-1);
			m_fNoiseSamplesX[i] = Random::GetFloat(0.01f, 0.1f) * noiseSigns[signIndex];
			m_fNoiseSamplesY[i] = Random::GetFloat(0.01f, 0.1f) * noiseSigns[signIndex];
			noiseSigns.erase(noiseSigns.begin()+signIndex);
		}
	}

	//! destructor
	PlayerCamera::~PlayerCamera()
	{
		ms_pInstance = NULL;
	}

	//! serializes the entity to/from a PropertyStream
	void PlayerCamera::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);

		stream.Serialize(PT_Vec3, "PositionOffset", &m_vPositionOffset);
		stream.Serialize(PT_Vec3, "LookAtOffset", &m_vLookAtOffset);
		stream.Serialize(PT_Float, "FollowAmount", &m_fFollowAmount);
		stream.Serialize(PT_Float, "FollowDuration", &m_fFollowDuration);		
	}

	//! called during the initialization of the entity
	void PlayerCamera::Init()
	{
		super::Init();

		if(Player* pPlayer = Player::Instance())
		{
			Vector3 vPlayerMeshPos = pPlayer->GetMeshEntity()->GetTransformationMatrix().GetTranslation();
			SetPosition(vPlayerMeshPos + m_vPositionOffset);
			SetLookAt(vPlayerMeshPos + m_vLookAtOffset);
		}

		if(QuadTree* pQuadTree = GetRootContext()->GetQuadTree())
		{
			pQuadTree->SetCamera(this);
		}

		SHOOT_ASSERT(!ms_pInstance, "Multiple PlayerCamera instances detected");
		ms_pInstance = this;
	}

	//! called during the update of the entity	
	void PlayerCamera::Update()
	{
		Player* pPlayer = Player::Instance();
		Vector3 vBasePlayerPos = pPlayer->GetPosition();		

		Vector3 vDeltaPos = pPlayer->GetMeshEntity()->GetPosition() * m_fFollowAmount;
		vDeltaPos.X = m_bFollowOnXAxis ? vDeltaPos.X : 0.0f;
		vDeltaPos.Y = m_bFollowOnYAxis ? vDeltaPos.Y : 0.0f;
		Vector3 vLookAtDest = vBasePlayerPos + m_vLookAtOffset + vDeltaPos;
		Vector3 vLookAt = Math::Damp(m_vLookAt, vLookAtDest, g_fDeltaTime, m_fFollowDuration);
		SetLookAt(vLookAt);

		Vector3 vPosDest = vBasePlayerPos + m_vPositionOffset + vDeltaPos;
		Vector3 vPosition = Math::Damp(m_vPosition, vPosDest, g_fDeltaTime, m_fFollowDuration);
		SetPosition(vPosition);

		// apply shake
		if(m_bIsShaking)
		{
			if(m_fShakeTimer < m_fShakeDuration)
			{
				m_fNoiseIndex += g_fDeltaTime * m_fInvShakeFrequency;
				u32 noiseIndex = ((u32)m_fNoiseIndex) % MaxNoiseSamples;
				f32 fNoiseSampleX = m_fNoiseSamplesX[noiseIndex];
				f32 fNoiseSampleY = m_fNoiseSamplesY[noiseIndex];
				SetPosition(m_vPosition + Vector3::Create(-fNoiseSampleX, fNoiseSampleY, fNoiseSampleX)*m_fShakeIntensity);
				m_fShakeTimer += g_fDeltaTime;
			}
			else
			{
				m_bIsShaking = false;
			}
		}
	}

	//! resets the camera around the player
	void PlayerCamera::Reset()
	{
		const Vector3& vPlayerPos = Player::Instance()->GetPosition();
		SetLookAt(vPlayerPos + m_vLookAtOffset);
		SetPosition(vPlayerPos + m_vPositionOffset);
	}

	//! shakes the camera
	void PlayerCamera::Shake(f32 fIntensity, f32 fFrequency, f32 fDuration)
	{
		m_fShakeDuration = fDuration;
		m_fInvShakeFrequency = 1.0f/fFrequency;
		m_fShakeIntensity = fIntensity;
		m_fShakeTimer = 0.0f;
		m_fNoiseIndex = 0.0f;
		m_bIsShaking = true;
	}	
}

