/* 

Amine Rehioui
Created: August 20th 2011

*/

#ifndef _EDITOR_RENDERER_H_INCLUDED_
#define _EDITOR_RENDERER_H_INCLUDED_

#include "MeshResource.h"

#include "BillBoardEntity.h"

#include "QuadTree.h"

namespace shoot
{
	// forwards
	class Entity;
	class Path;
	class CollisionComponent;

	//! Editor renderer
	class EditorRenderer : public Singleton<EditorRenderer>
	{
		DECLARE_SINGLETON(EditorRenderer);

	public:

		//! Init
		virtual void Init();

		//! performs editor rendering
		void Render();

		//! sets enabled status
		void SetEnabled(bool bEnabled) { m_bEnabled = bEnabled; }

		//! set picking info
		void SetPickingInfo(const Vector3& vRayStart, const Vector3& vRayDir, const Vector3& vIntersection);

		//! set picking status
		inline void SetPickingInfoValid(bool bValid) { m_bPickingInfoValid = bValid; }

	protected:

		//! extracts entities for rendering
		void ExtractEntities(Entity* pEntity);

		//! registers an entity for post rendering
		virtual void RegisterEntity(Entity* pEntity);

		//! custom Post 3D rendering
		virtual void RenderPost3D();

		//! custom Post 2D rendering
		virtual void RenderPost2D();

		//! Renders collision shapes
		void RenderCollisionShapes(CollisionComponent* pCollisionComponent);

		//! Render path
		void RenderPost3DPath(Path* pPath);

		//! Render quad tree nodes
		void RenderQuadTreeNodes(QuadTreeNode* pNode);

		//! Renders visible quad tree nodes
		void RenderVisibleQuadTreeNodes(QuadTreeNode* pNode);

		//! Renders a quad tree node
		void RenderQuadTreeNode(QuadTreeNode* pNode, const Color& color);

		//! Renders a camera frustum
		void RenderCameraFrustum(Camera* pCamera);

		// lists for post rendering passes
		std::vector<Entity*> m_2DEntities;
		std::vector<Entity*> m_3DEntities;
		std::vector<Path*> m_aPaths;

		// debug icons
		typedef std::map<u32, Reference<Texture> > DebugIconMap;
		DebugIconMap m_DebugIconMap;
		Reference<BillBoardEntity> m_DebugIconEntity;

		Reference<MeshResource> m_UnitSphere;
		Reference<VertexBuffer> m_GridVertexBuffer;
		Reference<Texture> m_PinTexture;

		bool m_bEnabled;
		bool m_bPickingInfoValid;
		Vector3 m_vPickStart, m_vPickEnd, m_vPickIntersection;
	};
}

#endif // _EDITOR_RENDERER_H_INCLUDED_

