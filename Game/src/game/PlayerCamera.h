/* 

Amine Rehioui
Created: October 16th 2011

*/

#ifndef _PLAYER_CAMERA_H_INCLUDED_
#define _PLAYER_CAMERA_H_INCLUDED_

#include "Camera.h"

#include "CameraPath.h"

namespace shoot
{
	//! player camera
	class PlayerCamera : public Camera
	{
		//! Macro to provide type information
		DECLARE_OBJECT(PlayerCamera, Camera);

	public:

		//! constructor
		PlayerCamera();

		//! destructor
		virtual ~PlayerCamera();

		//! serializes the entity to/from a PropertyStream
		void Serialize(PropertyStream& stream);

		//! called during the initialization of the entity
		void Init();

		//! called during the update of the entity		
		void Update();

		//! resets the camera around the player
		void Reset();

		//! sets the follow amount
		inline void SetFollowAmount(f32 fAmount) { m_fFollowAmount = fAmount; }

		//! shakes the camera
		void Shake(f32 fIntensity, f32 fFrequency, f32 fDuration);

		//! stops shaking
		inline void StopShaking() { m_bIsShaking = false; }

		//! sets follow on X axis status
		inline void SetFollowOnXAxis(bool bFollow) { m_bFollowOnXAxis = bFollow; }

		//! sets follow on Y axis status
		inline void SetFollowOnYAxis(bool bFollow) { m_bFollowOnYAxis = bFollow; }

		//! returns the instance
		inline static PlayerCamera* Instance() { return ms_pInstance; }

	private:	

		static PlayerCamera* ms_pInstance;

		bool m_bFollowOnXAxis;
		bool m_bFollowOnYAxis;

		// camera shake
		static const u32 MaxNoiseSamples = 10;
		bool m_bIsShaking;
		f32 m_fShakeTimer;
		f32 m_fShakeDuration;
		f32 m_fInvShakeFrequency;
		f32 m_fShakeIntensity;
		f32 m_fNoiseSamplesX[MaxNoiseSamples];
		f32 m_fNoiseSamplesY[MaxNoiseSamples];
		f32 m_fNoiseIndex;
		Vector3 m_vShakeOffset;

		// properties
		Vector3 m_vPositionOffset;
		Vector3 m_vLookAtOffset;
		f32 m_fFollowAmount;
		f32 m_fFollowDuration;
	};
}

#endif // _PLAYER_CAMERA_H_INCLUDED_

