/* 

Amine Rehioui
Created: August 14th 2011

*/

#ifndef _FOLLOW_CAMERA_PATH_VISITOR_H_INCLUDED_
#define _FOLLOW_CAMERA_PATH_VISITOR_H_INCLUDED_

#include "AnimationVisitor.h"

#include "CameraPath.h"

namespace shoot
{
	// forwards 
	class Camera;

	//! FollowPathVisitor class
	class FollowCameraPathVisitor : public AnimationVisitor
	{
	public:

		DECLARE_OBJECT(FollowCameraPathVisitor, AnimationVisitor);

		//! constructor
		FollowCameraPathVisitor();

		//! Reads/Writes struct properties from/to a stream
		virtual void Serialize(PropertyStream& stream);		

		//! visits a particular entity
		virtual void Visit(Entity* pTarget);

		//! updates the visitor
		virtual void Update();

	protected:

		Camera* m_pCamera;
		f32 m_fTimer;
		f32 m_fDuration;
		s32 m_CurrentElement;

		// properties
		Link<CameraPath> m_Path;
	};
}

#endif // _FOLLOW_CAMERA_PATH_VISITOR_H_INCLUDED_

