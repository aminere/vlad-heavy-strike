/* 

Amine Rehioui
Created: November 18th 2011

*/

#include "ShootTest.h"

#include "BulletManager.h"

#include "GameManager.h"

namespace shoot
{
	DEFINE_OBJECT(BulletManager);

	//! Constructor
	BulletManager::BulletManager()
		// properties
		: m_BulletPoolSize(4096)
	{
	}

	//! serializes the entity to/from a PropertyStream
	void BulletManager::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);

		if(stream.Serialize(PT_UInt, "BulletPoolSize", &m_BulletPoolSize))
		{
			if(IsInitialized())
			{
				m_Pool = snew MemoryPool(m_BulletPoolSize);
				m_Bullets.clear();
			}
		}
	}

	//! called during the initialization of the entity
	void BulletManager::Init()
	{
		m_Pool = snew MemoryPool(m_BulletPoolSize);

		GraphicComponent* pGraphic = GetComponent<GraphicComponent>();
		if(!pGraphic)
		{
			pGraphic = snew GraphicComponent();
			pGraphic->SetRenderingPass(GraphicComponent::RP_Transparent3D);
			AddComponent(pGraphic, true);
		}

		super::Init();
	}

	//! called during the update of the entity	
	void BulletManager::Update()
	{
		VertexBuffer* pVB = GetComponent<GraphicComponent>()->GetVertexBuffer();
		pVB->SetNumVertices(0);

		if(m_Bullets.size())
		{
			u32 bulletIndex = 0;

			for(std::list<Bullet*>::iterator it = m_Bullets.begin(); it != m_Bullets.end();)
			{
				if((*it)->fLife > 0.0f && !GameManager::Instance()->IsOutOfPlayfield((*it)->vPosition))
				{
					(*it)->vPosition += (*it)->vDirection*(*it)->fSpeed*g_fDeltaTime;
					SetupRendering((*it), bulletIndex++, pVB);

					(*it)->fLife -= g_fDeltaTime;
					++it;
				}
				else
				{
					m_Pool->Free((*it));
					it = m_Bullets.erase(it);
				}
			}

			pVB->SetDirty(true);
		}		
	}

	//! Adds a bullet
	void BulletManager::AddBullet(const Bullet::BulletParams& params)
	{
		if(Bullet* pBullet = m_Pool->Alloc<Bullet>())
		{
			pBullet->Init(params);
			m_Bullets.push_back(pBullet);
		}
	}

	//! clears the bullets
	void BulletManager::Clear()
	{
		for(std::list<Bullet*>::iterator it = m_Bullets.begin(); it != m_Bullets.end(); ++it)
		{
			(*it)->fLife = 0.0f;
		}
	}
}

