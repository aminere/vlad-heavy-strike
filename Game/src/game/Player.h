/* 

Amine Rehioui
Created: October 10th 2011

*/

#ifndef _PLAYER_H_INCLUDED_
#define _PLAYER_H_INCLUDED_

#include "RigidBodyComponent.h"

#include "Path.h"

#include "Event.h"

namespace shoot
{
	// fowards
	class PlayerCannons;	
	class Item;

	//! Player
	class Player : public Actor
	{
		friend class FSMState<Player>;

	public:
		
		DECLARE_OBJECT(Player, Actor);

		//! constructor
		Player();

		//! destructor
		virtual ~Player();

		//! serializes the entity to/from a PropertyStream
		virtual void Serialize(PropertyStream& stream);

		//! called during the initialization of the entity
		virtual void Init();

		//! sets the base speed
		inline void SetBaseSpeed(Vector3 vBaseSpeed) { m_vBaseSpeed = vBaseSpeed; }

		//! returns the player base speed
		inline const Vector3& GetBaseSpeed() const { return m_vBaseSpeed; }

		//! returns motion bounds
		inline const AABBox3D& GetMotionBounds() const { return m_MotionBounds; }		

		//! sets the motion width
		void SetMotionWidth(f32 fWidth);

		//! gets the motion width
		inline f32 GetMotionWidth() const { return m_MotionBounds.Size().X; }

		//! shoots
		void Shoot();

		//! called when the actor has been shot
		bool OnShot(const Vector3& vPosition, CollisionComponent* pCollisionComponent, f32 damage);

		//! called when an item has been collected
		void OnItemCollected(Item* pItem);

		//! follows a path
		void FollowPath(Path* pPath, f32 fDuration);

		//! gets the touch target
		bool GetTouchTarget(Vector3& vTarget) const;

		//! called when the player has just respawned
		void OnRespawn();

		//! sets bossfight status
		void SetInBossFight(bool bInBossFight);

		//! gets boss fight status
		inline bool IsInBossFight() const { return m_bInBossFight; }

		//! sets shield display status
		void SetAlwaysShowShield(bool bAlwaysShow);

		//! returns the skybox entity
		const Vector3& GetSkyBoxRotation() const { return m_vSkyBoxRotation; }

		//! sets the skybox rotation
		void SetSkyBoxRotation(const Vector3& vRotation);

		//! set the spawn width
		void SetSpawnWidth(f32 fWidth);

		//! get the spawn width
		inline f32 GetSpawnWidth() const { return m_fSpawnWidth; }

		//! get the spawning points
		inline std::vector<u32>& GetSpawningPoints() { return m_aSpawningPoints; }

		//! returns the instance
		inline static Player* Instance() { return ms_pInstance;	}

		//! shoot event listeners
		static void HandleTriggerEvent(void* pListener, Event* pEvent);

	// public for FSM

		static Player* ms_pInstance;

		//! common update
		void CommonUpdate();

		//! update the shield color
		void UpdateShieldColor();

		f32 m_fTilt;
		Vector3 m_vLocalSpeed;
		Vector3 m_vPreviousTargetPos;
		bool m_bPreviousTargetPosValid;
		Color m_ShieldColor;
		bool m_bAlwaysShowShield;

		Handle<PlayerCannons> m_Cannons;
		Handle<RigidBodyComponent> m_RigidBody;
		Reference<Path> m_PathToFollow;
		Handle<Entity> m_Shield;
		Handle<Material> m_ShieldMaterial;
		Handle<Material> m_ShieldBBMaterial;

		f32 m_fFollowPathDuration;
		f32 m_fFollowPathTime;
		Handle<Entity3D> m_SkyBox;
		Vector3 m_vSkyBoxRotation;
		bool m_bInBossFight;
		f32 m_fHealth;
		f32 m_fNoDamageTimer;
		f32 m_fFullHealthTimer;
		bool m_bHealthChanging;
		std::vector<u32> m_aSpawningPoints;

		// properties
		f32 m_fAcceleration;
		f32 m_fTouchAcceleration;
		f32 m_fDecelerationFactor;
		AABBox3D m_MotionBounds;
		Vector3 m_vBaseSpeed;
		f32 m_fMaxTilt;
		f32 m_fTiltDuration;
		f32 m_fSkyBoxRotationSpeed;
		f32 m_fClampDuration;
		f32 m_fRegenPerSecond;
		f32 m_fRegenDelay;
		f32 m_fHideHealthBarTime;
		f32 m_fContactDamagePerSecond;
		Color m_FullHealthColor;
		Color m_LowHealthColor;
		f32 m_fRespawnSpectateTime;
		f32 m_fSpawnWidth;
	};
}

#endif // _PLAYER_H_INCLUDED_


