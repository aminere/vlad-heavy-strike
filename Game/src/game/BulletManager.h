/* 

Amine Rehioui
Created: February 5th 2012

*/

#ifndef _BULLET_MANAGER_H_INCLUDED_
#define _BULLET_MANAGER_H_INCLUDED_

#include "RenderableEntity.h"

#include "MemoryPool.h"

namespace shoot
{
	// forwards
	class VertexBuffer;

	//! Bullet class
	class Bullet
	{
	public:
		
		//! bullet type
		enum E_Type
		{
			T_Pulse,
			T_Pellet
		};

		//! bullet params
		struct BulletParams
		{
			BulletParams()
				: vPosition(Vector3::Zero)
				, vDirection(Vector3::Zero)
				, fSpeed(50.0f)
				, fLife(5.0f)
				, fRadius(.5f)
				, damage(30.0f)
				, type(T_Pulse)
			{
			}
			
			Vector3 vPosition;
			Vector3 vDirection;
			f32 fSpeed;
			f32 fLife;
			f32 fRadius;
			f32 damage;
			E_Type type;
		};

		//! Initialize
		void Init(const BulletParams& params)
		{
			vPosition = params.vPosition;
			vDirection = params.vDirection;
			fSpeed = params.fSpeed;
			fLife = params.fLife;
			fRadius = params.fRadius;
			damage = params.damage;
			type = params.type;
		}

		Vector3 vPosition;
		Vector3 vDirection;
		f32 fSpeed;
		f32 fLife;
		f32 fRadius;
		f32 damage;
		E_Type type;
	};

	//! BulletManager class
	class BulletManager : public RenderableEntity
	{
	public:
		
		DECLARE_OBJECT(BulletManager, RenderableEntity);

		//! Constructor
		BulletManager();

		//! serializes the entity to/from a PropertyStream
		virtual void Serialize(PropertyStream& stream);

		//! called during the initialization of the entity
		virtual void Init();

		//! called during the update of the entity		
		virtual void Update();

		//! Adds a bullet
		virtual void AddBullet(const Bullet::BulletParams& params);

		//! returns the bullet list
		inline const std::list<Bullet*>& GetBullets() const { return m_Bullets; }

		//! clears the bullets
		void Clear();

	protected:

		//! Prepares a bullet for rendering
		virtual void SetupRendering(Bullet* pBullet, u32 index, VertexBuffer* pVB) { }

		Reference<MemoryPool> m_Pool;
		std::list<Bullet*> m_Bullets;

		// properties
		u32 m_BulletPoolSize;
	};
}

#endif // _BULLET_MANAGER_H_INCLUDED_


