/* 

Amine Rehioui
Created: May 4th 2010

*/

#ifndef _MARIO_TRAMPOLINE_FSM_H_INCLUDED_
#define _MARIO_TRAMPOLINE_FSM_H_INCLUDED_

#include "FSM.h"
#include "MarioTrampolineManager.h"

//! Mario State Machine class
class MarioTrampolineFSM : public FSM<MarioTrampolineManager>
{
	typedef FSM<MarioTrampolineManager> super;
public:

	//! states
	enum E_State
	{
		State_Idle = 0,
		NumStates
	};

	//! constructor
	MarioTrampolineFSM(MarioTrampolineManager* pActor) : super(pActor) 
	{
	}

	//! Initializes the state table
	void Init();
};

// MarioTrampolineFSM States:
//! State_Idle
class MarioTrampolineStateIdle : public FSMState<MarioTrampolineManager>
{
public:
	void Begin(shoot::s32 iPreviousState);
	void Update(shoot::f32 fDeltaTime);
};

#endif // _MARIO_TRAMPOLINE_FSM_H_INCLUDED_

