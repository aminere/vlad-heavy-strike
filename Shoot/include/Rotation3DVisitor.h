/* 

Amine Rehioui
Created: April 7th 2011

*/

#ifndef _ROTATION_3D_VISITOR_H_INCLUDED_
#define _ROTATION_3D_VISITOR_H_INCLUDED_

#include "AnimationVisitor.h"

#include "ExtendedProperties.h"

namespace shoot
{
	// forwards 
	class Entity3D;

	//! Rotation3DVisitor class
	class Rotation3DVisitor : public AnimationVisitor
	{
	public:

		DECLARE_OBJECT(Rotation3DVisitor, AnimationVisitor);

		//! constructor
		Rotation3DVisitor();

		//! destructor
		virtual ~Rotation3DVisitor();

		//! Reads/Writes struct properties from/to a stream
		virtual void Serialize(PropertyStream& stream);		

		//! visits a particular entity
		virtual void Visit(Entity* pTarget);

		//! updates the visitor		
		virtual void Update();

	protected:

		Entity3D* m_p3DEntity;

		f32 m_fCurrentTime;
		Vector3 m_vSrcRotation;
		Vector3 m_vDestRotation;

		// properties
		ESP<Vector3, PT_Vec3> m_Src;
		ESP<Vector3, PT_Vec3> m_Dest;
		f32 m_fDuration;
	};
}

#endif // _ROTATION_3D_VISITOR_H_INCLUDED_

