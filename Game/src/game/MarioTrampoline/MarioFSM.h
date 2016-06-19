/* 

Amine Rehioui
Created: May 4th 2010

*/

#ifndef _MARIO_FSM_H_INCLUDED_
#define _MARIO_FSM_H_INCLUDED_

#include "FSM.h"
#include "Mario.h"

//! Mario State Machine class
class MarioFSM : public FSM<Mario>
{
	typedef FSM<Mario> super;
public:

	//! states
	enum E_State
	{
		State_Idle = 0,
		State_Running,
		State_PrepareJump,
		State_Flying,
		NumStates
	};

	//! constructor
	MarioFSM(Mario* pMario) : super(pMario) 
	{
	}

	//! Initializes the state table
	void Init();
};

// Mario States:
//! Mario State_Idle
class MarioStateIdle : public FSMState<Mario>
{
public:
	void Begin(shoot::s32 iPreviousState);
	void Update(shoot::f32 fDeltaTime);
};

//! Mario State_Running
class MarioStateRunning : public FSMState<Mario>
{
public:
	void Begin(shoot::s32 iPreviousState);
	void Update(shoot::f32 fDeltaTime);
};

//! Mario State_PrepareJump
class MarioStatePrepareJump : public FSMState<Mario>
{
public:
	void Begin(shoot::s32 iPreviousState);
	void Update(shoot::f32 fDeltaTime);
};

//! Mario State_Flying
class MarioStateFlying : public FSMState<Mario>
{
public:
	void Begin(shoot::s32 iPreviousState);
	void Update(shoot::f32 fDeltaTime);
};

#endif // _MARIO_FSM_H_INCLUDED_

