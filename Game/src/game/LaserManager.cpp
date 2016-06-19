/* 

Amine Rehioui
Created: March 5th 2013

*/

#include "ShootTest.h"

#include "LaserManager.h"

#include "MaterialProvider.h"

#include "Quaternion.h"

namespace shoot
{
	DEFINE_OBJECT(LaserManager);

	//! Constructor
	LaserManager::LaserManager()
		// properties
		: m_PoolSize(4096)
	{
	}

	//! destructor
	LaserManager::~LaserManager()
	{
	}

	//! serializes the entity to/from a PropertyStream
	void LaserManager::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);

		if(stream.Serialize(PT_UInt, "PoolSize", &m_PoolSize))
		{
			if(IsInitialized())
			{
				m_Pool = snew MemoryPool(m_PoolSize);
				m_Lasers.clear();
			}
		}
	}

	//! called during the initialization of the entity
	void LaserManager::Init()
	{
		m_Pool = snew MemoryPool(m_PoolSize);

		GraphicComponent* pGraphic = GetComponent<GraphicComponent>();
		if(!pGraphic)
		{
			pGraphic = snew GraphicComponent();
			pGraphic->SetRenderingPass(GraphicComponent::RP_Transparent3D);
			AddComponent(pGraphic, true);
		}

		if(!pGraphic->GetMaterial())
		{
			pGraphic->SetMaterial(snew Material());
			pGraphic->GetMaterial()->SetCullMode(GraphicsDriver::CM_None);
		}

		u32 maxLasers = m_PoolSize/sizeof(Laser);
		u32 maxVertices = maxLasers*24;
		Vertex3D* pVertices = snew Vertex3D[maxVertices];
		VertexBuffer* pVB = GraphicsDriver::Instance()->CreateVertexBuffer();
		pVB->SetVertices(pVertices, maxVertices);
		pVB->SetVertexFlag(Vertex3D::VF_Pos);
		pVB->SetVertexFlag(Vertex3D::VF_UV);
		pVB->SetPrimitiveType(GraphicsDriver::PT_Triangle);		
		pVB->SetName(GetClassName());
		pGraphic->SetVertexBuffer(pVB);

		pVB->SetDynamic(true);

		super::Init();
	}	

	//! called during the update of the entity	
	void LaserManager::Update()
	{
		VertexBuffer* pVB = GetComponent<GraphicComponent>()->GetVertexBuffer();
		pVB->SetNumVertices(0);

		if(m_Lasers.size())
		{
			u32 index = 0;
			Vertex3D* pVertices = pVB->GetVertices();

			for(std::list<Laser*>::iterator it = m_Lasers.begin(); it != m_Lasers.end();)
			{
				Vector3 vLateralAxe = (*it)->vDir.CrossProduct(EntityRenderer::Instance()->Get3DCamera()->GetUpVector());
				Vector3 vVerticalAxe = vLateralAxe.CrossProduct((*it)->vDir);
				vLateralAxe = vLateralAxe*(*it)->fThickness;
				vVerticalAxe = vVerticalAxe*(*it)->fThickness;
				Vector3 vDiagonalAxe = Quaternion((*it)->vDir, Math::PI/4.0f).Rotate(vLateralAxe);
				Vector3 vDiagonalAxe2 = Vector3::Create(vDiagonalAxe.X, vDiagonalAxe.Y, -vDiagonalAxe.Z);
				Vector3 vEnd = (*it)->vStart + (*it)->vDir * (*it)->fLength;

				pVertices[index+0].UV = Vector2(0.0f, 0.0f); pVertices[index+0].Pos = vEnd+vLateralAxe;
				pVertices[index+1].UV = Vector2(1.0f, 0.0f); pVertices[index+1].Pos = (*it)->vStart+vLateralAxe;
				pVertices[index+2].UV = Vector2(0.0f, 1.0f); pVertices[index+2].Pos = vEnd-vLateralAxe;
				pVertices[index+3].UV = Vector2(0.0f, 1.0f); pVertices[index+3].Pos = vEnd-vLateralAxe;
				pVertices[index+4].UV = Vector2(1.0f, 0.0f); pVertices[index+4].Pos = (*it)->vStart+vLateralAxe;
				pVertices[index+5].UV = Vector2(1.0f, 1.0f); pVertices[index+5].Pos = (*it)->vStart-vLateralAxe;

				pVertices[index+6].UV = Vector2(0.0f, 0.0f); pVertices[index+6].Pos = vEnd+vVerticalAxe;
				pVertices[index+7].UV = Vector2(1.0f, 0.0f); pVertices[index+7].Pos = (*it)->vStart+vVerticalAxe;
				pVertices[index+8].UV = Vector2(0.0f, 1.0f); pVertices[index+8].Pos = vEnd-vVerticalAxe;
				pVertices[index+9].UV = Vector2(0.0f, 1.0f); pVertices[index+9].Pos = vEnd-vVerticalAxe;
				pVertices[index+10].UV = Vector2(1.0f, 0.0f); pVertices[index+10].Pos = (*it)->vStart+vVerticalAxe;
				pVertices[index+11].UV = Vector2(1.0f, 1.0f); pVertices[index+11].Pos = (*it)->vStart-vVerticalAxe;

				pVertices[index+12].UV = Vector2(0.0f, 0.0f); pVertices[index+12].Pos = vEnd+vDiagonalAxe;
				pVertices[index+13].UV = Vector2(1.0f, 0.0f); pVertices[index+13].Pos = (*it)->vStart+vDiagonalAxe;
				pVertices[index+14].UV = Vector2(0.0f, 1.0f); pVertices[index+14].Pos = vEnd-vDiagonalAxe;
				pVertices[index+15].UV = Vector2(0.0f, 1.0f); pVertices[index+15].Pos = vEnd-vDiagonalAxe;
				pVertices[index+16].UV = Vector2(1.0f, 0.0f); pVertices[index+16].Pos = (*it)->vStart+vDiagonalAxe;
				pVertices[index+17].UV = Vector2(1.0f, 1.0f); pVertices[index+17].Pos = (*it)->vStart-vDiagonalAxe;

				pVertices[index+18].UV = Vector2(0.0f, 0.0f); pVertices[index+18].Pos = vEnd+vDiagonalAxe2;
				pVertices[index+19].UV = Vector2(1.0f, 0.0f); pVertices[index+19].Pos = (*it)->vStart+vDiagonalAxe2;
				pVertices[index+20].UV = Vector2(0.0f, 1.0f); pVertices[index+20].Pos = vEnd-vDiagonalAxe2;
				pVertices[index+21].UV = Vector2(0.0f, 1.0f); pVertices[index+21].Pos = vEnd-vDiagonalAxe2;
				pVertices[index+22].UV = Vector2(1.0f, 0.0f); pVertices[index+22].Pos = (*it)->vStart+vDiagonalAxe2;
				pVertices[index+23].UV = Vector2(1.0f, 1.0f); pVertices[index+23].Pos = (*it)->vStart-vDiagonalAxe2;

				index += 24;
				pVB->SetNumVertices(index);

				(*it)->fLife -= g_fDeltaTime;
				f32 fDelta = (*it)->fSpeed * g_fDeltaTime;
				if((*it)->fLife > 0.0f)
				{
					(*it)->fLength += fDelta;
					if((*it)->fLength > (*it)->fMaxLength)
					{
						(*it)->fLength = (*it)->fMaxLength;
						if(!(*it)->bStatic)
						{
							(*it)->vStart += (*it)->vDir * fDelta;
						}
					}
					++it;
				}
				else
				{
					(*it)->fLength -= fDelta;
					if((*it)->fLength > 0.0f)
					{
						(*it)->vStart += (*it)->vDir * fDelta;
						++it;
					}
					else
					{
						m_Pool->Free((*it));
						it = m_Lasers.erase(it);
					}
				}
			}

			pVB->SetDirty(true);
		}
	}
	
	//! Adds a laser
	Laser* LaserManager::AddLaser(const Laser::Params& params)
	{
		if(Laser* pLaser = m_Pool->Alloc<Laser>())
		{
			pLaser->Init(params);
			m_Lasers.push_back(pLaser);
			return pLaser;
		}
		return NULL;
	}

	//! clears all lasers
	void LaserManager::Clear()
	{
		for(std::list<Laser*>::iterator it = m_Lasers.begin(); it != m_Lasers.end(); ++it)
		{
			(*it)->fLife = 0.0f;
		}
	}
}

