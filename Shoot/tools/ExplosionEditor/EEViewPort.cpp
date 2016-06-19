/* 

Amine Rehioui
Created: December 24th 2012

*/

#include "ShootEditorCommon.h"

#include "EEViewPort.h"

#include "CameraManager.h"
#include "EditorSettings.h"

#include "ExplosionEditor.h"
#include "Triangle.h"
#include "Plane.h"

// Define event table
BEGIN_EVENT_TABLE(EEViewPort, wxGLCanvas)
	EVT_MOTION(EEViewPort::OnMouseMove)
	EVT_LEFT_DOWN(EEViewPort::OnMouseLeftDown)
	EVT_LEFT_UP(EEViewPort::OnMouseLeftUp)
	EVT_MIDDLE_DOWN(EEViewPort::OnMouseMiddleDown)
	EVT_MIDDLE_UP(EEViewPort::OnMouseMiddleUp)
	EVT_RIGHT_DOWN(EEViewPort::OnMouseRightDown)
	EVT_RIGHT_UP(EEViewPort::OnMouseRightUp)
	EVT_MOUSEWHEEL(EEViewPort::OnMouseWheelRolled)
	EVT_KEY_DOWN(EEViewPort::OnKeyDown)
	EVT_KEY_UP(EEViewPort::OnKeyUp)
	EVT_LEAVE_WINDOW(EEViewPort::OnMouseLeave)
	EVT_SIZE(EEViewPort::OnResized)
	EVT_SET_FOCUS(EEViewPort::OnFocusGained)
	EVT_KILL_FOCUS(EEViewPort::OnFocusLost)
	EVT_PAINT(EEViewPort::OnPaint)
	EVT_ERASE_BACKGROUND(EEViewPort::OnEraseBackground)
END_EVENT_TABLE()

//! Constructor
EEViewPort::EEViewPort(wxWindow *parent)
	: super(parent, (wxGLCanvas*)NULL, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE)
	, m_bZoomingStarted(false)
	, m_bLookingStarted(false)
	, m_SelectedTriangleIndex(-1)
	, m_SelectedSubMeshIndex(-1)
{
	SetCurrent();

	EditorSettings::CreateInstance();
	CameraManager::CreateInstance();
	CameraManager::Instance()->SetMode(CameraManager::M_Inspect);
}

//! Destructor
EEViewPort::~EEViewPort()
{
	EditorSettings::DestroyInstance();
	CameraManager::DestroyInstance();
}

//! returns the index of a triangle in a chunk, -1 if not found
s32 EEViewPort::GetTriangleIndex(ExplosionVisitor::Chunk& chunk, u32 triangle)
{
	s32 index = -1;
	for(u32 i=0; i<chunk.aTriangleIndices.GetSize(); ++i)
	{
		if(chunk.aTriangleIndices[i] == triangle)
		{
			index = i;
			break;
		}
	}
	return index;
}

//! returns the index of a chunk containing a triangle, -1 if not found
s32 EEViewPort::GetChunkIndex(Array<ExplosionVisitor::Chunk>& aChunks, u32 triangle)
{
	s32 index = -1;
	for(u32 i=0; i<aChunks.GetSize(); ++i)
	{
		for(u32 j=0; j<aChunks[i].aTriangleIndices.GetSize(); ++j)
		{
			if(aChunks[i].aTriangleIndices[j] == triangle)
			{
				index = i;
				break;
			}
		}
	}	
	return index;
}

// event handlers
void EEViewPort::OnMouseMove(wxMouseEvent& event)
{
	// Update the status bar
	char strPosition[128];
	std::stringstream ss;
	ss << event.GetX() << ", " << event.GetY();
	ss.get(strPosition, 128);
	static_cast<wxFrame*>(wxTheApp->GetTopWindow())->SetStatusText(strPosition, 1);

	Vector2 mousePos(event.GetX(), event.GetY());
	Vector2 deltaPos = mousePos - m_LastMousePosition;
	Camera* pCamera = EntityRenderer::Instance()->Get3DCamera();

	if(m_bZoomingStarted)
	{
		CameraManager::Instance()->ZoomCamera(pCamera, deltaPos.Y);
	}
	else if(m_bLookingStarted)
	{
		CameraManager::Instance()->RotateCamera(pCamera, deltaPos);
	}
	else
	{
		// determine the selected triangle
		m_SelectedTriangleIndex = -1;
		m_SelectedSubMeshIndex = -1;
		static_cast<wxFrame*>(wxTheApp->GetTopWindow())->SetStatusText("", 3);

		ExplosionEditor* pEditor = static_cast<ExplosionEditor*>(wxTheApp->GetTopWindow());		
		if(MeshEntity* pMesh = pEditor->GetCurrentMesh())
		{
			Vector3 rayStart, rayDir;
			pCamera->Get3DRayFromScreenCoords(rayStart, rayDir, Point(s32(mousePos.X), s32(mousePos.Y)), Vector2(GetSize().x, GetSize().y));
			f32 fMinDist = Math::Maxf32;

			std::vector<SubMesh*> subMeshes = pMesh->GetSubMeshes();
			for(u32 i=0; i<subMeshes.size(); ++i)
			{
				VertexBuffer* pVertexBuffer = subMeshes[i]->GetComponent<GraphicComponent>()->GetVertexBuffer();
				u16* pIndices = pVertexBuffer->GetIndices();
				Vertex3D* pVertices = pVertexBuffer->GetVertices();
				u32 numTriangles = pVertexBuffer->GetNumIndices()/3;

				for(u32 j=0; j<numTriangles; ++j)
				{
					u32 index1 = pIndices[j*3 + 0];
					u32 index2 = pIndices[j*3 + 1];
					u32 index3 = pIndices[j*3 + 2];

					Vector3 v1 = pVertexBuffer->GetVertices()[index1].Pos;
					Vector3 v2 = pVertexBuffer->GetVertices()[index2].Pos;
					Vector3 v3 = pVertexBuffer->GetVertices()[index3].Pos;

					Plane plane(v1, v2, v3);
					Vector3 vIntersection;
					Plane::E_Classification eClass;
					if(plane.IntersectWithRay(rayStart, rayDir, &vIntersection, &eClass))
					{
						if(eClass == Plane::C_Front)
						{
							Triangle triangle(v1, v2, v3);
							if(triangle.IsPointInside(vIntersection))
							{
								f32 fDist = (vIntersection - rayStart).GetLengthSquared();
								if(fDist < fMinDist)
								{
									m_SelectedTriangleIndex = j;
									m_SelectedSubMeshIndex = i;
									fMinDist = fDist;
								}
							}
						}
					}
				}
			}

			if(m_SelectedTriangleIndex >= 0)
			{
				char buf[128];
				std::stringstream ss;
				ss << "SubMesh: " << m_SelectedSubMeshIndex;
				ss.get(buf, 128);
				static_cast<wxFrame*>(wxTheApp->GetTopWindow())->SetStatusText(buf, 3);
			}
		}
	}

	m_LastMousePosition = mousePos;
}

void EEViewPort::OnMouseLeftDown(wxMouseEvent& event)
{
	SetFocus();

	Vector2 clickPos(event.GetX(), event.GetY());
	if(event.AltDown())
	{
		m_bLookingStarted = true;
	}
	else
	{
		ExplosionEditor* pEditor = static_cast<ExplosionEditor*>(wxTheApp->GetTopWindow());
		if(m_SelectedTriangleIndex < 0)
		{
			pEditor->SetSelection(-1, -1);			
		}
		else
		{
			MeshEntity* pMesh = pEditor->GetCurrentMesh();
			ExplosionVisitor* pExplosionVisitor = pMesh->GetComponent<ExplosionVisitor>();
			Array<ExplosionVisitor::ChunkGroup>& aChunkGroups = pExplosionVisitor->GetChunkGroups();
			s32 selectedChunk = pEditor->GetSelectedChunk();
			s32 selectedSubMesh = pEditor->GetSelectedSubMesh();

			if(selectedChunk >= 0
			&& selectedSubMesh == m_SelectedSubMeshIndex
			&& event.ControlDown())
			{
				// find out if the selected triangle is part of the selected chunk				
				s32 triangleIndexInChunk = GetTriangleIndex(aChunkGroups[selectedSubMesh].m_aChunks[selectedChunk], m_SelectedTriangleIndex);

				// flip the selected triangle status
				if(triangleIndexInChunk >= 0)
				{
					aChunkGroups[selectedSubMesh].m_aChunks[selectedChunk].aTriangleIndices.Delete(triangleIndexInChunk);
				}
				else
				{
					aChunkGroups[selectedSubMesh].m_aChunks[selectedChunk].aTriangleIndices.Add(snew u32(m_SelectedTriangleIndex));

					// make sure the triangle is removed from every other chunk
					for(u32 i=0; i<aChunkGroups[selectedSubMesh].m_aChunks.GetSize(); ++i)
					{
						if(i != selectedChunk)
						{
							s32 triangleIndexInChunk = GetTriangleIndex(aChunkGroups[selectedSubMesh].m_aChunks[i], m_SelectedTriangleIndex);
							if(triangleIndexInChunk >= 0)
							{
								aChunkGroups[selectedSubMesh].m_aChunks[i].aTriangleIndices.Delete(triangleIndexInChunk);
							}
						}
					}
				}
				
				pEditor->OnChunksChanged();
			}
			else
			{
				s32 chunkIndex = GetChunkIndex(aChunkGroups[m_SelectedSubMeshIndex].m_aChunks, m_SelectedTriangleIndex);
				if(chunkIndex >= 0)
				{
					pEditor->SetSelection(chunkIndex, m_SelectedSubMeshIndex);					
				}
			}
		}
	}
}

void EEViewPort::OnMouseLeftUp(wxMouseEvent& event)
{
	m_bLookingStarted = false;
}

void EEViewPort::OnMouseMiddleDown(wxMouseEvent& event)
{
}

void EEViewPort::OnMouseMiddleUp(wxMouseEvent& event)
{
}

void EEViewPort::OnMouseRightDown(wxMouseEvent& event)
{
	if(event.AltDown())
	{
		m_bZoomingStarted = true;
	}
}

void EEViewPort::OnMouseRightUp(wxMouseEvent& event)
{
	m_bZoomingStarted = false;
}

void EEViewPort::OnMouseWheelRolled(wxMouseEvent& event)
{			
}

void EEViewPort::OnKeyDown(wxKeyEvent& event)
{
}

void EEViewPort::OnKeyUp(wxKeyEvent& event)
{				
}

void EEViewPort::OnMouseLeave(wxMouseEvent& event)
{	
}

void EEViewPort::OnResized(wxSizeEvent& event)
{
	super::OnSize(event);

	if(GetContext())	
	{
		SetCurrent();
		Size newSize(event.GetSize().x, event.GetSize().y);	
		Engine::Instance()->ResizeScreen(newSize);
	}
}

void EEViewPort::OnFocusGained(wxFocusEvent& event)
{
	Refresh(true); // make sure background is erased and refreshed		
}

void EEViewPort::OnFocusLost(wxFocusEvent& event)
{
}

void EEViewPort::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	if(GetContext())
	{
		SetCurrent();

		GraphicsDriver::Instance()->SetViewPort(AABBox2D(Vector2(0.0f, 0.0f), Vector2(f32(GetSize().x), f32(GetSize().y))));		
		GraphicsDriver::Instance()->SetRenderState(GraphicsDriver::RS_FillMode, GraphicsDriver::FM_Solid);

		Engine::Instance()->Render();

		// custom rendering
		GraphicsDriver::Instance()->SetRenderState(GraphicsDriver::RS_DepthFunc, GraphicsDriver::CF_LessOrEqual);
		GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_Projection, EntityRenderer::Instance()->Get3DCamera()->GetProjectionMatrix());
		GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_View, EntityRenderer::Instance()->Get3DCamera()->GetViewMatrix());
		GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_World, Matrix44::Identity);
		ExplosionEditor* pEditor = static_cast<ExplosionEditor*>(wxTheApp->GetTopWindow());
		MeshEntity* pMesh = pEditor->GetCurrentMesh();
		ExplosionVisitor* pExplosionVisitor = pMesh ? pMesh->GetComponent<ExplosionVisitor>() : NULL;
		if(pMesh && pExplosionVisitor)
		{
			Color wireColor(0.5f, 0.5f, 0.5f);
			Color selectedColor(Color::White);
			Color chunkColor(Color(0.5f, 0.0f, 0.5f, 0.5f));
			Color selectedChunkColor(Color(1.0f, 1.0f, 0.0f, 0.5f));

			std::vector<SubMesh*> subMeshes = pMesh->GetSubMeshes();
			Array<ExplosionVisitor::ChunkGroup>& aChunkGroups = pExplosionVisitor->GetChunkGroups();
			for(u32 i=0; i<subMeshes.size(); ++i)
			{
				VertexBuffer* pVB = subMeshes[i]->GetComponent<GraphicComponent>()->GetVertexBuffer();
				GraphicsDriver::Instance()->SetRenderState(GraphicsDriver::RS_FillMode, GraphicsDriver::FM_Line);
				GraphicsDriver::Instance()->SetRenderState(GraphicsDriver::RS_2DTextureMapping, false);
				GraphicsDriver::Instance()->SetColor(wireColor);
				GraphicsDriver::Instance()->DrawIndexedPrimitiveList(GraphicsDriver::PT_Triangle, Vertex3D::VF_Pos, pVB->GetVertices(), pVB->GetNumIndices(), pVB->GetIndices());

				// render explosion chunks	
				s32 selectedChunk = pEditor->GetSelectedChunk();
				s32 selectedSubMesh = pEditor->GetSelectedSubMesh();

				GraphicsDriver::Instance()->SetRenderState(GraphicsDriver::RS_AlphaBlending, true);
				GraphicsDriver::Instance()->SetRenderState(GraphicsDriver::RS_FillMode, GraphicsDriver::FM_Solid);
				Array<ExplosionVisitor::Chunk>& aChunks = aChunkGroups[i].m_aChunks;
				for(u32 j=0; j<aChunks.GetSize(); ++j)
				{
					if(i == selectedSubMesh
					&& j == selectedChunk)
					{
						GraphicsDriver::Instance()->SetCullMode(GraphicsDriver::CM_None);
						GraphicsDriver::Instance()->SetColor(selectedChunkColor);	
					}
					else
					{
						GraphicsDriver::Instance()->SetCullMode(GraphicsDriver::CM_CounterClockWise);
						GraphicsDriver::Instance()->SetColor(chunkColor);						
					}

					for(u32 k=0; k<aChunks[j].aTriangleIndices.GetSize(); ++k)
					{
						u32 triangleIndex = aChunks[j].aTriangleIndices[k];
						GraphicsDriver::Instance()->DrawIndexedPrimitiveList(GraphicsDriver::PT_Triangle, Vertex3D::VF_Pos, pVB->GetVertices(), 3, pVB->GetIndices()+(triangleIndex*3));
					}
				}

				GraphicsDriver::Instance()->SetCullMode(GraphicsDriver::CM_CounterClockWise);
				if(m_SelectedTriangleIndex >= 0 && m_SelectedSubMeshIndex == i)
				{
					GraphicsDriver::Instance()->SetRenderState(GraphicsDriver::RS_FillMode, GraphicsDriver::FM_Line);
					GraphicsDriver::Instance()->SetColor(selectedColor);
					GraphicsDriver::Instance()->DrawIndexedPrimitiveList(GraphicsDriver::PT_Triangle, Vertex3D::VF_Pos, pVB->GetVertices(), 3, pVB->GetIndices()+(m_SelectedTriangleIndex*3));
				}
			}
		}

		glFlush();
		SwapBuffers();
	}
}

void EEViewPort::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{
	// Do nothing, to avoid flashing.
}

