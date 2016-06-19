/* 

Amine Rehioui
Created: May 4th 2010

*/

#include "Types.h"

#include "MarioFSM.h"

//! Initializes the state table
void MarioFSM::Init()
{
	AddState(new MarioStateIdle(), State_Idle);
	AddState(new MarioStateRunning(), State_Running);
	AddState(new MarioStatePrepareJump(), State_PrepareJump);
	AddState(new MarioStateFlying(), State_Flying);

	SwitchState(State_Idle);
}

// Mario State_Idle functions
void MarioStateIdle::Begin(shoot::s32 iPreviousState)
{
}

void MarioStateIdle::Update(shoot::f32 fDeltaTime)
{
}

// Mario State_Running functions
void MarioStateRunning::Begin(shoot::s32 iPreviousState)
{
}

void MarioStateRunning::Update(shoot::f32 fDeltaTime)
{
}

// Mario State_PrepareJump functions
void MarioStatePrepareJump::Begin(shoot::s32 iPreviousState)
{
}

void MarioStatePrepareJump::Update(shoot::f32 fDeltaTime)
{
}

// Mario State_Flying functions
void MarioStateFlying::Begin(shoot::s32 iPreviousState)
{
}

void MarioStateFlying::Update(shoot::f32 fDeltaTime)
{
}
