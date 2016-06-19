/* 

Amine Rehioui
Created: May 8th 2010

*/

#ifndef _ENTITY_RENDERER_H_INCLUDED_
#define _ENTITY_RENDERER_H_INCLUDED_

#include "RenderableEntity.h"

#include "Camera.h"

namespace shoot
{	
	// forwards
	class VertexBuffer;
	class ScreenAlignedQuad;
	class Material;

	//! EntityRenderer class	
	class EntityRenderer : public Singleton<EntityRenderer>
	{
		DECLARE_SINGLETON(EntityRenderer);

	public:	

		//! rendering stats
		struct Stats
		{
			Stats()
				: NumDrawCalls(0)
				, NumMaterials(0)
				, NumVertices(0)
				, NumTriangles(0)
				, Enabled(true)
			{
			}

			u32 NumDrawCalls;
			u32 NumMaterials;
			u32 NumVertices;
			u32 NumTriangles;
			bool Enabled;

			//! increment from a vertex buffer
			void Increment(VertexBuffer* pVertexBuffer, u32 numInstances = 1);

			//! increment from a material
			void Increment(Material* pMaterial);
		};

		//! Initializes the renderer
		void Init(const Size& screenSize);

		//! registers the entity for rendering
		void RegisterEntity(RenderableEntity* pEntity);

		//! registers the entity for rendering
		void RegisterDebugEntity(RenderableEntity* pEntity);

		//! clears the rendering lists
		void Clear();

		//! renders the entities
		void Render();		

		//! change the current 3D camera
		void Set3DCamera(Camera* pCamera);

		//! changes the current 2D camera
		void Set2DCamera(Camera* pCamera);
	
		//! returns the current 3D camera
		Camera* Get3DCamera() { return m_3DCamera; }

		//! returns the current 2D camera
		Camera* Get2DCamera()	{ return m_2DCamera; }

		//! returns rendering stats
		Stats& GetStats() { return m_Stats; }

		//! sets view field handling status
		inline void SetRenderViewField(bool bRender) { m_bRenderViewField = bRender; }

		//! gets view field handling status
		inline bool GetRenderViewField() const { return m_bRenderViewField; }

	private:

		Handle<Camera> m_3DCamera;
		Handle<Camera> m_2DCamera;

		//! vertex info
		struct VertexInfo
		{
			VertexBuffer* pVertexBuffer;
			std::vector<Matrix44> aWorldTransforms;
			std::vector<Matrix44> aTextureTransforms;
		};

		typedef std::map< u32, VertexInfo > VertexMap;

		//! render info
		struct RenderInfo
		{
			Material* pMaterial;
			VertexMap m_VertexMap;
		};
		
		typedef std::map< u32, RenderInfo > RenderMap;

		RenderMap m_Solid3DRenderMap;
		RenderMap m_Transparent3DRenderMap;
		RenderMap m_Solid2DRenderMap;
		RenderMap m_Transparent2DRenderMap;
		RenderMap m_Solid2DPostFadeRenderMap;
		RenderMap m_SkyBoxMap;
		RenderMap m_Debug2DMap;

		Stats m_Stats;
		bool m_bRenderViewField;

		ScreenAlignedQuad* m_pScreenAlignedQuad;
		Reference<Texture> m_RenderTarget;
		Reference<Texture> m_ShadowMap;

		//! adds a graphic to a render map
		void AddToRenderMap(RenderMap& renderMap, GraphicComponent* pGraphic);

		//! renders from a render map
		void Render(RenderMap& renderMap);
	};
}

#endif // _ENTITY_RENDERER_H_INCLUDED_


