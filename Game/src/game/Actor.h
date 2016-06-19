/* 

Amine Rehioui
Created: October 10th 2011

*/

#ifndef _ACTOR_H_INCLUDED_
#define _ACTOR_H_INCLUDED_

#include "FSM.h"

#include "MeshEntity.h"

namespace shoot
{
	//! Base ActorSettings class
	class ActorSettings : public Object
	{
	public:
		
		DECLARE_OBJECT(ActorSettings, Object);

		//! constructor
		ActorSettings();

		//! serializes the object to/from a PropertyStream
		virtual void Serialize(PropertyStream& stream);

		u32 m_HitPoints;
	};

	//! Actor
	class Actor : public Entity3D
	{
	public:
		
		DECLARE_OBJECT(Actor, Entity3D);

		//! Constructor
		Actor();

		//! Destructor
		virtual ~Actor();

		//! serializes the entity to/from a PropertyStream
		virtual void Serialize(PropertyStream& stream);

		//! called during the initialization of the entity
		virtual void Init();

		//! called during the update of the entity		
		virtual void Update();

		//! returns the actor mesh entity
		MeshEntity* GetMeshEntity() { return m_Mesh; }

		//! initializes the actors from an ActorSettings object
		void Init(ActorSettings* pSettings);

		//! called when the actor has been shot
		virtual bool OnShot(const Vector3& vPosition, CollisionComponent* pCollisionComponent, f32 damage);

		//! sets immune status
		virtual void SetImmune(bool bImmune) { m_bImmune = bImmune; }

		//! returns the actor hit points
		inline f32 GetHitPoints() const { return m_HitPoints; }

		//! returns the drop item status
		inline bool CanDropItem() const { return m_bCanDropItem; }

		//! returns destructible status
		inline bool IsDestructible() const { return m_bDestructible; }

		//! returns immune status
		inline bool IsImmune() const { return m_bImmune; }

		//! returns true if this actor is a boss
		inline bool IsBoss() const { return m_bIsBoss; }

		//! returns the shake on explode status
		inline bool GetShakeOnExplode() const { return m_bShakeOnExplode; }

		//! returns the shake on shot status
		inline bool GetShakeOnShot() const { return m_bShakeOnShot; }

		//! returns the explosion scale factor
		inline f32 GetExplosionScaleFactor() const { return m_fExplosionScaleFactor; }

		//! returns reset rotation on explode status
		inline bool GetResetRotationOnExplode() const { return m_bResetRotationOnExplode; }

		//! returns respawn status
		inline bool GetCanRespawn() const { return m_bCanRespawn; }

		//! returns the actor settings
		ActorSettings* GetSettings() const { return m_Settings; }

		//! returns a spawning point
		virtual Vector3 GetSpawningPoint() const;

		//! returns the FSM
		inline FSMBase* GetFSM() const { return m_pFSM; }

	protected:

		FSMBase* m_pFSM;
		Reference<ActorSettings> m_Settings;
		Handle<MeshEntity> m_Mesh;

		// properties
		bool m_bCanDropItem;
		bool m_bDestructible;
		f32 m_HitPoints;
		bool m_bPlayfieldCheck;
		bool m_bImmune;
		bool m_bIsBoss;
		bool m_bShakeOnExplode;
		bool m_bShakeOnShot;
		bool m_bResetRotationOnExplode;
		f32 m_fExplosionScaleFactor;
		f32 m_fDamageDist;
		bool m_bCanRespawn;
	};
}

#endif // _ACTOR_H_INCLUDED_


