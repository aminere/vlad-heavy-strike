/* 

Amine Rehioui
Created: April 9th 2011

*/

#ifndef _SCALE_2D_VISITOR_H_INCLUDED_
#define _SCALE_2D_VISITOR_H_INCLUDED_

#include "AnimationVisitor.h"

#include "ExtendedProperties.h"

namespace shoot
{
	// forwards 
	class Entity2D;

	//! Rotation2DVisitor class
	class Scale2DVisitor : public AnimationVisitor
	{
	public:

		DECLARE_OBJECT(Scale2DVisitor, AnimationVisitor);

		//! constructor
		Scale2DVisitor();

		//! destructor
		virtual ~Scale2DVisitor();

		//! Reads/Writes struct properties from/to a stream
		virtual void Serialize(PropertyStream& stream);		

		//! visits a particular entity
		virtual void Visit(Entity* pTarget);

		//! updates the visitor
		virtual void Update();

	protected:

		Entity2D* m_p2DEntity;

		f32 m_fCurrentTime;	
		Vector2 m_vSrcScale;
		Vector2 m_vDestScale;

		// properties
		ESP<Vector2, PT_Vec2> m_Src;
		ESP<Vector2, PT_Vec2> m_Dest;
		f32 m_fDuration;		
	};
}

#endif // _SCALE_2D_VISITOR_H_INCLUDED_


