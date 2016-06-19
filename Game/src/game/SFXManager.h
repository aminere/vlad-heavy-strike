/* 

Amine Rehioui
Created: May 18th 2013

*/

#ifndef _SFX_MANAGER_H_INCLUDED_
#define _SFX_MANAGER_H_INCLUDED_

#include "PulseManager.h"
#include "LaserManager.h"
#include "PelletManager.h"

namespace shoot
{
	// fowards
	class Bullet;
	class Laser;
	class Actor;

	//! SFXManager
	class SFXManager : public Entity
	{
		DECLARE_OBJECT(SFXManager, Entity);

	public:

		//! constructor
		SFXManager();

		//! destructor
		virtual ~SFXManager();

		//! serializes the entity to/from a PropertyStream
		void Serialize(PropertyStream& stream);

		//! called during the initialization of the entity
		void Init();

		//! called during the update of the entity
		void Update();

		//! called when a bullet collided
		void OnBulletCollided(Bullet* pBullet);

		//! called when a laser collided
		void OnLaserCollided(const Vector3& vIntersection);

		//! called when an actor was shot
		void OnActorShot(Actor* pActor, const Vector3& vPosition);

		//! called when an actor is destroyed
		void OnActorDestroyed(Actor* pActor);

		//! Makes an Actor explode
		f32 Explode(Actor* pActor);

		//! animates the skybox color
		void SetSkyBoxColor(const Color& color);

		//! returns the player PulseManager instance
		PulseManager* GetPlayerPulseManager() { return m_PlayerPulseManager; }

		//! returns the player SidePulseManager instance
		PulseManager* GetPlayerSidePulseManager() { return m_PlayerSidePulseManager; }

		//! return the player LaserManager
		LaserManager* GetPlayerLaserManager() { return m_PlayerLaserManager; }

		//! return the player SideLaserManager
		LaserManager* GetPlayerSideLaserManager() { return m_PlayerSideLaserManager; }

		//! return the player PelletManager
		PelletManager* GetPlayerPelletManager() { return m_PlayerPelletManager; }

		//! return the player SidePelletManager
		PelletManager* GetPlayerSidePelletManager() { return m_PlayerSidePelletManager; }

		//! return the enemy PulseManager
		PulseManager* GetEnemyPulseManager() { return m_EnemyPulseManager; }

		//! return the boss PulseManager
		PulseManager* GetBossPulseManager() { return m_BossPulseManager; }

		//! return the enemy LaserManager
		LaserManager* GetEnemyLaserManager() { return m_EnemyLaserManager; }

		//! return the enemy PelletManager
		PelletManager* GetEnemyPelletManager() { return m_EnemyPelletManager; }

		//! clears all effects
		void Clear();
			
		//! returns the instance
		inline static SFXManager* Instance() { return ms_pInstance;	}

	private:

		static SFXManager* ms_pInstance;
		Handle<Entity> m_Root;

		Handle<PulseManager> m_PlayerPulseManager;
		Handle<LaserManager> m_PlayerLaserManager;
		Handle<PelletManager> m_PlayerPelletManager;
		Handle<PulseManager> m_PlayerSidePulseManager;
		Handle<LaserManager> m_PlayerSideLaserManager;
		Handle<PelletManager> m_PlayerSidePelletManager;
		Handle<PulseManager> m_EnemyPulseManager;
		Handle<PulseManager> m_BossPulseManager;		
		Handle<LaserManager> m_EnemyLaserManager;
		Handle<PelletManager> m_EnemyPelletManager;
		f32 m_fExplosionTracker;

		//! returns the ExplosionVisitor corresponding to a template
		Object* GetExplosionVisitor(const std::string& templatePath);

		//! returns an object from the preloaded pool
		Object* GetPreloadedObject(const char* strPath);

		//! updates the skybox
		void UpdateSkyBox();
		
		// skybox anim
		Color m_SrcSkyBoxColor;
		Color m_DestSkyBoxColor;
		f32 m_fInterpolator;
		bool m_bSkyBoxAnim;
		Handle<Material> m_SkyBoxMaterial;		

		//! ExplosionInfo
		struct ExplosionInfo : public ISerializableStruct
		{
			//! Reads/Writes struct properties from/to a stream
			void Serialize(PropertyStream& stream)
			{
				stream.SerializeReference("Explosion", &m_Explosion);
				stream.SerializeArray("TemplatePaths", &m_TemplatePaths, PT_File);
			}

			Reference<Object> m_Explosion;
			Array<std::string> m_TemplatePaths;
		};

		// properties
		f32 m_fExplosionIntensity;
		f32 m_fExplosionDuration;
		f32 m_fSkyBoxAnimDuration;
		f32 m_fCameraShakeIntensity;
		f32 m_fCameraShakeFrequency;
		Array< Reference<Object> > m_PreloadedObjects;
		Array<ExplosionInfo> m_ExplosionInfos;
	};
}

#endif // _SFX_MANAGER_H_INCLUDED_

