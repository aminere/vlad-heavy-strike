/* 

Amine Rehioui
Created: July 1st 2013

*/

#ifndef _BOSS2_H_INCLUDED_
#define _BOSS2_H_INCLUDED_

#include "Event.h"

namespace shoot
{
	//! Boss2 class
	class Boss2 : public Actor
	{
		friend class FSMState<Boss2>;

	public:
		
		DECLARE_OBJECT(Boss2, Actor);

		//! constructor
		Boss2();

		//! serializes the entity to/from a PropertyStream
		void Serialize(PropertyStream& stream);

		//! called during the initialization of the entity
		void Init();

		//! called during the update of the entity		
		void Update();

	// public for FSM

		Handle<Actor> m_LeftHook;
		Handle<Actor> m_RightHook;
		std::vector< Handle<Entity> > m_aTowers;
		u32 m_CurrentHook;

		// properties
		f32 m_fPrepareHookDuration;
		f32 m_fWarningDuration;
		f32 m_fSpinSpeedNormal;
		f32 m_fSpinSpeedFast;
		f32 m_fHookOrientDuration;
		f32 m_fHookSpeed;
		f32 m_fHookSpeedRetreat;
		f32 m_fHookRange;
		f32 m_fHookMaxRotation;		
		bool m_bDestroyWhenDone;
		Reference<Component> m_Retreat;
	};
}

#endif // _BOSS2_H_INCLUDED_


