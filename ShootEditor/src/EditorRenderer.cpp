/* 

Amine Rehioui
Created: August 20th 2011

*/

#include "ShootEditorCommon.h"

#include "EditorRenderer.h"

#include "Path.h"

#include "EditorSettings.h"

#include "Entity3DController.h"

#include "Plane.h"

#include "CollisionComponent.h"

#include "MeshEntity.h"

#include "ParticleGenerator.h"

#include "OpenGL/OpenGLExtensionHandler.h"

#include "EnvironmentCollisionComponent.h"
#include "RigidBodyComponent.h"
#include "SkyBoxEntity.h"

namespace shoot
{
	//! constructor
	EditorRenderer::EditorRenderer()
		: m_bEnabled(true)
		, m_bPickingInfoValid(false)
	{
	}

	//! destructor
	EditorRenderer::~EditorRenderer()
	{
	}

	//! Init
	void EditorRenderer::Init()
	{
		m_UnitSphere.SetObject(ResourceManager::Instance()->GetResource<MeshResource>("../rawdata/UnitSphere.xml"));

		m_GridVertexBuffer = GraphicsDriver::Instance()->CreateVertexBuffer();
		m_GridVertexBuffer->SetVertexFlag(Vertex3D::VF_Pos);
		m_GridVertexBuffer->SetPrimitiveType(GraphicsDriver::PT_Line);
		m_GridVertexBuffer->SetDynamic(true);

		m_PinTexture = ResourceManager::Instance()->GetResource<Texture>("../EditorData/DebugIcons/Pin.png");

		// debug icons
		m_DebugIconEntity = snew BillBoardEntity();
		GraphicComponent* pGraphic = snew GraphicComponent();
		pGraphic->SetMaterial(snew Material());
		pGraphic->GetMaterial()->SetFlag(Material::MF_AlphaBlending, true);
		pGraphic->GetMaterial()->SetFlag(Material::MF_DepthTest, false);
		m_DebugIconEntity->AddComponent(pGraphic, true);
		m_DebugIconEntity->Init();
	}

	//! performs editor rendering
	void EditorRenderer::Render()
	{
		if(Engine::Instance()->IsLoading())
		{
			return;
		}

#ifndef DX11
		OpenGLExtensionHandler::Instance()->extGlBindVertexArray(0);
		OpenGLExtensionHandler::Instance()->extGlBindBuffer(GL_ARRAY_BUFFER, 0);
		OpenGLExtensionHandler::Instance()->extGlBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#endif

		GraphicsDriver::Instance()->SetRenderState(GraphicsDriver::RS_FillMode, GraphicsDriver::FM_Line);
		GraphicsDriver::Instance()->SetRenderState(GraphicsDriver::RS_2DTextureMapping, false);
		GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_Texture, Matrix44::Identity);

		if(m_bEnabled)
		{
			Entity* pSelectedEntity = ShootEditor::Instance()->GetSelectedEntity();
			Entity* pRoot = pSelectedEntity ? pSelectedEntity->GetRoot() : Engine::Instance()->GetContextStack();
			ExtractEntities(pRoot);

			RenderPost3D();
			RenderPost2D();

			m_2DEntities.clear();
			m_aPaths.clear();
			m_3DEntities.clear();
		}

		// render entity controller
		if(Camera* pCamera = EntityRenderer::Instance()->Get3DCamera())
		{
			GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_Projection, pCamera->GetProjectionMatrix());
			GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_View, pCamera->GetViewMatrix());
			GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_World, Matrix44::Identity);
			GraphicsDriver::Instance()->SetRenderState(GraphicsDriver::RS_DepthTesting, false);
			Entity3DController::Instance()->Render();
		}

		GraphicsDriver::Instance()->SetRenderState(GraphicsDriver::RS_2DTextureMapping, true);
		GraphicsDriver::Instance()->SetRenderState(GraphicsDriver::RS_FillMode, GraphicsDriver::FM_Solid);
	}

	//! extracts entities for rendering
	void EditorRenderer::ExtractEntities(Entity* pEntity)
	{
		Context* pContext = DYNAMIC_CAST(pEntity, Context);
		if(pContext && pContext->IsLoading())
		{
			return;
		}

		for(u32 i=0; i<pEntity->GetChildCount(); ++i)
		{
			RegisterEntity(pEntity->GetChild(i));
			ExtractEntities(pEntity->GetChild(i));
		}
	}

	//! registers an entity for post rendering
	void EditorRenderer::RegisterEntity(Entity* pEntity)
	{
		if(pEntity->IsA(Entity2D::TypeID))
		{
			m_2DEntities.push_back(pEntity);
		}
		else if(pEntity->IsA(Path::TypeID))
		{
			Path* pPath = static_cast<Path*>(pEntity);
			if(pPath->GetChildCount() > 0)
			{
				m_aPaths.push_back(pPath);
			}
		}
		else if(pEntity->IsA(Entity3D::TypeID))
		{
			m_3DEntities.push_back(pEntity);
		}
	}

	//! custom Post 2D rendering
	void EditorRenderer::RenderPost2D()
	{
		Camera* pCamera = EntityRenderer::Instance()->Get2DCamera();
		GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_Projection, pCamera->GetProjectionMatrix());
		GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_View, pCamera->GetViewMatrix());
		GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_World, Matrix44::Identity);

		// render 2D origin
		if(EditorSettings::Instance()->HasFlag(EditorSettings::EF_Render2DOrigin))
		{
			GraphicsDriver::Instance()->SetColor(Color::Red);
			GraphicsDriver::Instance()->DrawLine(Vector3::Zero, Vector3::Create(EditorSettings::Instance()->f2DOriginSize, 0.0f, 0.0f));			

			GraphicsDriver::Instance()->SetColor(Color::Green);
			GraphicsDriver::Instance()->DrawLine(Vector3::Zero, Vector3::Create(0.0f, EditorSettings::Instance()->f2DOriginSize, 0.0f));			
		}

		// render entity specific elements
		for(u32 i=0; i<m_2DEntities.size(); ++i)
		{
			Entity2D* pEntity2D = static_cast<Entity2D*>(m_2DEntities[i]);

			// render 2d bounding boxes
			if(EditorSettings::Instance()->HasFlag(EditorSettings::EF_RenderBBox))
			{
				GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_World, pEntity2D->GetTransformationMatrix());
				GraphicsDriver::Instance()->SetColor(EditorSettings::Instance()->BBoxColor);
				GraphicsDriver::Instance()->DrawDebugBox(pEntity2D->GetBoundingBox());
			}

			// render Entity2D positions			
			if(EditorSettings::Instance()->HasFlag(EditorSettings::EF_RenderEntity2DPos))			
			{
				GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_World, pEntity2D->GetCenterTransformationMatrix());

				pEntity2D == ShootEditor::Instance()->GetSelectedEntity() ?
					GraphicsDriver::Instance()->SetColor(EditorSettings::Instance()->SelectedEntityColor) :
					GraphicsDriver::Instance()->SetColor(EditorSettings::Instance()->EntityPosColor);					

				f32 fSize = EditorSettings::Instance()->fEntity2DPosSize;
				GraphicsDriver::Instance()->DrawLine(Vector3::Create(-fSize/2.0f/pEntity2D->GetScale().X, 0.0f, 0.0f), Vector3::Create(fSize/2.0f/pEntity2D->GetScale().X, 0.0f, 0.0f));
				GraphicsDriver::Instance()->DrawLine(Vector3::Create(0.0f, -fSize/2.0f/pEntity2D->GetScale().Y, 0.0f), Vector3::Create(0.0f, fSize/2.0f/pEntity2D->GetScale().Y, 0.0f));
			}
		}

		// render selected entity at the end
		if(Entity2D* pEntity2D = DYNAMIC_CAST(ShootEditor::Instance()->GetSelectedEntity(), Entity2D))		
		{
			GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_World, pEntity2D->GetTransformationMatrix());
			GraphicsDriver::Instance()->SetColor(EditorSettings::Instance()->SelectedEntityColor);
			GraphicsDriver::Instance()->DrawDebugBox(pEntity2D->GetBoundingBox());
		}
	}

	//! custom Post 3D rendering
	void EditorRenderer::RenderPost3D()
	{
		Camera* pCamera = EntityRenderer::Instance()->Get3DCamera();
		GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_Projection, pCamera->GetProjectionMatrix());
		GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_View, pCamera->GetViewMatrix());
		GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_World, Matrix44::Identity);

		// render grid
		if(EditorSettings::Instance()->HasFlag(EditorSettings::EF_Render3DGrid))		
		{
			GraphicsDriver::Instance()->SetRenderState(GraphicsDriver::RS_DepthTesting, true);
			const u32 gridSize = EditorSettings::Instance()->GridSize;
			const u32 numGridVertices = (gridSize+1)*4;
			if(m_GridVertexBuffer->GetNumVertices() != numGridVertices)
			{
				Vertex3D* pVertices = snew Vertex3D[numGridVertices];
				Vector3 lineStartHoriz = Vector3::Create(-s32(gridSize)/2, -s32(gridSize)/2, 0.0f);
				Vector3 lineStartVert = lineStartHoriz;
				u32 currentVertex = 0;
				for(u32 i=0; i<=gridSize; ++i)
				{
					pVertices[currentVertex++].Pos = lineStartHoriz;
					pVertices[currentVertex++].Pos = lineStartHoriz+Vector3::Create(f32(gridSize), 0.0f, 0.0f);
					pVertices[currentVertex++].Pos = lineStartVert;
					pVertices[currentVertex++].Pos = lineStartVert+Vector3::Create(0.0f, f32(gridSize), 0.0f);					
					lineStartHoriz.Y++;
					lineStartVert.X++;
				}
				SHOOT_ASSERT(currentVertex == numGridVertices, "Grid rendering error");
				m_GridVertexBuffer->SetVertices(pVertices, numGridVertices);
			}

			GraphicsDriver::Instance()->SetColor(Color(0.2f, 0.2f, 0.2f));
			m_GridVertexBuffer->Begin();
			m_GridVertexBuffer->Draw();
			m_GridVertexBuffer->End();
			GraphicsDriver::Instance()->SetRenderState(GraphicsDriver::RS_DepthTesting, false);
		}

		// render origin
		if(EditorSettings::Instance()->HasFlag(EditorSettings::EF_Render3DOrigin))			
		{
			GraphicsDriver::Instance()->SetColor(Color::Red);
			GraphicsDriver::Instance()->DrawLine(Vector3::Zero, Vector3::Create(EditorSettings::Instance()->f3DOriginSize, 0.0f, 0.0f));

			GraphicsDriver::Instance()->SetColor(Color::Green);
			GraphicsDriver::Instance()->DrawLine(Vector3::Zero, Vector3::Create(0.0f, EditorSettings::Instance()->f3DOriginSize, 0.0f));		

			GraphicsDriver::Instance()->SetColor(Color::Blue);
			GraphicsDriver::Instance()->DrawLine(Vector3::Zero, Vector3::Create(0.0f, 0.0f, EditorSettings::Instance()->f3DOriginSize));			
		}

		// render quad trees
		if(EditorSettings::Instance()->HasFlag(EditorSettings::EF_RenderQuadTrees))
		{
			for(u32 i=0; i<Engine::Instance()->GetNumContexts(); ++i)
			{
				if(QuadTree* pQuadTree = Engine::Instance()->GetContext(i)->GetQuadTree())
				{
					if(QuadTreeNode* pRoot = pQuadTree->GetRoot())
					{
						RenderQuadTreeNodes(pRoot);
						RenderVisibleQuadTreeNodes(pRoot);						
					}
				}
			}
		}

		// render entity specific elements
		for(u32 i=0; i<m_3DEntities.size(); ++i)
		{
			Entity3D* pEntity3D = static_cast<Entity3D*>(m_3DEntities[i]);

			// render bounding boxes
			if(EditorSettings::Instance()->HasFlag(EditorSettings::EF_RenderBBox)
			&& !pEntity3D->IsA(MeshEntity::TypeID)
			&& pEntity3D != EntityRenderer::Instance()->Get3DCamera())
			{
				AABBox3D box(-Vector3::One/2.0f, Vector3::One/2.0f);
				if(ParticleGenerator *pParticleGenerator = DYNAMIC_CAST(pEntity3D, ParticleGenerator))
				{
					box = pParticleGenerator->GetEmitBox() + pEntity3D->Entity3D::GetTransformationMatrix().GetTranslation();
				}

				GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_World, pEntity3D->GetTransformationMatrix());

				pEntity3D == ShootEditor::Instance()->GetSelectedEntity() ?
					GraphicsDriver::Instance()->SetColor(EditorSettings::Instance()->SelectedEntityColor) :
					GraphicsDriver::Instance()->SetColor(EditorSettings::Instance()->BBoxColor);

				GraphicsDriver::Instance()->DrawDebugBox(box);
			}

			// render Entity3D positions			
			if(EditorSettings::Instance()->HasFlag(EditorSettings::EF_RenderEntity3DPos)
				&& pEntity3D != EntityRenderer::Instance()->Get3DCamera())
			{
				GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_World, pEntity3D->GetCenterTransformationMatrix());

				pEntity3D == ShootEditor::Instance()->GetSelectedEntity() ?
					GraphicsDriver::Instance()->SetColor(EditorSettings::Instance()->SelectedEntityColor) :
					GraphicsDriver::Instance()->SetColor(EditorSettings::Instance()->EntityPosColor) ;					

				GraphicsDriver::Instance()->DrawLine(Vector3::Create(-EditorSettings::Instance()->fEntity3DPosSize/2.0f, 0.0f, 0.0f), Vector3::Create(EditorSettings::Instance()->fEntity3DPosSize/2.0f, 0.0f, 0.0f));
				GraphicsDriver::Instance()->DrawLine(Vector3::Create(0.0f, -EditorSettings::Instance()->fEntity3DPosSize/2.0f, 0.0f), Vector3::Create(0.0f, EditorSettings::Instance()->fEntity3DPosSize/2.0f, 0.0f));
				GraphicsDriver::Instance()->DrawLine(Vector3::Create(0.0f, 0.0f, -EditorSettings::Instance()->fEntity3DPosSize/2.0f), Vector3::Create(0.0f, 0.0f, EditorSettings::Instance()->fEntity3DPosSize/2.0f));
			}

			// render collision shapes if any
			if(EditorSettings::Instance()->HasFlag(EditorSettings::EF_CollisionShapes))
			{
				std::vector<CollisionComponent*> colComponents;
				pEntity3D->GetComponents<CollisionComponent>(colComponents);
				for(u32 i=0; i<colComponents.size(); ++i)
				{
					if(colComponents[i]->IsEnabled())
					{
						GraphicsDriver::Instance()->SetColor(EditorSettings::Instance()->CollisionShapeColor);
						RenderCollisionShapes(colComponents[i]);
					}
				}
			}

			// render debug icons
			if(EditorSettings::Instance()->HasFlag(EditorSettings::EF_DebugIcons))
			{
				GraphicsDriver::Instance()->SetColor(Color::White);
				bool bDebugIconSet = false;
				for(DebugIconMap::iterator it = m_DebugIconMap.begin(); it != m_DebugIconMap.end(); ++it)
				{
					if(pEntity3D->IsA(it->first))
					{
						if(!bDebugIconSet)
						{
							m_DebugIconEntity->SetScale(Vector3::One*EditorSettings::Instance()->fDebugIconSize);
							m_DebugIconEntity->Update();
							bDebugIconSet = true;
						}
						Texture* pTexture = it->second.Get();
						GraphicComponent* pGraphic = m_DebugIconEntity->GetComponent<GraphicComponent>();
						pGraphic->GetMaterial()->SetTexture(0, pTexture);
						pGraphic->GetMaterial()->Begin();
						m_DebugIconEntity->SetPosition(pEntity3D->GetTransformationMatrix().GetTranslation());
						GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_World, m_DebugIconEntity->GetTransformationMatrix());
						GraphicsDriver::Instance()->SetRenderState(GraphicsDriver::RS_FillMode, GraphicsDriver::FM_Solid);
						pGraphic->GetVertexBuffer()->Begin();
						pGraphic->GetVertexBuffer()->Draw();
						pGraphic->GetVertexBuffer()->End();
						pGraphic->GetMaterial()->End();
						GraphicsDriver::Instance()->SetRenderState(GraphicsDriver::RS_FillMode, GraphicsDriver::FM_Line);
						GraphicsDriver::Instance()->SetRenderState(GraphicsDriver::RS_2DTextureMapping, false);
						break;
					}
				}
			}

			// render frustums
			if(EditorSettings::Instance()->HasFlag(EditorSettings::EF_RenderFrustums))
			{
				if(Camera* pCamera = DYNAMIC_CAST(pEntity3D, Camera))
				{
					if(pCamera->GetType() != Camera::Type_Ortho2D)
					{
						RenderCameraFrustum(pCamera);						
					}
				}
			}
		}

		// render pin
		if(ShootEditor::Instance()->HasPin())
		{
			GraphicComponent* pGraphic = m_DebugIconEntity->GetComponent<GraphicComponent>();
			pGraphic->GetMaterial()->SetTexture(0, m_PinTexture);
			pGraphic->GetMaterial()->Begin();
			m_DebugIconEntity->SetPosition(ShootEditor::Instance()->GetPinPosition());
			f32 fProportionalSize = (ShootEditor::Instance()->GetPinPosition() - EntityRenderer::Instance()->Get3DCamera()->GetPosition()).GetLength()*EditorSettings::Instance()->fPinSizeFactor;			
			m_DebugIconEntity->SetScale(Vector3::One*fProportionalSize);
			m_DebugIconEntity->Update();
			GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_World, m_DebugIconEntity->GetTransformationMatrix());
			GraphicsDriver::Instance()->SetRenderState(GraphicsDriver::RS_FillMode, GraphicsDriver::FM_Solid);
			GraphicsDriver::Instance()->SetColor(Color::White);
			pGraphic->GetVertexBuffer()->Begin();
			pGraphic->GetVertexBuffer()->Draw();
			pGraphic->GetVertexBuffer()->End();
			pGraphic->GetMaterial()->End();
			GraphicsDriver::Instance()->SetRenderState(GraphicsDriver::RS_FillMode, GraphicsDriver::FM_Line);
			GraphicsDriver::Instance()->SetRenderState(GraphicsDriver::RS_2DTextureMapping, false);
		}

		// render paths
		for(u32 i=0; i<m_aPaths.size(); ++i)
		{
			RenderPost3DPath(m_aPaths[i]);
		}

		// render selected entity at the end
		if(Entity3D* pEntity3D = DYNAMIC_CAST(ShootEditor::Instance()->GetSelectedEntity(), Entity3D))
		{
			GraphicComponent* pGraphic = pEntity3D->GetComponent<GraphicComponent>();
			if(VertexBuffer* pVertexBuffer = pGraphic ? pGraphic->GetVertexBuffer() : NULL)
			{
				ParticleGenerator* pParticles = DYNAMIC_CAST(pEntity3D, ParticleGenerator);
				if(pParticles && !pParticles->GetApplyWorldTransform())
				{
					GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_World, Matrix44::Identity);
				}
				else
				{
					GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_World, pEntity3D->GetTransformationMatrix());
				}
				GraphicsDriver::Instance()->SetColor(EditorSettings::Instance()->SelectedEntityColor);
				pVertexBuffer->Begin();
				pVertexBuffer->Draw();
				pVertexBuffer->End();
			}

			if(pEntity3D->IsA(SkyBoxEntity::TypeID))
			{
				// reset 3D view
				GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_Projection, pCamera->GetProjectionMatrix());
				GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_View, pCamera->GetViewMatrix());
				GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_World, Matrix44::Identity);
			}
		}

		if(m_bPickingInfoValid && EditorSettings::Instance()->HasFlag(EditorSettings::EF_RenderPickingInfo))
		{
			GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_World, Matrix44::Identity);
			GraphicsDriver::Instance()->SetColor(Color::Green);
			GraphicsDriver::Instance()->DrawLine(m_vPickStart, m_vPickEnd);
			Matrix44 matrixIntersect = Matrix44::Identity;
			matrixIntersect.Translate(m_vPickIntersection);
			GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_World, matrixIntersect);
			const f32 fIntersectionSize = 2.0f;
			GraphicsDriver::Instance()->DrawLine(Vector3::Create(-fIntersectionSize/2.0f, 0.0f, 0.0f), Vector3::Create(fIntersectionSize/2.0f, 0.0f, 0.0f));
			GraphicsDriver::Instance()->DrawLine(Vector3::Create(0.0f, -fIntersectionSize/2.0f, 0.0f), Vector3::Create(0.0f, fIntersectionSize/2.0f, 0.0f));
			GraphicsDriver::Instance()->DrawLine(Vector3::Create(0.0f, 0.0f, -fIntersectionSize/2.0f), Vector3::Create(0.0f, 0.0f, fIntersectionSize/2.0f));
		}
	}

	//! Renders collision shapes
	void EditorRenderer::RenderCollisionShapes(CollisionComponent* pCollisionComponent)
	{
		Entity3D* pParent = DYNAMIC_CAST(pCollisionComponent->GetParent(), Entity3D);
		std::vector<Matrix44> aSphereTransforms;

		if(EnvironmentCollisionComponent* pEnvCol = DYNAMIC_CAST(pCollisionComponent, EnvironmentCollisionComponent))
		{
			if(MeshResource* pMesh = pEnvCol->GetMesh())
			{
				Vertex3D* pVertices = pMesh->GetSubMeshes()[0].m_VertexBuffer->GetVertices();
				u16* pIndices = pMesh->GetSubMeshes()[0].m_VertexBuffer->GetIndices();
				u32 numIndices = pMesh->GetSubMeshes()[0].m_VertexBuffer->GetNumIndices();
				const Matrix44& matrix = static_cast<Entity3D*>(pEnvCol->GetParent())->GetTransformationMatrix();

				GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_World, Matrix44::Identity);
				for(u32 i=0; i<numIndices; i += 3)
				{
					Vector3 v1 = matrix.TransformVect(pVertices[pIndices[i + 0]].Pos);
					Vector3 v2 = matrix.TransformVect(pVertices[pIndices[i + 1]].Pos);
					Vector3 v3 = matrix.TransformVect(pVertices[pIndices[i + 2]].Pos);

					GraphicsDriver::Instance()->SetColor(EditorSettings::Instance()->EntityPosColor);
					GraphicsDriver::Instance()->DrawLine(v1, v2);
					GraphicsDriver::Instance()->DrawLine(v2, v3);
					GraphicsDriver::Instance()->DrawLine(v3, v1);

					// draw the normal
					const f32 fNormalSize = 1.0f;
					Plane plane(v1, v2, v3);
					Vector3 vCenter = (v1 + v2 + v3)/3.0f;
					GraphicsDriver::Instance()->DrawLine(vCenter, vCenter+plane.Normal*fNormalSize);
				}

				// draw collision points
				for(std::list<EnvironmentCollisionComponent::CollisionInfo>::iterator it = pEnvCol->m_aCollisionInfos.begin(); it != pEnvCol->m_aCollisionInfos.end();)
				{
					GraphicsDriver::Instance()->SetColor(Color::Red);
					Matrix44 collisionPoint = Matrix44::Identity;
					collisionPoint.SetTranslation((*it).vPosition);
					GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_World, collisionPoint);
					GraphicsDriver::Instance()->DrawLine(Vector3::Create(-EditorSettings::Instance()->fEntity3DPosSize/2.0f, 0.0f, 0.0f), Vector3::Create(EditorSettings::Instance()->fEntity3DPosSize/2.0f, 0.0f, 0.0f));
					GraphicsDriver::Instance()->DrawLine(Vector3::Create(0.0f, -EditorSettings::Instance()->fEntity3DPosSize/2.0f, 0.0f), Vector3::Create(0.0f, EditorSettings::Instance()->fEntity3DPosSize/2.0f, 0.0f));
					GraphicsDriver::Instance()->DrawLine(Vector3::Create(0.0f, 0.0f, -EditorSettings::Instance()->fEntity3DPosSize/2.0f), Vector3::Create(0.0f, 0.0f, EditorSettings::Instance()->fEntity3DPosSize/2.0f));
					(*it).fLife -= g_fDeltaTime;

					if((*it).fLife > 0.0f)
					{
						++it;
					}
					else
					{
						it = pEnvCol->m_aCollisionInfos.erase(it);
					}
				}
			}
		}
		else if(RigidBodyComponent* pRigidBody = DYNAMIC_CAST(pCollisionComponent, RigidBodyComponent))
		{
			for(u32 i=0; i<pRigidBody->GetShapeCount(); ++i)
			{
				CollisionShape* pShape = pRigidBody->GetShape(i);			

				if(CollisionShapeBox* pBox = DYNAMIC_CAST(pShape, CollisionShapeBox))
				{
					Matrix44 transform = Matrix44::Identity;
					transform.Scale(pParent->GetAbsoluteScale());
					transform.Translate(pParent->GetTransformationMatrix().GetTranslation());
					GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_World, transform);
					GraphicsDriver::Instance()->DrawDebugBox(pBox->GetBox());
				}
				else if(CollisionShapeSphere* pSphere = DYNAMIC_CAST(pShape, CollisionShapeSphere))
				{
					Matrix44 transform = Matrix44::Identity;
					f32 fScale = (pParent->GetAbsoluteScale()*pSphere->GetRadius()).X;
					transform.Scale(Vector3::Create(fScale, fScale, fScale));
					transform.Translate(pSphere->GetCenter(pParent->GetTransformationMatrix()));
					aSphereTransforms.push_back(transform);
				}
			}
		}

		if(!aSphereTransforms.empty())
		{
			m_UnitSphere->GetSubMeshes()[0].m_VertexBuffer->Begin();
			for(u32 i=0; i<aSphereTransforms.size(); ++i)
			{
				GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_World, aSphereTransforms[i]);
				m_UnitSphere->GetSubMeshes()[0].m_VertexBuffer->Draw();
			}
			m_UnitSphere->GetSubMeshes()[0].m_VertexBuffer->End();
		}
	}

	//! Render path
	void EditorRenderer::RenderPost3DPath(Path* pPath)
	{
		GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_World, Matrix44::Identity);

		std::vector<Vector3> aPoints;
		f32 fStep = 1.0f/EditorSettings::Instance()->fPathSmoothness;
		f32 k = 0.0f;
		while(k < 1.0f)
		{
			aPoints.push_back(pPath->GetPosition(k));
			k += fStep;
		}
		aPoints.push_back(pPath->GetPosition(1.0f));		

		GraphicsDriver::Instance()->SetColor(EditorSettings::Instance()->PathColor);
		for(s32 i=0; i<s32(aPoints.size())-1; ++i)
		{
			GraphicsDriver::Instance()->DrawLine(aPoints[i], aPoints[i+1]);
		}
	}

	//! Render quad tree nodes
	void EditorRenderer::RenderQuadTreeNodes(QuadTreeNode* pNode)
	{
		RenderQuadTreeNode(pNode, Color(.5f, .5f, .5f));
		
		for(std::list< Handle<Entity3D> >::const_iterator it = pNode->GetEntityList().begin(); 
			it != pNode->GetEntityList().end();
			++it)
		{
			if((*it).IsValid())
			{
				GraphicsDriver::Instance()->SetColor(Color::Red);
				GraphicsDriver::Instance()->DrawDebugBox((*it)->GetBoundingBox());
			}
		}

		for(u32 i=0; i<QuadTreeNode::T_Count; ++i)
		{
			if(QuadTreeNode* pChild = pNode->GetChild((QuadTreeNode::E_NodeType)i))
			{
				RenderQuadTreeNodes(pChild);
			}
		}
	}

	//! Renders visible quad tree nodes
	void EditorRenderer::RenderVisibleQuadTreeNodes(QuadTreeNode* pNode)
	{		
		if(pNode->IsInFrustum())
		{
			for(u32 i=0; i<QuadTreeNode::T_Count; ++i)
			{
				if(QuadTreeNode* pChild = pNode->GetChild((QuadTreeNode::E_NodeType)i))
				{
					RenderVisibleQuadTreeNodes(pChild);
				}
			}

			if(pNode->IsLeaf())
			{
				RenderQuadTreeNode(pNode, Color::Yellow);
			}
		}
	}

	//! Renders a quad tree node
	void EditorRenderer::RenderQuadTreeNode(QuadTreeNode* pNode, const Color& color)
	{
		GraphicsDriver::Instance()->SetColor(color);
		GraphicsDriver::Instance()->DrawDebugBox(pNode->GetBBox());
	}

	//! Renders a camera frustum
	void EditorRenderer::RenderCameraFrustum(Camera* pCamera)
	{
		GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_World, Matrix44::Identity);
		GraphicsDriver::Instance()->SetColor(Color::Yellow);
		const Frustum& frustum = pCamera->GetFrustum();

		GraphicsDriver::Instance()->DrawLine(frustum.m_NTopLeft, frustum.m_FTopLeft);					
		GraphicsDriver::Instance()->DrawLine(frustum.m_NTopRight, frustum.m_FTopRight);
		GraphicsDriver::Instance()->DrawLine(frustum.m_NBottomLeft, frustum.m_FBottomLeft);					
		GraphicsDriver::Instance()->DrawLine(frustum.m_NBottomRight, frustum.m_FBottomRight);

		GraphicsDriver::Instance()->DrawLine(frustum.m_NTopLeft, frustum.m_NTopRight);
		GraphicsDriver::Instance()->DrawLine(frustum.m_NBottomLeft, frustum.m_NBottomRight);
		GraphicsDriver::Instance()->DrawLine(frustum.m_NTopLeft, frustum.m_NBottomLeft);
		GraphicsDriver::Instance()->DrawLine(frustum.m_NTopRight, frustum.m_NBottomRight);

		GraphicsDriver::Instance()->DrawLine(frustum.m_FTopLeft, frustum.m_FTopRight);
		GraphicsDriver::Instance()->DrawLine(frustum.m_FBottomLeft, frustum.m_FBottomRight);
		GraphicsDriver::Instance()->DrawLine(frustum.m_FTopLeft, frustum.m_FBottomLeft);
		GraphicsDriver::Instance()->DrawLine(frustum.m_FTopRight, frustum.m_FBottomRight);

		// draw normals
		for(u32 i=0; i<Frustum::P_Count; ++i)
		{
			const Plane& plane = frustum.GetPlane((Frustum::E_Plane)i);
			Vector3 vCenter = plane.Normal*plane.D;
			GraphicsDriver::Instance()->DrawLine(vCenter, vCenter+plane.Normal);
		}
	}

	//! set picking info
	void EditorRenderer::SetPickingInfo(const Vector3& vRayStart, const Vector3& vRayDir, const Vector3& vIntersection)
	{
		m_vPickStart = vRayStart;
		m_vPickEnd = vRayStart + vRayDir*999.0f;
		m_vPickIntersection = vIntersection;
	}
}
