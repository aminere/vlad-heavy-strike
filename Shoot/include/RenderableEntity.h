/* 

Amine Rehioui
Created: December 28th 2010

*/

#ifndef _RENDERABLE_ENTITY_H_INCLUDED_
#define _RENDERABLE_ENTITY_H_INCLUDED_

namespace shoot
{
	//! renderable entity with a material
	class RenderableEntity : public Entity
	{
	public:

		DECLARE_OBJECT(RenderableEntity, Entity);

		//! registers the entity and its children for rendering
		virtual void RegisterForRendering();

		//! returns the affine transformation matrix
		virtual Matrix44 GetTransformationMatrix()
		{
			return Matrix44::Identity;
		}

		//! returns the texture transformation matrix
		virtual Matrix44 GetTextureTransformationMatrix()
		{
			return Matrix44::Identity;
		}
	};
}

#endif // _RENDERABLE_ENTITY_H_INCLUDED_