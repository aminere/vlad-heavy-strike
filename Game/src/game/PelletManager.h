/* 

Amine Rehioui
Created: May 1st 2013

*/

#ifndef _PELLET_MANAGER_H_INCLUDED_
#define _PELLET_MANAGER_H_INCLUDED_

#include "BulletManager.h"

namespace shoot
{
	// forwards
	class VertexBuffer;

	//! PelletManager class
	class PelletManager : public BulletManager
	{
	public:
		
		DECLARE_OBJECT(PelletManager, BulletManager);

		//! Constructor
		PelletManager();

		//! destructor
		virtual ~PelletManager();

		//! serializes the entity to/from a PropertyStream
		virtual void Serialize(PropertyStream& stream);

		//! called during the initialization of the entity
		virtual void Init();

		//! called during the update of the entity		
		virtual void Update();

		//! sets the bullet size
		void SetBulletSize(f32 fSize);

		//! sets the min dist to enemy
		inline void SetMinDistToEnemy(f32 fMinDist) { m_fMinDistToEnemy = fMinDist; }

		//! sets the homing factor
		inline void SetHomingFactor(f32 fFactor) { m_fHomingFactor = fFactor; }

		//! sets the max homing angle
		inline void SetMaxHomingAngle(f32 fAngle) 
		{
			m_fMaxHomingAngle = fAngle;
			m_fMinCos = Math::Cos(m_fMaxHomingAngle*Math::DegToRadFactor);
		}

	private:

		//! Prepares a bullet for rendering
		void SetupRendering(Bullet* pBullet, u32 index, VertexBuffer* pVB);

		//! returns info about the closest enemy with respect to a pellet
		bool GetClosestEnemy(const Vector3& vPosition, Vector3& vEnemyPos, f32& fDistToEnemy); 

		f32 m_fMinCos;

		//! properties
		f32 m_fBulletSize;
		f32 m_fMinDistToEnemy;
		f32 m_fHomingFactor;
		f32 m_fMaxHomingAngle;
		bool m_bOwnerIsPlayer;
	};
}

#endif // _PELLET_MANAGER_H_INCLUDED_


