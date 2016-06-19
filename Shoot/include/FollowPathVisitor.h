/* 

Amine Rehioui
Created: August 7th 2011

*/

#ifndef _FOLLOW_PATH_VISITOR_H_INCLUDED_
#define _FOLLOW_PATH_VISITOR_H_INCLUDED_

#include "AnimationVisitor.h"

#include "Path.h"

namespace shoot
{
	// forwards 
	class Entity3D;	

	//! FollowPathVisitor class
	class FollowPathVisitor : public AnimationVisitor
	{
	public:

		DECLARE_OBJECT(FollowPathVisitor, AnimationVisitor);

		//! constructor
		FollowPathVisitor();

		//! Reads/Writes struct properties from/to a stream
		virtual void Serialize(PropertyStream& stream);		

		//! visits a particular entity
		virtual void Visit(Entity* pTarget);

		//! updates the visitor
		virtual void Update();

		//! overrides the path to follow
		inline void SetPath(Path* pPath) { m_Path.SetObject(pPath); }

	protected:

		Entity3D* m_p3DEntity;
		f32 m_fTimer;
		f32 m_fDuration;
		s32 m_CurrentElement;

		// properties
		Link<Path> m_Path;
		bool m_bOrientEntity;
	};
}

#endif // _FOLLOW_PATH_VISITOR_H_INCLUDED_

