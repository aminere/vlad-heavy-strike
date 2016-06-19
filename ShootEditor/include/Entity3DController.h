/* 

Amine Rehioui
Created: May 14th 2010

*/

#ifndef _ENTITY_3D_CONTROLLER_H_INCLUDED_
#define _ENTITY_3D_CONTROLLER_H_INCLUDED_

namespace shoot
{
	//! Helper to control 3D entities in the editor
	class Entity3DController : public Singleton<Entity3DController>
	{
		DECLARE_SINGLETON(Entity3DController);

	public:
		
		//! Renders the controller elements
		void Render();
	
		//! Sets the 3D entity to control
		void SetEntity(Entity3D* pEntity);

		//! returns true if a 3D entity is currently targeted by this controller
		bool HasEntity() const { return m_Entity.IsValid(); }

		//! is called when the 3D picking ray has changed
		void OnPickingRayMoved(const Vector3& vRayStart, const Vector3& vRayEnd);

		//! called when mouse is dragged
		void OnMouseDragged(const Vector3& vRayStart, const Vector3& vRayDir);

		//! returns true is one of the axes is being highlighted by the mouse
		bool IsAxeHighlighted() const { return m_SelectedAxes != Axe_None; }

	private:

		Handle<Entity3D> m_Entity;
		Matrix44 m_EntityCenterTransformation;

		Vector3 m_vIntersection;

		// settings
		Color m_AxeHighlightColor;
		f32 m_AxeSize;		
		f32 m_fTranslationStep;
		f32 m_fRotationStep;
		f32 m_fScaleStep;

		f32 m_fProportionalAxeSize;

		//! transformation axes
		enum E_Axes
		{			
			Axe_X,
			Axe_Y,
			Axe_Z,
			Axe_XY,
			Axe_XZ,
			Axe_ZY,
			Axe_None
		};

		E_Axes m_SelectedAxes;
	};
}

#endif // _ENTITY_3D_CONTROLLER_H_INCLUDED_


