/* 

Amine Rehioui
Created: July 1st 2013

*/

#include "ShootTest.h"

#include "Boss2.h"

#include "Boss2FSM.h"

namespace shoot
{
	DEFINE_OBJECT(Boss2);

	//! constructor
	Boss2::Boss2()
		: m_CurrentHook(0)
		// properties
		, m_fPrepareHookDuration(4.0f)
		, m_fWarningDuration(.5f)
		, m_fSpinSpeedNormal(360.0f)
		, m_fSpinSpeedFast(720.0f)
		, m_fHookOrientDuration(.3f)
		, m_fHookSpeed(45.0f)
		, m_fHookSpeedRetreat(25.0f)
		, m_fHookRange(25.0f)
		, m_fHookMaxRotation(27.0f)
		, m_bDestroyWhenDone(false)
	{
	}

	//! serializes the entity to/from a PropertyStream
	void Boss2::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);
		
		stream.Serialize(PT_Float, "PrepareHookDuration", &m_fPrepareHookDuration);
		stream.Serialize(PT_Float, "WarningDuration", &m_fWarningDuration);
		stream.Serialize(PT_Float, "SpinSpeedNormal", &m_fSpinSpeedNormal);
		stream.Serialize(PT_Float, "SpinSpeedFast", &m_fSpinSpeedFast);
		stream.Serialize(PT_Float, "HookOrientDuration", &m_fHookOrientDuration);
		stream.Serialize(PT_Float, "HookSpeed", &m_fHookSpeed);
		stream.Serialize(PT_Float, "HookSpeedRetreat", &m_fHookSpeedRetreat);
		stream.Serialize(PT_Float, "HookRange", &m_fHookRange);
		stream.Serialize(PT_Float, "HookMaxRotation", &m_fHookMaxRotation);		
		stream.Serialize(PT_Bool, "DestroyWhenDone", &m_bDestroyWhenDone);		
		stream.SerializeReference("Retreat", &m_Retreat);
	}

	//! called during the initialization of the entity
	void Boss2::Init()
	{
		m_pFSM = snew Boss2FSM(this);
		super::Init();

		m_LeftHook = DYNAMIC_CAST(GetChildByName("LeftHook"), Actor);
		m_RightHook = DYNAMIC_CAST(GetChildByName("RightHook"), Actor);
	}

	//! called during the update of the entity	
	void Boss2::Update()
	{
		m_pFSM->Update();
	}
}

