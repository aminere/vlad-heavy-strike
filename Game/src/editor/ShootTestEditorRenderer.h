/* 

Amine Rehioui
Created: October 15th 2011

*/

#ifndef _SHOOT_TEST_EDITOR_RENDERER_H_INCLUDED_
#define _SHOOT_TEST_EDITOR_RENDERER_H_INCLUDED_

#include "EditorRenderer.h"

namespace shoot
{
	// forwards
	class BulletManager;
	class AreaSpawner;
	class TriggerSpawner;

	//! Shoot Test Editor renderer
	class ShootTestEditorRenderer : public EditorRenderer
	{
		typedef EditorRenderer super;

	public:

		//! destructor
		virtual ~ShootTestEditorRenderer();

		//! Init
		void Init();

	protected:

		//! registers an entity for post rendering
		void RegisterEntity(Entity* pEntity);

		//! custom Post 3D rendering
		void RenderPost3D();

		//! custom Post 2D rendering
		void RenderPost2D();

		std::vector<BulletManager*> m_aBulletManagers;
		std::vector<AreaSpawner*> m_aAreaSpawners;
		std::vector<TriggerSpawner*> m_aTriggerSpawners;

		Reference<VertexBuffer> m_UnitCircle;
	};
}

#endif // _SHOOT_TEST_EDITOR_RENDERER_H_INCLUDED_

