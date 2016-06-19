/* 

Amine Rehioui
Created: July 27th 2013

*/

#ifndef _DIRECTX11_DRIVER_H_INCLUDED_
#define _DIRECTX11_DRIVER_H_INCLUDED_

namespace shoot
{
	//! OpenGL 3D driver
	class DirectX11Driver : public GraphicsDriver
	{
		typedef GraphicsDriver super;

	public:

		//! constructor
		DirectX11Driver();

		//! destructor
		virtual ~DirectX11Driver();

		//! returns the instance of the graphics driver
		inline static DirectX11Driver* Instance() { return static_cast<DirectX11Driver*>(m_spInstance); }

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
		void DrawLine(const Vector3& vStart, const Vector3& vEnd);

		//! renders a 2D quad
		void Draw2DQuad(const Vector2& vOffset, const Vector2& vSize);

		//! renders a debug box
		void DrawDebugBox(const AABBox3D& box);

		//! renders a debug box
		void DrawDebugBox(const AABBox2D& box);

		//! sets the viewport area
		void SetViewPort(const AABBox2D& area);

		//! resizes the screen surface
		void ResizeScreen(const Size& newSize);

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

		//! draws using a vertex buffer
		void Draw(u32 numVertices, u32 startVertex);

		//! draws using an index buffer
		void DrawIndexed(u32 numIndices, u32 startIndex, u32 startVertex);

		//! clears graphic buffers
		/** \param mask: a combination of E_BufferFlag flags */
		void ClearBuffers(u32 mask, Color clearColor = Color::Black);

		//! sets the culling mode
		void SetCullMode(E_CullMode eMode);

		//! presents the graphics
		void Present();

		//! sets the window handle
		inline void SetWindow(HWND hWindow) { m_hWindow = hWindow; }

		//! returns the device
		inline ID3D11Device1* GetDevice() const { return m_Device; }

		//! returns the immediate context
		inline ID3D11DeviceContext1* GetContext() const { return m_ImmediateContext; }

		//! binds material states
		void BindMaterialStates();

		//! registers a COM reference
		void RegisterReference(IUnknown* pObject);

		//! unregisters a COM reference
		void UnregisterReference(IUnknown* pObject);

	private:
		
		std::map<IUnknown*, s32> m_COMReferences;

		AABBox2D m_ViewPortArea;
		Matrix44 m_ProjectionViewMatrix;
		HWND m_hWindow;

		COMReference<IDXGISwapChain1> m_SwapChain;
		COMReference<ID3D11RenderTargetView> m_RenderTargetView;
		COMReference<ID3D11DepthStencilView> m_DepthStencilView;
		COMReference<ID3D11Buffer> m_MVPBuffer;
		COMReference<ID3D11Buffer> m_TextureTransformBuffer;
		COMReference<ID3D11Buffer> m_MaterialBuffer;
		COMReference<ID3D11SamplerState> m_TextureSamplerLinear;
		COMReference<ID3D11BlendState> m_BlendState[2][D3D11_BLEND_INV_SRC1_ALPHA+1][D3D11_BLEND_INV_SRC1_ALPHA+1]; // blendEnable / srcBlend / destBlend
		COMReference<ID3D11DepthStencilState> m_DepthStencilState[2][2][D3D11_COMPARISON_ALWAYS+1]; // testEnable / writeEnable / depthFunc
		COMReference<ID3D11RasterizerState> m_RasterizerState[2][D3D11_FILL_SOLID+1][D3D11_CULL_BACK+1]; // frontCCW / fillMode / cullMode
		COMReference<ID3D11DeviceContext1> m_ImmediateContext;
		COMReference<ID3D11Device1> m_Device;
		D3D11_BLEND_DESC m_BlendDesc;
		D3D11_DEPTH_STENCIL_DESC m_DSDesc;
		D3D11_RASTERIZER_DESC  m_RSDesc;
		D3D_FEATURE_LEVEL m_FeatureLevel;
	};
}

#endif // _DIRECTX11_DRIVER_H_INCLUDED_

