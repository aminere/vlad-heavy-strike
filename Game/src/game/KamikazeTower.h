/* 

Amine Rehioui
Created: October 26th 2013

*/

#ifndef _KAMIKAZE_TOWER_H_INCLUDED_
#define _KAMIKAZE_TOWER_H_INCLUDED_

#include "Timer.h"
#include "BasicTower.h"

namespace shoot
{
	//! KamikazeTower settings
	class KamikazeTowerSettings : public BasicTowerSettings
	{
		DECLARE_OBJECT(KamikazeTowerSettings, BasicTowerSettings);

	public:

		//! constructor
		KamikazeTowerSettings();

		//! serializes the entity to/from a PropertyStream
		void Serialize(PropertyStream& stream);

		f32 m_fHomingFactor;
		f32 m_fHomingDuration;
		f32 m_fSpeed;
	};

	//! KamikazeTower class
	class KamikazeTower : public BasicTower
	{
	public:
		
		DECLARE_OBJECT(KamikazeTower, BasicTower);

		//! constructor
		KamikazeTower();

		//! called during the initialization of the entity
		virtual void Init();

		//! called during the update of the entity		
		virtual void Update();

		//! called when the actor has been shot
		bool OnShot(const Vector3& vPosition, CollisionComponent* pCollisionComponent, f32 damage);

	private:

		bool m_bKamikazeMode;
		Vector3 m_vDirection;
		f32 m_fTimer;
		Handle<Entity3D> m_Platform;
	};
}

#endif // _KAMIKAZE_TOWER_H_INCLUDED_


