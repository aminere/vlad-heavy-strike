/* 

Amine Rehioui
Created: July 10th 2010

*/

#include "Shoot.h"

#include "OpenGLDriver.h"
#include "OpenGLTexture.h"
#include "OpenGLShader.h"
#include "OpenGLCubeMapTexture.h"
#include "SkyBoxVertexBufferNoCubemap.h"
#include "FakeCubeMapTexture.h"
#include "OpenGLRenderTarget.h"
#include "OpenGLShadowMap.h"
#include "OpenGLVertexBuffer.h"

#include "OpenGLExtensionHandler.h"

#include "VertexBuffer.h"

namespace shoot
{
	//! destructor
	OpenGLDriver::~OpenGLDriver()
	{
	}

	//! driver initialization
	void OpenGLDriver::Init()
	{
		super::Init();

		SetCullMode(CM_CounterClockWise);
	}

	//! begins taking graphic commands
	void OpenGLDriver::Begin(bool bClearBackBuffer /*= true*/, bool bClearDepthBuffer /*= true*/, Color clearColor /*= Color()*/)
	{		
		int mask = 0;
		if(bClearBackBuffer) { mask |= GL_COLOR_BUFFER_BIT; }
		if(bClearDepthBuffer) { mask |= GL_DEPTH_BUFFER_BIT; }
		GL_CHECK(glClearColor(clearColor.R, clearColor.G, clearColor.B, clearColor.A));
		GL_CHECK(glClear(mask));
	}

	//! create a render target texture
	Texture* OpenGLDriver::CreateRenderTarget(const Vector2& vSize)
	{
		return snew OpenGLRenderTarget(vSize);
	}

	//! create a shadow map texture
	Texture* OpenGLDriver::CreateShadowMap(const Vector2& vSize)
	{
		return snew OpenGLShadowMap(vSize);
	}

	//! sets a render target
	/** \param pTarget: render target texture. Pass NULL to select the regular back buffer */
	void OpenGLDriver::SetRenderTarget(Texture* pTarget)
	{
		if(pTarget)
		{
			GL_CHECK(glViewport(0, 0, u32(pTarget->GetSize().X), u32(pTarget->GetSize().Y)));
			switch(pTarget->GetType())
			{
			case Texture::T_RenderTarget:
				OpenGLExtensionHandler::Instance()->extGlBindFramebuffer(GL_FRAMEBUFFER_EXT, static_cast<OpenGLRenderTarget*>(pTarget)->GetFrameBufferID());
				break;

			case Texture::T_ShadowMap:
				OpenGLExtensionHandler::Instance()->extGlBindFramebuffer(GL_FRAMEBUFFER_EXT, static_cast<OpenGLShadowMap*>(pTarget)->GetFrameBufferID());
				break;

			default:
				SHOOT_ASSERT(false, "Render target has wrong type");
			}			
		}
		else
		{
			GraphicsDriver::Instance()->SetViewPort(GraphicsDriver::Instance()->GetViewPort());
			OpenGLExtensionHandler::Instance()->extGlBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
		}

		ClearBuffers(BF_BackBuffer | BF_DepthBuffer);
	}

	//! draws a list of primitives from batch of vertices
	/** \param eType: primitive type 
		\param vertexFlags: a combination of E_VertexFlag 
		\param numVertices: number of vertices provided 
		\param pVertices: vertex array */
	void OpenGLDriver::DrawPrimitiveList(E_PrimitiveType eType, u32 vertexFlags, u32 numVertices, const Vertex3D* pVertices)
	{
		static s32 glPrimitives[] = { GL_POINTS,
									  GL_LINES,
									  GL_LINE_LOOP,
									  GL_LINE_STRIP,
									  GL_TRIANGLES,
									  GL_TRIANGLE_STRIP,
									  GL_TRIANGLE_FAN };

		const bool bPos = ((vertexFlags & Vertex3D::VF_Pos) != 0);
		const bool bUV = ((vertexFlags & Vertex3D::VF_UV) != 0);
		const bool bNormal = ((vertexFlags & Vertex3D::VF_Normal) != 0);
		const bool bColor = ((vertexFlags & Vertex3D::VF_Color) != 0);
		const bool bUVW = ((vertexFlags & Vertex3D::VF_UVW) != 0);

		if(bPos)
		{
			GL_CHECK(glEnableClientState(GL_VERTEX_ARRAY));
			GL_CHECK(glVertexPointer(3, GL_FLOAT, sizeof(Vertex3D), &(pVertices[0].Pos)));
		}
		else
		{
			GL_CHECK(glDisableClientState(GL_VERTEX_ARRAY));
		}

		if(bUV || bUVW)
		{
			GL_CHECK(glEnableClientState(GL_TEXTURE_COORD_ARRAY));

			if(bUV)
			{
				GL_CHECK(glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex3D), &(pVertices[0].UV)));
			}
			else			
			{
				GL_CHECK(glTexCoordPointer(3, GL_FLOAT, sizeof(Vertex3D), &(pVertices[0].UVW)));
			}
		}
		else
		{
			GL_CHECK(glDisableClientState(GL_TEXTURE_COORD_ARRAY));
		}
		
		if(bNormal)
		{
			GL_CHECK(glEnableClientState(GL_NORMAL_ARRAY));
			GL_CHECK(glNormalPointer(GL_FLOAT, sizeof(Vertex3D), &(pVertices[0].Normal)));
		}
		else
		{
			GL_CHECK(glDisableClientState(GL_NORMAL_ARRAY));
		}

		if(bColor)
		{
			GL_CHECK(glEnableClientState(GL_COLOR_ARRAY));
			GL_CHECK(glColorPointer(4, GL_FLOAT, sizeof(Vertex3D), &(pVertices[0].color)));
		}
		else
		{
			GL_CHECK(glDisableClientState(GL_COLOR_ARRAY));
		}

		GL_CHECK(glDrawArrays(glPrimitives[eType], 0, numVertices));

		bPos ? glDisableClientState(GL_VERTEX_ARRAY) : void();
		(bUV || bUVW) ? glDisableClientState(GL_TEXTURE_COORD_ARRAY) : void();
		bNormal ? glDisableClientState(GL_NORMAL_ARRAY) : void();
		bColor ? glDisableClientState(GL_COLOR_ARRAY) : void();
	}

	//! draws a list of primitives from batch of vertices
	/** \param eType: primitive type 
		\param pVertexBuffer: vertex buffer to use */
	void OpenGLDriver::DrawPrimitiveList(E_PrimitiveType eType, VertexBuffer* pVertexBuffer)
	{
		DrawPrimitiveList(eType, pVertexBuffer->GetVertexFlags(), pVertexBuffer->GetNumVertices(), pVertexBuffer->GetVertices());
	}

	//! draws a list of primitives from batch of vertices and indices
	/** \param eType: primitive type 
		\param vertexFlags: a combination of E_VertexFlag		
		\param pVertices: vertex array
		\param pIndices: index array 
		\param numIndices: number of indices provided */
	void OpenGLDriver::DrawIndexedPrimitiveList(E_PrimitiveType eType, u32 vertexFlags, Vertex3D* pVertices, u32 numIndices, u16* pIndices)
	{
		static s32 glPrimitives[] = { GL_POINTS,
									  GL_LINES,
									  GL_LINE_LOOP,
									  GL_LINE_STRIP,
									  GL_TRIANGLES,
									  GL_TRIANGLE_STRIP,
									  GL_TRIANGLE_FAN };

		const bool bPos = ((vertexFlags & Vertex3D::VF_Pos) != 0);
		const bool bUV = ((vertexFlags & Vertex3D::VF_UV) != 0);
		const bool bNormal = ((vertexFlags & Vertex3D::VF_Normal) != 0);
		const bool bColor = ((vertexFlags & Vertex3D::VF_Color) != 0);
		const bool bUVW = ((vertexFlags & Vertex3D::VF_UVW) != 0);

		if(bPos)
		{
			GL_CHECK(glEnableClientState(GL_VERTEX_ARRAY));
			GL_CHECK(glVertexPointer(3, GL_FLOAT, sizeof(Vertex3D), &(pVertices[0].Pos)));
		}
		else
		{
			GL_CHECK(glDisableClientState(GL_VERTEX_ARRAY));
		}

		if(bUV || bUVW)
		{
			GL_CHECK(glEnableClientState(GL_TEXTURE_COORD_ARRAY));

			if(bUV)
			{
				GL_CHECK(glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex3D), &(pVertices[0].UV)));
			}
			else			
			{
				GL_CHECK(glTexCoordPointer(3, GL_FLOAT, sizeof(Vertex3D), &(pVertices[0].UVW)));
			}
		}
		else
		{
			GL_CHECK(glDisableClientState(GL_TEXTURE_COORD_ARRAY));
		}
		
		if(bNormal)
		{
			GL_CHECK(glEnableClientState(GL_NORMAL_ARRAY));
			GL_CHECK(glNormalPointer(GL_FLOAT, sizeof(Vertex3D), &(pVertices[0].Normal)));
		}
		else
		{
			GL_CHECK(glDisableClientState(GL_NORMAL_ARRAY));
		}

		if(bColor)
		{
			GL_CHECK(glEnableClientState(GL_COLOR_ARRAY));
			GL_CHECK(glColorPointer(4, GL_FLOAT, sizeof(Vertex3D), &(pVertices[0].color)));
		}
		else
		{
			GL_CHECK(glDisableClientState(GL_COLOR_ARRAY));
		}
		
		GL_CHECK(glDrawElements(glPrimitives[eType], numIndices, GL_UNSIGNED_SHORT, pIndices));
	}	

	//! renders a line
	void OpenGLDriver::DrawLine(const Vector3& vStart, const Vector3& vEnd)
	{
		glBegin(GL_LINES);
		glVertex3f(vStart.X, vStart.Y, vStart.Z); glVertex3f(vEnd.X, vEnd.Y, vEnd.Z);			
		glEnd();
	}

	//! renders a 2D quad
	void OpenGLDriver::Draw2DQuad(const Vector2& vOffset, const Vector2& vSize)
	{
		const Vertex3D aVertices[] = 
		{
			{ Vector3::Create(vOffset.X, vOffset.Y, 0.0f), Vector2(), Vector3::Zero, Color(), Vector3::Zero },
			{ Vector3::Create(vOffset.X+vSize.X, vOffset.Y, 0.0f), Vector2(), Vector3::Zero, Color(), Vector3::Zero },
			{ Vector3::Create(vOffset.X+vSize.X, vOffset.Y+vSize.Y, 0.0f), Vector2(), Vector3::Zero, Color(), Vector3::Zero },
			{ Vector3::Create(vOffset.X, vOffset.Y+vSize.Y, 0.0f), Vector2(), Vector3::Zero, Color(), Vector3::Zero },
			{ Vector3::Create(vOffset.X, vOffset.Y, 0.0f), Vector2(), Vector3::Zero, Color(), Vector3::Zero },
			{ Vector3::Create(vOffset.X+vSize.X, vOffset.Y+vSize.Y, 0.0f), Vector2(), Vector3::Zero, Color(), Vector3::Zero }
		};

		DrawPrimitiveList(GraphicsDriver::PT_Triangle, Vertex3D::VF_Pos, 6, aVertices);
	}

	//! renders a debug box
	void OpenGLDriver::DrawDebugBox(const AABBox3D& box)
	{
		Vector3 vMin = box.Min();
		Vector3 vMax = box.Max();

		glBegin(GL_LINE_LOOP);				
		glVertex3f(vMin.X, vMin.Y, vMin.Z); glVertex3f(vMax.X, vMin.Y, vMin.Z); glVertex3f(vMax.X, vMax.Y, vMin.Z); glVertex3f(vMin.X, vMax.Y, vMin.Z); glVertex3f(vMin.X, vMin.Y, vMin.Z);
		glEnd();
		glBegin(GL_LINE_LOOP);				
		glVertex3f(vMin.X, vMin.Y, vMax.Z); glVertex3f(vMax.X, vMin.Y, vMax.Z); glVertex3f(vMax.X, vMax.Y, vMax.Z); glVertex3f(vMin.X, vMax.Y, vMax.Z); glVertex3f(vMin.X, vMin.Y, vMax.Z);
		glEnd();
		glBegin(GL_LINES);				
		glVertex3f(vMin.X, vMin.Y, vMin.Z); glVertex3f(vMin.X, vMin.Y, vMax.Z);
		glVertex3f(vMax.X, vMin.Y, vMin.Z); glVertex3f(vMax.X, vMin.Y, vMax.Z);
		glVertex3f(vMin.X, vMax.Y, vMin.Z); glVertex3f(vMin.X, vMax.Y, vMax.Z);
		glVertex3f(vMax.X, vMax.Y, vMin.Z); glVertex3f(vMax.X, vMax.Y, vMax.Z);
		glEnd();
	}

	//! renders a debug box
	void OpenGLDriver::DrawDebugBox(const AABBox2D& box)
	{
		Vector2 vMin = box.Min();
		Vector2 vMax = box.Max();

		glBegin(GL_LINE_LOOP);
		glVertex3f(vMin.X, vMin.Y, 0.0f); 
		glVertex3f(vMax.X, vMin.Y, 0.0f); 
		glVertex3f(vMax.X, vMax.Y, 0.0f); 
		glVertex3f(vMin.X, vMax.Y, 0.0f); 
		glVertex3f(vMin.X, vMin.Y, 0.0f);
		glEnd();
	}

	//! sets the viewport area
	void OpenGLDriver::SetViewPort(const AABBox2D& area)
	{
		GL_CHECK(glViewport(s32(area.Min().X), s32(area.Min().Y), s32(area.Max().X), s32(area.Max().Y)));
		m_ViewPortArea = area;
	}

	//! Sets transformation matrices
	/** override with driver specific code */
	void OpenGLDriver::SetTransform(E_TransformState eState, const Matrix44& matrix)
	{
		super::SetTransform(eState, matrix); 

		switch(eState)
		{
		case TS_Projection:
			GL_CHECK(glMatrixMode(GL_PROJECTION));
			GL_CHECK(glLoadMatrixf(matrix.GetFloats()));
			break;		

		case TS_World:
		case TS_View:
			GL_CHECK(glMatrixMode(GL_MODELVIEW));
			GL_CHECK(glLoadMatrixf((m_StateMatrices[TS_World] * m_StateMatrices[TS_View]).GetFloats()));			
			break;

		case TS_Texture:
			GL_CHECK(glMatrixMode(GL_TEXTURE));
			GL_CHECK(glLoadMatrixf(matrix.GetFloats()));
			break;
		}
	}

	//! Sets a render state
	void OpenGLDriver::SetRenderState(E_RenderState eState, bool bEnable)
	{
		switch(eState)
		{
		case RS_DepthTesting: 
			bEnable ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
			SHOOT_LOG_WARNING(glGetError() == GL_NO_ERROR, "GL_DEPTH_TEST change failed");
			break;

		case RS_DepthWriting:
			GL_CHECK(glDepthMask(bEnable));
			break;

		case RS_AlphaBlending:
			bEnable ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
			SHOOT_LOG_WARNING(glGetError() == GL_NO_ERROR, "GL_BLEND change failed");
			break;

		case RS_2DTextureMapping:
			bEnable ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);
			SHOOT_LOG_WARNING(glGetError() == GL_NO_ERROR, "GL_TEXTURE_2D change failed");
			break;

		case RS_PointSprite:
			bEnable ? glEnable(GL_POINT_SPRITE) : glDisable(GL_POINT_SPRITE);
			SHOOT_LOG_WARNING(glGetError() == GL_NO_ERROR, "GL_POINT_SPRITE change failed");
			GL_CHECK(glTexEnvf( GL_POINT_SPRITE, GL_COORD_REPLACE, GLfloat(bEnable ? GL_TRUE : GL_FALSE)));
			break;

		default:
			SHOOT_WARNING(0, "SetRenderState could not handle state");
		}
	}

	//! Sets a render state
	void OpenGLDriver::SetRenderState(E_RenderState eState, s32 iValue)
	{
		static s32 glCompFuncs[] = { GL_NEVER, GL_LESS, GL_EQUAL, GL_LEQUAL, GL_GREATER, GL_NOTEQUAL, GL_GEQUAL, GL_ALWAYS };

		static s32 glFillModes[] = { GL_POINT, GL_LINE, GL_FILL };

		switch(eState)
		{
		case RS_DepthFunc:
			{
				SHOOT_ASSERT(iValue < CF_Count, "Invalid E_ComparisonFunction");
				s32 func = glCompFuncs[iValue];
				GL_CHECK(glDepthFunc(func));
			}
			break;

		case RS_FillMode:
			{
				SHOOT_ASSERT(iValue < FM_Count, "Invalid E_FillMode");
				GL_CHECK(glPolygonMode(GL_FRONT_AND_BACK, glFillModes[iValue]));
			}
			break;

		default:
			SHOOT_WARNING(0, "SetRenderState could not handle state");
		}
	}

	//! Sets a render state
	void OpenGLDriver::SetRenderState(E_RenderState eState, f32 fValue)
	{
		switch(eState)
		{
		case RS_PointSize:
			GL_CHECK(glPointSize(fValue));
			GL_CHECK(glLineWidth(fValue));
			break;

		case RS_MinPointSize:
			OpenGLExtensionHandler::Instance()->extGlPointParameterf(GL_POINT_SIZE_MIN, fValue);
			break;

		case RS_MaxPointSize:
			OpenGLExtensionHandler::Instance()->extGlPointParameterf(GL_POINT_SIZE_MAX, fValue);
			break;

		default:
			SHOOT_WARNING(0, "SetRenderState could not handle state");
		}
	}

	//! Sets a render state
	void OpenGLDriver::SetRenderState(E_RenderState eState, const f32* fValues)
	{
		switch(eState)
		{
		case RS_PointDistanceAttenuation:
			OpenGLExtensionHandler::Instance()->extGlPointParameterfv(GL_POINT_DISTANCE_ATTENUATION, fValues);
			break;

		default:
			SHOOT_WARNING(0, "SetRenderState could not handle state");
		}
	}
	
	//! Sets the blend function
	void OpenGLDriver::SetBlendFunc(E_BlendFactor eSrc, E_BlendFactor eDest)
	{
		static s32 glBlendFactors[] = { GL_ZERO, 
										GL_ONE, 
										GL_SRC_COLOR, 
										GL_ONE_MINUS_SRC_COLOR, 
										GL_SRC_ALPHA, 
										GL_ONE_MINUS_SRC_ALPHA,
										GL_DST_ALPHA, 
										GL_ONE_MINUS_DST_ALPHA, 
										GL_DST_COLOR, 
										GL_ONE_MINUS_DST_COLOR, 
										GL_SRC_ALPHA_SATURATE };

		GL_CHECK(glBlendFunc(glBlendFactors[eSrc], glBlendFactors[eDest]));
	}

	//! returns a render state
	void OpenGLDriver::GetRenderState(E_RenderState eState, bool& bEnabled)
	{
		GLboolean bResult;

		switch(eState)
		{
		case RS_DepthTesting:			
			GL_CHECK(glGetBooleanv(GL_DEPTH_TEST, &bResult));			
			break;

		case RS_DepthWriting:
			GL_CHECK(glGetBooleanv(GL_DEPTH_WRITEMASK, &bResult));			
			break;

		case RS_AlphaBlending:
			GL_CHECK(glGetBooleanv(GL_BLEND, &bResult));						
			break;

		default:
			SHOOT_WARNING(0, "GetRenderState could not handle state");
		}

		bEnabled = (bResult != 0);
	}

	//! returns a render state
	void OpenGLDriver::GetRenderState(E_RenderState eState, s32& iValue)
	{
		GLint iResult;

		switch(eState)
		{
		case RS_DepthFunc:
			GL_CHECK(glGetIntegerv(GL_DEPTH_FUNC, &iResult));			
			break;		

		case RS_SrcBlend:				
			GL_CHECK(glGetIntegerv(GL_BLEND_SRC, &iResult));					
			break;

		case RS_DestBlend:			
			GL_CHECK(glGetIntegerv(GL_BLEND_DST, &iResult));
			break;

		default:
			SHOOT_WARNING(0, "GetRenderState could not handle state");
		}

		iValue = iResult;
	}	

	//! Sets a material color		
	void OpenGLDriver::SetColor(const Color& color)
	{
		GL_CHECK(glColor4f(color.R, color.G, color.B, color.A));
	}

	//! creates a texture from a path
	Texture* OpenGLDriver::CreateTexture(const char* strPath)
	{
		return snew OpenGLTexture(strPath);
	}

	//! create a shader
	ShaderImpl* OpenGLDriver::CreateShader()
	{
		return snew OpenGLShader();
	}

	//! creates a cubemap texture
	Texture* OpenGLDriver::CreateCubeMapTexture(const char* strPath[6])
	{
		if(OpenGLExtensionHandler::Instance()->HasExtension(OpenGLExtensionHandler::E_CubeMap))
		{
			return snew OpenGLCubeMapTexture(strPath);
		}
		else
		{
			return snew FakeCubeMapTexture(strPath);
		}
	}

	//! create a vertex buffer
	VertexBuffer* OpenGLDriver::CreateVertexBuffer()
	{
		return snew OpenGLVertexBuffer();
	}

	//! create a skybox vertex buffer
	VertexBuffer* OpenGLDriver::CreateSkyBoxVertexBufferNoCubemap(Texture* pTexture)
	{
		return snew SkyBoxVertexBufferNoCubemap(static_cast<FakeCubeMapTexture*>(pTexture));
	}

	//! clears graphic buffers
	/** \param mask: a combination of E_BufferFlag flags */
	void OpenGLDriver::ClearBuffers(u32 mask, Color clearColor /*= Color()*/)
	{
		GLbitfield glMask = 0;

		if((mask & BF_BackBuffer) != 0)
		{
			GL_CHECK(glClearColor(clearColor.R, clearColor.G, clearColor.B, clearColor.A));
			glMask |= GL_COLOR_BUFFER_BIT;
		}

		if((mask & BF_DepthBuffer) != 0)
		{			
			glMask |= GL_DEPTH_BUFFER_BIT;
		}

		if((mask & BF_StencilBuffer) != 0)
		{			
			glMask |= GL_STENCIL_BUFFER_BIT;
		}
		
		GL_CHECK(glClear(glMask));
	}

	//! sets the culling mode
	void OpenGLDriver::SetCullMode(E_CullMode eMode)
	{
		switch(eMode)
		{
		case CM_ClockWise:
			GL_CHECK(glFrontFace(GL_CCW));
			GL_CHECK(glCullFace(GL_BACK));
			glEnable(GL_CULL_FACE);
			break;

		case CM_CounterClockWise:
			GL_CHECK(glFrontFace(GL_CW));
			GL_CHECK(glCullFace(GL_BACK));
			glEnable(GL_CULL_FACE);
			break;

		case CM_None:
			glDisable(GL_CULL_FACE);
			break;

		default:
			SHOOT_WARNING(0, "SetCullMode could not handle mode");
		}
	}
}

