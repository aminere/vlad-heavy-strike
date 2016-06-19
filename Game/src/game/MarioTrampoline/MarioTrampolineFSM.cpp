/* 

Amine Rehioui
Created: May 4th 2010

*/

#include "Types.h"

#include "MarioTrampolineFSM.h"

//! Initializes the state table
void MarioTrampolineFSM::Init()
{
	AddState(new MarioTrampolineStateIdle(), State_Idle);

	SwitchState(State_Idle);
}

// State_Idle functions
void MarioTrampolineStateIdle::Begin(shoot::s32 iPreviousState)
{
}

void MarioTrampolineStateIdle::Update(shoot::f32 fDeltaTime)
{
}

