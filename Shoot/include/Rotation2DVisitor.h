/* 

Amine Rehioui
Created: April 7th 2011

*/

#ifndef _ROTATION_2D_VISITOR_H_INCLUDED_
#define _ROTATION_2D_VISITOR_H_INCLUDED_

#include "AnimationVisitor.h"

#include "ExtendedProperties.h"

namespace shoot
{
	// forwards 
	class Entity2D;

	//! Rotation2DVisitor class
	class Rotation2DVisitor : public AnimationVisitor
	{
	public:

		DECLARE_OBJECT(Rotation2DVisitor, AnimationVisitor);

		//! constructor
		Rotation2DVisitor();

		//! destructor
		virtual ~Rotation2DVisitor();

		//! Reads/Writes struct properties from/to a stream
		virtual void Serialize(PropertyStream& stream);		

		//! visits a particular entity
		virtual void Visit(Entity* pTarget);

		//! updates the visitor
		virtual void Update();

	protected:

		Entity2D* m_p2DEntity;

		f32 m_fCurrentTime;
		f32 m_fSrcAngle;
		f32 m_fDestAngle;

		// properties
		ESP<f32, PT_Float> m_Src;
		ESP<f32, PT_Float> m_Dest;
		f32 m_fDuration;
	};
}

#endif // _ROTATION_2D_VISITOR_H_INCLUDED_

