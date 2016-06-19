/* 

Amine Rehioui
Created: June 25th 2011

*/

#include "Shoot.h"

#include "ScreenAlignedQuad.h"

#include "Vertex3D.h"

namespace shoot
{
	//! renders the quad		
	void ScreenAlignedQuad::Render()
	{
		GraphicsDriver::Instance()->SetRenderState(GraphicsDriver::RS_DepthWriting, false);
		GraphicsDriver::Instance()->SetRenderState(GraphicsDriver::RS_AlphaBlending, false);
		GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_Projection, Matrix44::Identity);
		GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_World, Matrix44::Identity);
		GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_View, Matrix44::Identity);
		GraphicsDriver::Instance()->SetTransform(GraphicsDriver::TS_Texture, Matrix44::Identity);
		GraphicsDriver::Instance()->SetColor(Color::White);
		
		if(Texture* pTexture = m_Texture.Get())
		{
			pTexture->Begin(0);
		}

		static const Vertex3D Quad[] = 
		{
			{ Vector3::Create(-1.0f, -1.0f, 0.0f), Vector2(0.0f, 0.0f), Vector3::Zero, Color(), Vector3::Zero },
			{ Vector3::Create(-1.0f, 1.0f, 0.0f), Vector2(0.0f, 1.0f), Vector3::Zero, Color(), Vector3::Zero },
			{ Vector3::Create(1.0f, 1.0f, 0.0f), Vector2(1.0f, 1.0f), Vector3::Zero, Color(), Vector3::Zero },
			{ Vector3::Create(-1.0f, -1.0f, 0.0f), Vector2(0.0f, 0.0f), Vector3::Zero, Color(), Vector3::Zero },
			{ Vector3::Create(1.0f, 1.0f, 0.0f), Vector2(1.0f, 1.0f), Vector3::Zero, Color(), Vector3::Zero },
			{ Vector3::Create(1.0f, -1.0f, 0.0f), Vector2(1.0f, 0.0f), Vector3::Zero, Color(), Vector3::Zero }
		};

		GraphicsDriver::Instance()->DrawPrimitiveList(GraphicsDriver::PT_Triangle, Vertex3D::VF_Pos | Vertex3D::VF_UV, 6, Quad);
	}	
}

