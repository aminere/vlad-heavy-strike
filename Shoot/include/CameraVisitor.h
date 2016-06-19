/* 

Amine Rehioui
Created: May 31st 2013

*/

#ifndef _CAMERA_VISITOR_H_INCLUDED_
#define _CAMERA_VISITOR_H_INCLUDED_

#include "Visitor.h"

namespace shoot
{
	// forwards 
	class Camera;
	class Entity3D;

	//! CameraVisitor class
	class CameraVisitor : public Visitor
	{
	public:

		DECLARE_OBJECT(CameraVisitor, Visitor);

		//! constructor
		CameraVisitor();

		//! destructor
		virtual ~CameraVisitor();

		//! Reads/Writes struct properties from/to a stream
		virtual void Serialize(PropertyStream& stream);		

		//! visits a particular entity
		virtual void Visit(Entity* pTarget);

		//! updates the visitor		
		virtual void Update();

	protected:

		Camera* m_pCamera;
		f32 m_fCurrentTime;
		bool m_bFollowStarted;
		Vector3 m_vFollowDirection;

		// properties
		Link<Entity3D> m_LookAtTarget;
		f32 m_fLookAtDuration;
		f32 m_fDuration;
		f32 m_fMaxDistToTarget;
	};
}

#endif // _CAMERA_VISITOR_H_INCLUDED_


