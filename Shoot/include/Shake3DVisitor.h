/* 

Amine Rehioui
Created: May 23rd 2013

*/

#ifndef _SHAKE_3D_VISITOR_H_INCLUDED_
#define _SHAKE_3D_VISITOR_H_INCLUDED_

#include "AnimationVisitor.h"

namespace shoot
{
	// forwards 
	class Entity3D;

	//! Shake3DVisitor class
	class Shake3DVisitor : public AnimationVisitor
	{
	public:

		DECLARE_OBJECT(Shake3DVisitor, AnimationVisitor);

		//! constructor
		Shake3DVisitor();

		//! destructor
		virtual ~Shake3DVisitor();

		//! Reads/Writes struct properties from/to a stream
		virtual void Serialize(PropertyStream& stream);		

		//! visits a particular entity
		virtual void Visit(Entity* pTarget);

		//! updates the visitor		
		virtual void Update();

	protected:

		Entity3D* m_p3DEntity;
		f32 m_fCurrentTime;

		// properties
		f32 m_fDuration;
		Vector3 m_vAmplitude;
		Vector3 m_vFrequency;
	};
}

#endif // _SHAKE_3D_VISITOR_H_INCLUDED_


