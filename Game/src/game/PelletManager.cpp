/* 

Amine Rehioui
Created: May 1st 2013

*/

#include "ShootTest.h"

#include "PelletManager.h"

#include "MaterialProvider.h"

#include "AIManager.h"

#include "GraphicExtensionHandler.h"

#include "PlayerFSM.h"

namespace shoot
{
	DEFINE_OBJECT(PelletManager);

	//! Constructor
	PelletManager::PelletManager()
		: m_fMinCos(0.0f)
		// properties
		, m_fBulletSize(8.0f)
		, m_fMinDistToEnemy(10.0f)
		, m_fHomingFactor(3.0f)
		, m_fMaxHomingAngle(45.0f)
		, m_bOwnerIsPlayer(true)
	{
	}

	//! destructor
	PelletManager::~PelletManager()
	{
	}

	//! serializes the entity to/from a PropertyStream
	void PelletManager::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);

		bool bBulletSizeChanged = stream.Serialize(PT_Float, "BulletSize", &m_fBulletSize);
		stream.Serialize(PT_Float, "MinDistToEnemy", &m_fMinDistToEnemy);
		stream.Serialize(PT_Float, "HomingFactor", &m_fHomingFactor);
		if(stream.Serialize(PT_Float, "MaxHomingAngle", &m_fMaxHomingAngle))
		{
			m_fMinCos = Math::Cos(m_fMaxHomingAngle*Math::DegToRadFactor);
		}

		stream.Serialize(PT_Bool, "OwnerIsPlayer", &m_bOwnerIsPlayer);

		if(bBulletSizeChanged && IsInitialized())
		{
			SetBulletSize(m_fBulletSize);
		}		
	}

	//! called during the initialization of the entity
	void PelletManager::Init()
	{
		super::Init();
		
		GraphicComponent* pGraphic = GetComponent<GraphicComponent>();

		if(!pGraphic->GetMaterial())
		{
			pGraphic->SetMaterial(snew Material());
		}

		u32 maxBullets = m_BulletPoolSize/sizeof(Bullet);
		VertexBuffer* pVB = GraphicsDriver::Instance()->CreateVertexBuffer();
		
		pVB->SetVertexFlag(Vertex3D::VF_Pos);		
		pVB->SetName(GetClassName());
		if(GraphicExtensionHandler::Instance()->HasExtension(GraphicExtensionHandler::E_PointSprite))
		{
			pVB->SetPrimitiveType(GraphicsDriver::PT_Point);
			pVB->SetRenderStateSetter(snew PointSpriteStateSetter(m_fBulletSize));
			pVB->SetVertices(snew Vertex3D[maxBullets], maxBullets);
		}
		else
		{
			pVB->SetPrimitiveType(GraphicsDriver::PT_Triangle);
			pVB->SetVertexFlag(Vertex3D::VF_UV);
			pVB->SetVertices(snew Vertex3D[maxBullets*6], maxBullets*6);
		}
		pGraphic->SetVertexBuffer(pVB);

		pVB->SetDynamic(true);
	}	

	//! called during the update of the entity		
	void PelletManager::Update()
	{
		VertexBuffer* pVB = GetComponent<GraphicComponent>()->GetVertexBuffer();
		pVB->SetNumVertices(0);

		if(m_Bullets.size())
		{
			u32 bulletIndex = 0;
			for(std::list<Bullet*>::iterator it = m_Bullets.begin(); it != m_Bullets.end();)
			{
				Vector3 vClosestEnemy = Vector3::Zero;
				f32 fDistToClosestEnemy = Math::Epsilon;
				if(GetClosestEnemy((*it)->vPosition, vClosestEnemy, fDistToClosestEnemy))
				{
					f32 fDistFactor = m_fMinDistToEnemy/fDistToClosestEnemy;
					f32 fInterpolator = m_fHomingFactor*fDistFactor*fDistFactor*g_fDeltaTime;
					fInterpolator = Math::Clamp(fInterpolator, 0.0f, 1.0f);
					Vector3 vDirectionToEnemy = (vClosestEnemy-(*it)->vPosition).Normalize();
					(*it)->vDirection = (vDirectionToEnemy-(*it)->vDirection)*fInterpolator + (*it)->vDirection;
				}
				
				(*it)->vPosition += (*it)->vDirection*(*it)->fSpeed*g_fDeltaTime;
				SetupRendering((*it), bulletIndex++, pVB);

				(*it)->fLife -= g_fDeltaTime;
				if((*it)->fLife > 0.0f)
				{
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

	//! Prepares a bullet for rendering
	void PelletManager::SetupRendering(Bullet* pBullet, u32 index, VertexBuffer* pVB)
	{
		Vertex3D* pVertices = pVB->GetVertices();
		if(GraphicExtensionHandler::Instance()->HasExtension(GraphicExtensionHandler::E_PointSprite))
		{
			pVertices[index].Pos = pBullet->vPosition;
			pVB->SetNumVertices(index+1);
		}
		else
		{
			const Matrix44& viewInverse = EntityRenderer::Instance()->Get3DCamera()->GetViewMatrixInverse();
			const f32 fHalfSize = pBullet->fRadius;
			const u32 vtx = index*6;
			pVertices[vtx+0].UV = Vector2(0.0f, 1.0f); pVertices[vtx+0].Pos = Vector3::Create(-fHalfSize, -fHalfSize, 0.0f);
			pVertices[vtx+1].UV = Vector2(0.0f, 0.0f); pVertices[vtx+1].Pos = Vector3::Create(-fHalfSize, fHalfSize, 0.0f);
			pVertices[vtx+2].UV = Vector2(1.0f, 0.0f); pVertices[vtx+2].Pos = Vector3::Create(fHalfSize, fHalfSize, 0.0f);
			pVertices[vtx+3].UV = Vector2(0.0f, 1.0f); pVertices[vtx+3].Pos = Vector3::Create(-fHalfSize, -fHalfSize, 0.0f);
			pVertices[vtx+4].UV = Vector2(1.0f, 0.0f); pVertices[vtx+4].Pos = Vector3::Create(fHalfSize, fHalfSize, 0.0f);
			pVertices[vtx+5].UV = Vector2(1.0f, 1.0f); pVertices[vtx+5].Pos = Vector3::Create(fHalfSize, -fHalfSize, 0.0f);
			Matrix44 transform = viewInverse;
			transform.SetTranslation(pBullet->vPosition);
			for(u32 i=0; i<6; ++i)
			{
				pVertices[vtx+i].Pos = transform.TransformVect(pVertices[vtx+i].Pos);				
			}
			pVB->SetNumVertices(vtx+6);
		}		
	}

	//! sets the bullet size
	void PelletManager::SetBulletSize(f32 fSize)
	{
		VertexBuffer* pVB = GetComponent<GraphicComponent>()->GetVertexBuffer();
		if(GraphicExtensionHandler::Instance()->HasExtension(GraphicExtensionHandler::E_PointSprite))
		{
			static_cast<PointSpriteStateSetter*>(pVB->GetRenderStateSetter())->SetPointSize(fSize);
		}
		m_fBulletSize = fSize;
	}

		//! returns info about the closest enemy with respect to a pellet
	bool PelletManager::GetClosestEnemy(const Vector3& vPosition, Vector3& vEnemyPos, f32& fDistToEnemy) 
	{
		if(m_bOwnerIsPlayer)
		{
			std::list< Handle<Actor> >& lEnemies = AIManager::Instance()->GetEnemies();
			if(lEnemies.size())
			{
				Actor* pClosestEnemy = NULL;
				f32 fDistToClosestEnemySquared = 0.0f;
				for(std::list< Handle<Actor> >::iterator it = lEnemies.begin(); it != lEnemies.end(); )
				{
					if((*it).IsValid())
					{
						Vector3 vEnemyPos = (*it)->GetMeshEntity()->GetTransformationMatrix().GetTranslation();
						f32 fDistSquared = (vEnemyPos-vPosition).GetLengthSquared();
						Vector3 vToEnemy = (vEnemyPos-vPosition).Normalize();	
						f32 fDotP = Vector3::Create(0.0f, 1.0f, 0.0f).DotProduct(vToEnemy);
						if((pClosestEnemy == NULL || fDistSquared < fDistToClosestEnemySquared)
							&& (fDotP > m_fMinCos)
							&& ((*it)->GetHitPoints() >= 0.0f))
						{
							pClosestEnemy = (*it).Get();
							fDistToClosestEnemySquared = fDistSquared;
						}
						++it;
					}
					else
					{
						it = lEnemies.erase(it);
					}
				}
				if(pClosestEnemy)
				{
					vEnemyPos = pClosestEnemy->GetMeshEntity()->GetTransformationMatrix().GetTranslation();
					fDistToEnemy = Math::SquareRoot(fDistToClosestEnemySquared);
					return fDistToEnemy > Math::Epsilon;
				}
			}
			return false;
		}
		else
		{
			bool bReady = (Player::Instance()->GetFSM()->GetState() != PlayerFSM::State_Exploding);
			if(bReady)
			{
				vEnemyPos = Player::Instance()->GetMeshEntity()->GetTransformationMatrix().GetTranslation();
				Vector3 vToEnemy = (vEnemyPos-vPosition).Normalize();
				f32 fDotP = Vector3::Create(0.0f, -1.0f, 0.0f).DotProduct(vToEnemy);
				if(fDotP > m_fMinCos)
				{
					fDistToEnemy = (vEnemyPos-vPosition).GetLength();
					return true;
				}
			}
			return false;
		}
	}
}

