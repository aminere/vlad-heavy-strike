/* 

Amine Rehioui
Created: June 13th 2010

*/

#ifndef _GRAPHICS_DRIVER_H_INCLUDED_
#define _GRAPHICS_DRIVER_H_INCLUDED_

#include "Color.h"
#include "AABBox2D.h"
#include "AABBox3D.h"
#include "Material.h"
#include "Vertex3D.h"

namespace shoot
{
	// forwards
	class VertexBuffer;

	//! base 3D graphics driver interface	
	class GraphicsDriver
	{
	public:

		//! driver types
		enum E_DriverType
		{
			DT_OpenGL,
			DT_OpenGLES_1_1,
			DT_OpenGLES_2_0,
			DT_DirectX11,
			DT_NULL,
			DT_Count
		};

		//! Constructor
		GraphicsDriver();

		//! Destructor
		virtual ~GraphicsDriver();

		//! creates a driver - the singleton pattern was not used because GraphicsDriver has pure virtual methods
		static void CreateInstance(E_DriverType eType);

		//! destroys the driver
		static void DestroyInstance();

		//! returns the instance of the graphics driver
		inline static GraphicsDriver* Instance() { return m_spInstance; }

		//! transformation states
		enum E_TransformState
		{
			TS_Projection,			
			TS_View,
			TS_World,
			TS_Texture,
			TS_Count
		};

		//! render states
		enum E_RenderState
		{
			RS_DepthTesting,
			RS_DepthWriting,
			RS_AlphaBlending,
			RS_2DTextureMapping,
			RS_DepthFunc, // Uses E_ComparisonFunction enumeration
			RS_SrcBlend,
			RS_DestBlend,
			RS_PointSize,
			RS_PointSprite,
			RS_FillMode,
			RS_MinPointSize,
			RS_MaxPointSize,
			RS_PointDistanceAttenuation
		};

		//! buffer flags
		enum E_BufferFlag
		{
			BF_BackBuffer = 1,
			BF_DepthBuffer = 2,
			BF_StencilBuffer = 4			
		};

		//! comparison functions
		enum E_ComparisonFunction
		{			
			CF_Never,			
			CF_Less,
			CF_Equal,
			CF_LessOrEqual,
			CF_Greater,
			CF_NotEqual,
			CF_GreaterOrEqual,
			CF_Always,
			CF_Count
		};

		//! blending factor
		enum E_BlendFactor
		{
			BF_Zero,
			BF_One,			
			BF_SrcColor,
			BF_InvSrcColor,
			BF_SrcAlpha,
			BF_InvSrcAlpha,
			BF_DestAlpha,
			BF_InvDestAlpha,
			BF_DestColor,
			BF_InvDestColor,
			BF_SrcAlphaSaturate,
			BF_Count
		};

		//! primitive type
		enum E_PrimitiveType
		{
			PT_Point,
			PT_Line,
			PT_LineLoop,
			PT_LineStrip,
			PT_Triangle,
			PT_TriangleStrip,
			PT_TriangleFan
		};

		//! face culling modes
		enum E_CullMode
		{
			CM_ClockWise,
			CM_CounterClockWise,
			CM_None
		};

		//! polygon fill mode
		enum E_FillMode
		{
			FM_Point,
			FM_Line,
			FM_Solid,
			FM_Count
		};

		//! driver initialization
		virtual void Init();

		//! begins taking graphic commands
		virtual void Begin(bool bClearBackBuffer = true, bool bClearDepthBuffer = true, Color clearColor = Color::Black) = 0;

		//! create a render target texture
		virtual Texture* CreateRenderTarget(const Vector2& vSize) = 0;

		//! create a shadow map texture
		virtual Texture* CreateShadowMap(const Vector2& vSize) = 0;

		//! sets a render target
		/** \param pTarget: render target texture. Pass NULL to select the regular back buffer */
		virtual void SetRenderTarget(Texture* pTarget) = 0;

		//! draws a list of primitives from batch of vertices
		/** \param eType: primitive type 
			\param vertexFlags: a combination of E_VertexFlag 
			\param numVertices: number of vertices provided 
			\param pVertices: vertex array */
		virtual void DrawPrimitiveList(E_PrimitiveType eType, u32 vertexFlags, u32 numVertices, const Vertex3D* pVertices) = 0;

		//! draws a list of primitives from batch of vertices
		/** \param eType: primitive type 
			\param pVertexBuffer: vertex buffer to use */
		virtual void DrawPrimitiveList(E_PrimitiveType eType, VertexBuffer* pVertexBuffer) = 0;

		//! draws a list of primitives from batch of vertices and indices
		/** \param eType: primitive type 
			\param vertexFlags: a combination of E_VertexFlag 
			\param pVertices: vertex array
			\param pIndices: index array 
			\param numIndices: number of indices provided */
		virtual void DrawIndexedPrimitiveList(E_PrimitiveType eType, u32 vertexFlags, Vertex3D* pVertices, u32 numIndices, u16* pIndices) = 0;
		
		//! renders a line
		virtual void DrawLine(const Vector3& vStart, const Vector3& vEnd) = 0;

		//! renders a 2D quad
		virtual void Draw2DQuad(const Vector2& vOffset, const Vector2& vSize) = 0;

		//! renders a debug box
		virtual void DrawDebugBox(const AABBox3D& box) = 0;

		//! renders a debug box
		virtual void DrawDebugBox(const AABBox2D& box) = 0;

		//! sets the viewport area
		virtual void SetViewPort(const AABBox2D& area) = 0;

		//! resizes the screen surface
		virtual void ResizeScreen(const Size& newSize);

		//! returns the viewport area
		virtual const AABBox2D& GetViewPort() const = 0;

		//! Sets transformation matrices
		/** override with driver specific code */
		virtual void SetTransform(E_TransformState eState, const Matrix44& matrix) { m_StateMatrices[eState] = matrix; }

		//! Returns a transformation matrix		
		const Matrix44& GetTransform(E_TransformState eState) const { return m_StateMatrices[eState]; }

		//! Sets a render state
		virtual void SetRenderState(E_RenderState eState, bool bEnable) = 0;

		//! Sets a render state
		virtual void SetRenderState(E_RenderState eState, s32 iValue) = 0;

		//! Sets a render state
		virtual void SetRenderState(E_RenderState eState, f32 fValue) = 0;

		//! Sets a render state
		virtual void SetRenderState(E_RenderState eState, const f32* fValues) = 0;

		//! Sets the blend function
		virtual void SetBlendFunc(E_BlendFactor eSrc, E_BlendFactor eDest) = 0;

		//! returns a render state
		virtual void GetRenderState(E_RenderState eState, bool& bEnabled) = 0;

		//! returns a render state
		virtual void GetRenderState(E_RenderState eState, s32& iValue) = 0;

		//! Sets a material color		
		virtual void SetColor(const Color& color) = 0;
		
		//! creates a texture from a path
		virtual Texture* CreateTexture(const char* strPath) = 0;

		//! create a shader
		virtual ShaderImpl* CreateShader() = 0;

		//! creates a cubemap texture
		virtual Texture* CreateCubeMapTexture(const char* strPath[6]) = 0;

		//! create a vertex buffer
		virtual VertexBuffer* CreateVertexBuffer() = 0;

		//! create a skybox vertex buffer
		virtual VertexBuffer* CreateSkyBoxVertexBufferNoCubemap(Texture* pTexture) = 0;
				
		//! clears graphic buffers
		/** \param mask: a combination of E_BufferFlag flags */
		virtual void ClearBuffers(u32 mask, Color clearColor = Color::Black) = 0;

		//! sets the culling mode
		virtual void SetCullMode(E_CullMode eMode) = 0;

		//! returns the driver type
		inline E_DriverType GetType() const { return m_eType; }

		//! presents the graphics
		virtual void Present() { }

		//! registers a graphic object
		void RegisterObject(GraphicObject* pObject);

		//! unregister a graphic object
		void UnregisterObject(GraphicObject* pObject);

		//! unloads graphic objects
		void UnloadObjects();

		//! loads graphic objects
		void LoadObjects();

	protected:

		E_DriverType m_eType;
		Matrix44 m_StateMatrices[TS_Count];

		static GraphicsDriver* m_spInstance;

	private:

		typedef std::map<GraphicObject*, u32> GraphicObjectMap;
		GraphicObjectMap m_GraphicObjectMap;
	};

	//! null GraphicsDriver
	class NullGraphicsDriver : public GraphicsDriver
	{
	public:
		
		//! begins taking graphic commands
		void Begin(bool bClearBackBuffer = true, bool bClearDepthBuffer = true, Color clearColor = Color::Black) { }

		//! create a render target texture
		Texture* CreateRenderTarget(const Vector2& vSize) { return snew Texture(); }

		//! create a shadow map texture
		Texture* CreateShadowMap(const Vector2& vSize) { return snew Texture(); }

		//! sets a render target
		/** \param pTarget: render target texture. Pass NULL to select the regular back buffer */
		void SetRenderTarget(Texture* pTarget) { }

		//! draws a list of primitives from batch of vertices
		/** \param eType: primitive type 
			\param vertexFlags: a combination of E_VertexFlag 
			\param numVertices: number of vertices provided 
			\param pVertices: vertex array */
		void DrawPrimitiveList(E_PrimitiveType eType, u32 vertexFlags, u32 numVertices, const Vertex3D* pVertices) { }

		//! draws a list of primitives from batch of vertices
		/** \param eType: primitive type 
			\param pVertexBuffer: vertex buffer to use */
		void DrawPrimitiveList(E_PrimitiveType eType, VertexBuffer* pVertexBuffer) { }

		//! draws a list of primitives from batch of vertices and indices
		/** \param eType: primitive type 
			\param vertexFlags: a combination of E_VertexFlag 
			\param pVertices: vertex array
			\param pIndices: index array 
			\param numIndices: number of indices provided */
		void DrawIndexedPrimitiveList(E_PrimitiveType eType, u32 vertexFlags, Vertex3D* pVertices, u32 numIndices, u16* pIndices) { }

		//! renders a line
		void DrawLine(const Vector3& vStart, const Vector3& vEnd) { }

		//! renders a 2D quad
		void Draw2DQuad(const Vector2& vOffset, const Vector2& vSize) { }

		//! renders a debug box
		void DrawDebugBox(const AABBox3D& box) { }

		//! renders a debug box
		void DrawDebugBox(const AABBox2D& box) { }

		//! sets the viewport area
		void SetViewPort(const AABBox2D& area) { }

		//! resizes the screen surface
		void ResizeScreen(const Size& newSize) { }

		//! returns the viewport area
		const AABBox2D& GetViewPort() const { return m_ViewPortArea; }

		//! Sets transformation matrices
		/** override with driver specific code */
		void SetTransform(E_TransformState eState, const Matrix44& matrix) { }

		//! Sets a render state
		void SetRenderState(E_RenderState eState, bool bEnable) { }

		//! Sets a render state
		void SetRenderState(E_RenderState eState, s32 iValue) { }

		//! Sets a render state
		void SetRenderState(E_RenderState eState, f32 fValue) { }

		//! Sets a render state
		void SetRenderState(E_RenderState eState, const f32* fValues) { }

		//! Sets the blend function
		void SetBlendFunc(E_BlendFactor eSrc, E_BlendFactor eDest) { }

		//! returns a render state
		void GetRenderState(E_RenderState eState, bool& bEnabled) { }

		//! returns a render state
		void GetRenderState(E_RenderState eState, s32& iValue) { }

		//! Sets a material color		
		void SetColor(const Color& color) { }

		//! creates a texture from a path
		Texture* CreateTexture(const char* strPath) { return snew Texture(); }

		//! create a shader
		ShaderImpl* CreateShader() { return snew NULLShaderImpl(); }

		//! creates a cubemap texture
		Texture* CreateCubeMapTexture(const char* strPath[6]) { return snew Texture(); }

		//! create a vertex buffer
		VertexBuffer* CreateVertexBuffer();

		//! create a skybox vertex buffer
		VertexBuffer* CreateSkyBoxVertexBufferNoCubemap(Texture* pTexture);

		//! clears graphic buffers
		/** \param mask: a combination of E_BufferFlag flags */
		void ClearBuffers(u32 mask, Color clearColor = Color::Black) { }

		//! sets the culling mode
		void SetCullMode(E_CullMode eMode) { }

		//! returns the driver type
		E_DriverType GetType() const { return DT_NULL; }

	private:

		AABBox2D m_ViewPortArea;
	};
}

#endif // _GRAPHICS_DRIVER_H_INCLUDED_

