/* 

Amine Rehioui
Created: May 8th 2010

*/

#include "Shoot.h"

#include "EventManager.h"

#include "VertexBuffer.h"

#include "ScreenAlignedQuad.h"

#include "Text.h"

namespace shoot
{
	//! Constructor
	EntityRenderer::EntityRenderer()
		: m_bRenderViewField(true)
	{
		m_pScreenAlignedQuad = snew ScreenAlignedQuad();
	}

	//! Destructor
	EntityRenderer::~EntityRenderer()
	{
		sdelete(m_pScreenAlignedQuad);
	}

	//! Initializes the renderer
	void EntityRenderer::Init(const Size& screenSize)
	{
		if(Camera* p3DCamera = Get3DCamera())
		{
			p3DCamera->SetProjectionMatrixDirty();
		}

		if(Camera* p2DCamera = Get2DCamera())
		{
			p2DCamera->SetProjectionMatrixDirty();
			p2DCamera->SetViewMatrixDirty();
		}

		//if(!m_RenderTarget.Get())
		//{
		//	m_RenderTarget = GraphicsDriver::Instance()->CreateRenderTarget(screenSize);
		//}

		//if(!m_ShadowMap.Get())
		//{
		//	m_ShadowMap = GraphicsDriver::Instance()->CreateShadowMap(Size(1024, 1024));
		//}

		//m_RenderTarget->Resize(screenSize);
		//m_pScreenAlignedQuad->SetTexture(m_RenderTarget);
	}

	//! registers the entity for rendering
	void EntityRenderer::RegisterEntity(RenderableEntity* pEntity)
	{
		if(GraphicComponent* pGraphic = pEntity->GetComponent<GraphicComponent>())
		{
			switch(pGraphic->GetRenderingPass())
			{
			case GraphicComponent::RP_2D:
				AddToRenderMap(m_Solid2DRenderMap, pGraphic);
				break;

			case GraphicComponent::RP_3D:
				AddToRenderMap(m_Solid3DRenderMap, pGraphic);
				break;

			case GraphicComponent::RP_Transparent2D:
				AddToRenderMap(m_Transparent2DRenderMap, pGraphic);
				break;

			case GraphicComponent::RP_Transparent3D:
				AddToRenderMap(m_Transparent3DRenderMap, pGraphic);
				break;

			case GraphicComponent::RP_SkyBox:
				AddToRenderMap(m_SkyBoxMap, pGraphic);
				break;

			case GraphicComponent::RP_2DPostFade:
				AddToRenderMap(m_Solid2DPostFadeRenderMap, pGraphic);
				break;

			case GraphicComponent::RP_None:
				break;
			}
		}
	}

	//! registers the entity for rendering
	void EntityRenderer::RegisterDebugEntity(RenderableEntity* pEntity)
	{
		if(Text* pText = DYNAMIC_CAST(pEntity, Text))
		{
			pText->PreRender();
		}

		if(GraphicComponent* pGraphic = pEntity->GetComponent<GraphicComponent>())
		{
			switch(pGraphic->GetRenderingPass())
			{
			case GraphicComponent::RP_2D:
				AddToRenderMap(m_Debug2DMap, pGraphic);
				break;
			}
		}
	}

	//! clears the rendering lists
	void EntityRenderer::Clear()
	{
		m_Solid3DRenderMap.clear();
		m_Transparent3DRenderMap.clear();
		m_Solid2DRenderMap.clear();
		m_Transparent2DRenderMap.clear();
		m_Solid2DPostFadeRenderMap.clear();
		m_SkyBoxMap.clear();
		m_Debug2DMap.clear();
		m_Stats = Stats();
	}

	//! renders the entities
	void EntityRenderer::Render()
	{
		//GraphicsDriver::Instance()->SetRenderTarget(m_RenderTarget.Get());

		if(m_3DCamera.IsValid())
		{
			GraphicsDriver::Instance()->SetRenderState(GraphicsDriver::RS_DepthFunc, GraphicsDriver::CF_Less);
			GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_Projection, m_3DCamera->GetProjectionMatrix());
			GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_World, Matrix44::Identity);

			// render skybox
			if(m_SkyBoxMap.size())
			{
				Render(m_SkyBoxMap);
			}

			// set up 3D view
			GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_View, m_3DCamera->GetViewMatrix());
			GraphicsDriver::Instance()->SetRenderState(GraphicsDriver::RS_DepthTesting, true);
			GraphicsDriver::Instance()->SetRenderState(GraphicsDriver::RS_DepthWriting, true);

			// render 3D entities
			Render(m_Solid3DRenderMap);

			// render transparent 3D entities
			Render(m_Transparent3DRenderMap);
		}
		
		if(m_2DCamera.IsValid())
		{
			// set up 2D view
			GraphicsDriver::Instance()->ClearBuffers(GraphicsDriver::BF_DepthBuffer);

			GraphicsDriver::Instance()->SetRenderState(GraphicsDriver::RS_DepthTesting, true);
			GraphicsDriver::Instance()->SetRenderState(GraphicsDriver::RS_DepthWriting, true);
			// change depth func to make last rendered objects pass if their depth is the same as previously rendered objects
			GraphicsDriver::Instance()->SetRenderState(GraphicsDriver::RS_DepthFunc, GraphicsDriver::CF_LessOrEqual);
			GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_Projection, m_2DCamera->GetProjectionMatrix());
			GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_View, m_2DCamera->GetViewMatrix());

			// render 2D entities
			Render(m_Solid2DRenderMap);

			// render transparent 2D entities
			Render(m_Transparent2DRenderMap);

			// render the fade effect
			FadeRenderer::Instance()->Render();

			// render 2D entities that are not affected by the fade effect
			Render(m_Solid2DPostFadeRenderMap);

			// render 2D debug entities
			m_Stats.Enabled = false;
			Render(m_Debug2DMap);

#if SHOOT_PLATFORM == SHOOT_PLATFORM_WIN32
			// render black bars outside the view field
			if(m_bRenderViewField)
			{
				f32 fDeltaOffset = m_2DCamera->Get2DOffset().X - m_2DCamera->Get2DOffset().Y;
				if(!Math::FIsZero(fDeltaOffset))
				{
					Vector2 vScreenSize = GraphicsDriver::Instance()->GetViewPort().Size();
					GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_View, Matrix44::Identity);
					GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_World, Matrix44::Identity);
					GraphicsDriver::Instance()->SetRenderState(GraphicsDriver::RS_AlphaBlending, false);
					GraphicsDriver::Instance()->SetRenderState(GraphicsDriver::RS_2DTextureMapping, false);
					GraphicsDriver::Instance()->SetColor(Color::Black);
					if(fDeltaOffset > 0.0f)
					{
						f32 fBarWidth = m_2DCamera->Get2DOffset().X;
						GraphicsDriver::Instance()->Draw2DQuad(Vector2(), Vector2(fBarWidth, vScreenSize.Y));
						GraphicsDriver::Instance()->Draw2DQuad(Vector2(vScreenSize.X-fBarWidth, 0.0f), Vector2(fBarWidth, vScreenSize.Y));
					}
					else
					{
						f32 fBarHeight = m_2DCamera->Get2DOffset().Y;
						GraphicsDriver::Instance()->Draw2DQuad(Vector2(), Vector2(vScreenSize.X, fBarHeight));
						GraphicsDriver::Instance()->Draw2DQuad(Vector2(0.0f, vScreenSize.Y-fBarHeight), Vector2(vScreenSize.X, fBarHeight));
					}
				}
			}
#endif // SHOOT_PLATFORM
		}

		//GraphicsDriver::Instance()->ClearBuffers(GraphicsDriver::BF_DepthBuffer);

		//GraphicsDriver::Instance()->SetRenderTarget(NULL);

		//m_pScreenAlignedQuad->Render();
	}

	//! change the current 3D camera
	void EntityRenderer::Set3DCamera(Camera* pCamera)
	{
		m_3DCamera = pCamera;
		if(pCamera)
		{
			pCamera->SetProjectionMatrixDirty();
		}
	}

	//! changes the current 2D camera
	void EntityRenderer::Set2DCamera(Camera* pCamera)
	{ 
		m_2DCamera = pCamera;
		if(pCamera)
		{
			pCamera->SetProjectionMatrixDirty();
		}
	}

	//! adds an entity to a render map
	void EntityRenderer::AddToRenderMap(RenderMap& renderMap, GraphicComponent* pGraphic)
	{
		Material* pMaterial = pGraphic->GetMaterial();
		VertexBuffer* pVertexBuffer = pGraphic->GetVertexBuffer();
		
		if(pMaterial && pVertexBuffer)
		{
			if(pVertexBuffer->GetNumVertices() || pVertexBuffer->GetNumIndices())
			{
				SHOOT_ASSERT(pGraphic->GetParent()->IsA(RenderableEntity::TypeID), "Invalid GraphicComponent");
				RenderableEntity* pEntity = static_cast<RenderableEntity*>(pGraphic->GetParent());
				u32 materialID = (u32)pMaterial;
				u32 ID = ((pGraphic->GetRenderingPriority()&0xFF)<<24) | (materialID&0x00FFFFFF);

				u32 vbID = (u32)pVertexBuffer;
				renderMap[ID].pMaterial = pMaterial;
				renderMap[ID].m_VertexMap[vbID].pVertexBuffer = pVertexBuffer;
				renderMap[ID].m_VertexMap[vbID].aWorldTransforms.push_back(pEntity->GetTransformationMatrix());
				renderMap[ID].m_VertexMap[vbID].aTextureTransforms.push_back(pEntity->GetTextureTransformationMatrix());
			}
		}
		else
		{
			SHOOT_WARNING(false, "Incomplete GraphicComponent found");
		}
	}

	//! renders from a render map
	void EntityRenderer::Render(RenderMap& renderMap)
	{
		for(RenderMap::iterator it = renderMap.begin(); it != renderMap.end(); ++it)
		{
			Material* pMaterial = (*it).second.pMaterial;
			pMaterial->Begin();

			for(VertexMap::iterator it2 = (*it).second.m_VertexMap.begin(); 
				it2 != (*it).second.m_VertexMap.end();
				++it2)
			{
				VertexInfo& vertexInfo = (*it2).second;
				vertexInfo.pVertexBuffer->Begin();
				vertexInfo.pVertexBuffer->Draw(vertexInfo.aWorldTransforms, vertexInfo.aTextureTransforms, pMaterial);
				vertexInfo.pVertexBuffer->End();
			}
			
			pMaterial->End();
			m_Stats.Increment(pMaterial);
		}
	}

	//! increment from a vertex buffer
	void EntityRenderer::Stats::Increment(VertexBuffer* pVertexBuffer, u32 numInstances /*= 1*/)
	{
#ifndef _RETAIL_
		if(Enabled)
		{
			NumVertices += pVertexBuffer->GetNumVertices();
			if(pVertexBuffer->GetPrimitiveType() == GraphicsDriver::PT_Triangle)
			{
				NumTriangles += pVertexBuffer->GetNumVertices()/3;
			}
			++NumDrawCalls;
		}
#endif // _RETAIL_
	}

	//! increment from a material
	void EntityRenderer::Stats::Increment(Material* pMaterial)
	{
#ifndef _RETAIL_
		if(Enabled)
		{
			++NumMaterials;
		}
#endif // _RETAIL_
	}
}

