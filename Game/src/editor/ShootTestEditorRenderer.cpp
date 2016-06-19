/* 

Amine Rehioui
Created: October 15th 2011

*/

#include "ShootEditorCommon.h"

#include "ShootTestEditorRenderer.h"

#include "ShootTest.h"
#include "MeshEntity.h"

#include "EditorSettings.h"

#include "BulletManager.h"
#include "AreaSpawner.h"
#include "TriggerSpawner.h"
#include "ConfigPoint.h"
#include "UIContext.h"

namespace shoot
{
	//! destructor
	ShootTestEditorRenderer::~ShootTestEditorRenderer()
	{
	}

	//! Init
	void ShootTestEditorRenderer::Init()
	{
		super::Init();

		// Init the unit circle vertex buffer
		m_UnitCircle = GraphicsDriver::Instance()->CreateVertexBuffer();
		m_UnitCircle->SetVertexFlag(Vertex3D::VF_Pos);
		m_UnitCircle->SetPrimitiveType(GraphicsDriver::PT_Line);
		{
			const u32 numVertices = 100;
			const f32 fAngleStep = Math::PI*2.0f/f32(numVertices);
			f32 fAngle = 0;
			Vertex3D* pVertices = snew Vertex3D[numVertices];			
			for(u32 i=0; i<numVertices; ++i)
			{
				pVertices[i].Pos = Vector3::Create(Math::Cos(fAngle), Math::Sin(fAngle), 0.0f);
				fAngle += fAngleStep;				
			}
			m_UnitCircle->SetVertices(pVertices, numVertices);
		}

		// debug icons
		m_DebugIconMap[AreaSpawner::TypeID] = ResourceManager::Instance()->GetResource<Texture>("../EditorData/DebugIcons/spawner.png");
		m_DebugIconMap[TriggerSpawner::TypeID] = ResourceManager::Instance()->GetResource<Texture>("../EditorData/DebugIcons/spawner.png");
		m_DebugIconMap[ConfigPoint::TypeID] = ResourceManager::Instance()->GetResource<Texture>("../EditorData/DebugIcons/ConfigPoint.png");
	}

	//! registers an entity for post rendering
	void ShootTestEditorRenderer::RegisterEntity(Entity* pEntity)
	{
		super::RegisterEntity(pEntity);

		if(pEntity->IsA(BulletManager::TypeID))
		{
			m_aBulletManagers.push_back(static_cast<BulletManager*>(pEntity));
		}
		else if(pEntity->IsA(AreaSpawner::TypeID))
		{
			m_aAreaSpawners.push_back(static_cast<AreaSpawner*>(pEntity));
		}
		else if(pEntity->IsA(TriggerSpawner::TypeID))
		{
			m_aTriggerSpawners.push_back(static_cast<TriggerSpawner*>(pEntity));
		}
	}

	//! custom Post 3D rendering
	void ShootTestEditorRenderer::RenderPost3D()
	{
		super::RenderPost3D();

		if(Player::Instance()
		&& Player::Instance()->GetMeshEntity())
		{
			Player* pPlayer = Player::Instance();

			// render player motion bounds
			Matrix44 matrix = Matrix44::Identity;
			matrix.Translate(pPlayer->GetPosition());

			GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_World, matrix);
			GraphicsDriver::Instance()->SetColor(Color::Yellow);
			GraphicsDriver::Instance()->DrawDebugBox(pPlayer->GetMotionBounds());
		}

		// render bullets collision shapes
		if(EditorSettings::Instance()->HasFlag(EditorSettings::EF_CollisionShapes))
		{
			GraphicsDriver::Instance()->SetColor(EditorSettings::Instance()->CollisionShapeColor);
			m_UnitSphere->GetSubMeshes()[0].m_VertexBuffer->Begin();
			for(u32 i=0; i<m_aBulletManagers.size(); ++i)
			{
				const std::list<Bullet*>& bullets = m_aBulletManagers[i]->GetBullets();
				for(std::list<Bullet*>::const_iterator it = bullets.begin(); it != bullets.end(); ++it)
				{
					Bullet* pBullet = (*it);
					Matrix44 transform = Matrix44::Identity;
					transform.Scale(Vector3::One*pBullet->fRadius);
					transform.Translate(pBullet->vPosition);
					GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_World, transform);					
					m_UnitSphere->GetSubMeshes()[0].m_VertexBuffer->Draw();
				}
			}
			m_UnitSphere->GetSubMeshes()[0].m_VertexBuffer->End();
		}

		// render spawner radiuses
		if(!m_aAreaSpawners.empty())
		{
			GraphicsDriver::Instance()->SetColor(Color::Blue);
			for(u32 i=0; i<m_aAreaSpawners.size(); ++i)
			{
				AreaSpawner* pSpawner = m_aAreaSpawners[i];
				Matrix44 transform = Matrix44::Identity;
				transform.Scale(Vector3::Create(pSpawner->GetRadius(), pSpawner->GetRadius(), 1.0f));
				transform.Translate(pSpawner->GetTransformationMatrix().GetTranslation());
				GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_World, transform);
				m_UnitCircle->Begin();
				m_UnitCircle->Draw();
				m_UnitCircle->End();
			}
		}

		if(!m_aTriggerSpawners.empty())
		{
			GraphicsDriver::Instance()->SetColor(Color::Blue);
			for(u32 i=0; i<m_aTriggerSpawners.size(); ++i)
			{
				TriggerSpawner* pSpawner = m_aTriggerSpawners[i];
				Matrix44 transform = Matrix44::Identity;
				f32 fPositionY = pSpawner->GetTransformationMatrix().GetTranslation().Y-pSpawner->GetDistToPlayer();
				transform.Translate(Vector3::Create(0.0f, fPositionY, 0.0f));
				GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_World, transform);
				const Vertex3D aLine[] =
				{
					{ Vector3::Create(-30.0f, 0.0f, 0.0f), Vector2(), Vector3::Zero, Color(), Vector3::Zero },
					{ Vector3::Create(30.0f, 0.0f, 0.0f), Vector2(), Vector3::Zero, Color(), Vector3::Zero },
				};
				GraphicsDriver::Instance()->DrawPrimitiveList(GraphicsDriver::PT_Line, Vertex3D::VF_Pos, 2, aLine);
			}
		}

		m_aBulletManagers.clear();
		m_aAreaSpawners.clear();
		m_aTriggerSpawners.clear();
	}

	//! custom Post 2D rendering
	void ShootTestEditorRenderer::RenderPost2D()
	{
		super::RenderPost2D();

		// render UIContext info
		Entity* pSelectedEntity = ShootEditor::Instance()->GetSelectedEntity();
		if(pSelectedEntity && pSelectedEntity->GetRoot()->IsA(UIContext::TypeID))
		{
			UIContext* pUIContext = static_cast<UIContext*>(pSelectedEntity->GetRootContext());
			Size resolution = EntityRenderer::Instance()->Get2DCamera()->GetTargetResolution();
			GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_World, Matrix44::Identity);

			// transition line
			f32 fLinePos = pUIContext->GetTransitionLinePos();
			GraphicsDriver::Instance()->SetColor(Color::Red);
			GraphicsDriver::Instance()->DrawLine(Vector3::Create(0.0f, fLinePos, 0.0f), Vector3::Create(resolution.Width, fLinePos, 0.0f));

			// touch area
			GraphicsDriver::Instance()->SetColor(Color::Green);
			GraphicsDriver::Instance()->DrawDebugBox(pUIContext->GetTouchArea());
		}
	}
}

