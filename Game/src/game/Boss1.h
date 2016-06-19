/* 

Amine Rehioui
Created: May 9th 2013

*/

#ifndef _BOSS1_H_INCLUDED_
#define _BOSS1_H_INCLUDED_

#include "Event.h"

namespace shoot
{
	//! Boss1 class
	class Boss1 : public Actor
	{
		friend class FSMState<Boss1>;

	public:
		
		DECLARE_OBJECT(Boss1, Actor);

		//! constructor
		Boss1();

		//! destructor
		virtual ~Boss1();

		//! serializes the entity to/from a PropertyStream
		void Serialize(PropertyStream& stream);

		//! called during the initialization of the entity
		void Init();

		//! called during the update of the entity		
		void Update();

		//! shoot event listeners
		static void HandleTriggerEvent(void* pListener, Event* pEvent);

	// public for FSM

		static u32 m_sNumCannons;

		//! cannon info
		struct CannonInfo
		{
			CannonInfo(Actor* pEntity)
				: m_Entity(pEntity)
				, vOriginalPos(pEntity->GetPosition())
			{
			}
			Handle<Actor> m_Entity;
			Vector3 vOriginalPos;
		};

		std::vector<CannonInfo> m_aCannons;
		u32 m_CurrentSide;		

		// properties
		f32 m_fCannonOffset;
		f32 m_fPullbackDuration;
		f32 m_fCannonFrequency;
		f32 m_fCannonActiveTime;
		f32 m_fCannonInactiveTime;
		f32 m_fDualShootDuration;
		f32 m_fSideSwitchDuration;
		f32 m_fBulletOffset;
		f32 m_fBulletSize;
		f32 m_fBulletSpeed;
		f32 m_fBulletLife;
		f32 m_fAimDuration;
		Reference<Component> m_SpinAttackLeft;
		Reference<Component> m_SpinAttackRight;		
		Reference<Component> m_Retreat;
		bool m_bDestroyWhenDone;
	};
}

#endif // _BOSS1_H_INCLUDED_


