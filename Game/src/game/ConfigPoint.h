/* 

Amine Rehioui
Created: February 22nd 2013

*/

#ifndef _CONFIG_POINT_H_INCLUDED_
#define _CONFIG_POINT_H_INCLUDED_

#include "Path.h"

namespace shoot
{
	//! ConfigPoint class
	class ConfigPoint : public Entity3D
	{
	public:
		
		DECLARE_OBJECT(ConfigPoint, Entity3D);

		//! Constructor
		ConfigPoint();

		//! serializes the entity to/from a PropertyStream
		void Serialize(PropertyStream& stream);

		//! returns the basespeed
		inline const Vector3& GetBaseSpeed() const { return m_vBaseSpeed; }

		//! returns the skybox rotation
		inline const Vector3& GetSkyBoxRotation() const { return m_vSkyBoxRotation; }

		//! returns the score at this checkpoint
		inline s32 GetScore() const { return m_Score; }

		//! sets the score at this checkpoint
		inline void SetScore(s32 score) { m_Score = score; }

		//! applies the checkpoint settings
		void Apply();

		//! returns true if reached by the player
		bool IsReached();

		//! returns respawn status
		inline bool CanRespawnHere() const { return m_bCanRespawnHere; }

	protected:

		Vector3 m_vSkyBoxRotation;
		s32 m_Score;

		// properties
		f32 m_fPlayerMotionWidth;
		f32 m_fSpawnWidth;
		f32 m_fPlayerCameraFollowAmount;
		Vector3 m_vBaseSpeed;
		f32 m_fFollowPathDuration;
		Color m_SkyBoxColor;
		bool m_bCameraFollowOnXAxis;
		bool m_bCameraFollowOnYAxis;
		bool m_bAlwaysShowShield;
		bool m_bCanRespawnHere;
	};
}

#endif // _CONFIG_POINT_H_INCLUDED_


