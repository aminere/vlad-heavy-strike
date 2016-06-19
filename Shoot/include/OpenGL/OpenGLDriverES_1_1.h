/* 

Amine Rehioui
Created: June 13th 2010

*/

#ifndef _OPENGL_DRIVER_ES_1_1_H_INCLUDED_
#define _OPENGL_DRIVER_ES_1_1_H_INCLUDED_

namespace shoot
{
	//! OpenGL 3D driver
	class OpenGLDriverES_1_1 : public GraphicsDriver
	{
		typedef GraphicsDriver super;

	public:

		//! destructor
		virtual ~OpenGLDriverES_1_1();

		//! driver initialization
		void Init();
		
		//! begins taking graphic commands
		void Begin(bool bClearBackBuffer = true, bool bClearDepthBuffer = true, Color clearColor = Color::Black);
		
		//! create a render target texture
		Texture* CreateRenderTarget(const Vector2& vSize);

		//! create a shadow map texture
		Texture* CreateShadowMap(const Vector2& vSize);

		//! sets a render target
		/** \param pTarget: render target texture. Pass NULL to select the regular back buffer */
		void SetRenderTarget(Texture* pTarget);

		//! draws a list of primitives from batch of vertices
		/** \param eType: primitive type 
			\param vertexFlags: a combination of E_VertexFlag 
			\param numVertices: number of vertices provided 
			\param pVertices: vertex array */
		void DrawPrimitiveList(E_PrimitiveType eType, u32 vertexFlags, u32 numVertices, const Vertex3D* pVertices);

		//! draws a list of primitives from batch of vertices
		/** \param eType: primitive type 
			\param pVertexBuffer: vertex buffer to use */
		void DrawPrimitiveList(E_PrimitiveType eType, VertexBuffer* pVertexBuffer);

		//! draws a list of primitives from batch of vertices and indices
		/** \param eType: primitive type 
			\param vertexFlags: a combination of E_VertexFlag 
			\param pVertices: vertex array
			\param pIndices: index array 
			\param numIndices: number of indices provided */
		void DrawIndexedPrimitiveList(E_PrimitiveType eType, u32 vertexFlags, Vertex3D* pVertices, u32 numIndices, u16* pIndices);

		//! renders a line		
		void DrawLine(const Vector3& vStart, const Vector3& vEnd) { }

		//! renders a 2D quad
		void Draw2DQuad(const Vector2& vOffset, const Vector2& vSize) { }

		//! renders a debug box		
		void DrawDebugBox(const AABBox3D& box) { }

		//! renders a debug box
		void DrawDebugBox(const AABBox2D& box) { }

		//! sets the viewport area
		void SetViewPort(const AABBox2D& area);
		
		//! returns the viewport area
		const AABBox2D& GetViewPort() const { return m_ViewPortArea; }

		//! Sets transformation matrices
		/** override with driver specific code */
		void SetTransform(E_TransformState eState, const Matrix44& matrix);

		//! Sets a render state
		void SetRenderState(E_RenderState eState, bool bEnable);

		//! Sets a render state
		void SetRenderState(E_RenderState eState, s32 iValue);

		//! Sets a render state
		void SetRenderState(E_RenderState eState, f32 fValue);

		//! Sets a render state
		void SetRenderState(E_RenderState eState, const f32* fValues);

		//! Sets the blend function
		void SetBlendFunc(E_BlendFactor eSrc, E_BlendFactor eDest);

		//! returns a render state
		void GetRenderState(E_RenderState eState, bool& bEnabled);

		//! returns a render state
		void GetRenderState(E_RenderState eState, s32& iValue);

		//! Sets a material color		
		void SetColor(const Color& color);

		//! creates a texture from a path
		Texture* CreateTexture(const char* strPath);

		//! create a shader
		ShaderImpl* CreateShader();

		//! creates a cubemap texture
		Texture* CreateCubeMapTexture(const char* strPath[6]);

		//! create a vertex buffer
		VertexBuffer* CreateVertexBuffer();

		//! create a skybox vertex buffer
		VertexBuffer* CreateSkyBoxVertexBufferNoCubemap(Texture* pTexture);

		//! clears graphic buffers
		/** \param mask: a combination of E_BufferFlag flags */
		void ClearBuffers(u32 mask, Color clearColor = Color::Black);
		
		//! sets the culling mode
		void SetCullMode(E_CullMode eMode);
		
	protected:
		
		AABBox2D m_ViewPortArea;		
	};
}

#endif // _OPENGL_DRIVER_ES_1_1_H_INCLUDED_

