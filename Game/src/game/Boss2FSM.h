/* 

Amine Rehioui
Created: July 1st 2013

*/

#ifndef _BOSS2_FSM_H_INCLUDED_
#define _BOSS2_FSM_H_INCLUDED_

#include "FSM.h"
#include "Boss2.h"

namespace shoot
{
	//! Boss2FSM
	class Boss2FSM : public FSM<Boss2>
	{
		typedef FSM<Boss2> super;
	public:

		//! states
		enum E_State
		{
			State_Intro,
			State_PrepareToHook,
			State_Hook,
			State_Idle,
			State_Retreat,
			State_Exploding,
			State_Inactive,
			NumStates
		};

		//! constructor
		Boss2FSM(Boss2* pBoss2) : super(pBoss2) 
		{
		}

		//! Initializes the state table
		void Init();

		//! returns the towers left
		u32 GetTowersLeft() const;
	};

	// Boss2 States:
	//! Boss2 State_Intro
	class Boss2StateIntro : public FSMState<Boss2>
	{
	public:
		void Begin(s32 iPreviousState);
		void Update();
		void End(s32 iNextState);
	};

	//! Boss2 State_PrepareToHook
	class Boss2StatePrepareToHook : public FSMState<Boss2>
	{
	public:
		void Begin(s32 iPreviousState);
		void Update();
		void End(s32 iNextState);

	private:

		f32 m_fTimer;
		bool m_bWarning;
	};

	//! Boss2 State_Hook
	class Boss2StateHook : public FSMState<Boss2>
	{
	public:
		void Begin(s32 iPreviousState);
		void Update();
		void End(s32 iNextState);

	private:

		//! aim
		void Aim();

		//! state
		enum E_State
		{
			S_Orient1,
			S_Hook,
			S_Retreat,
			S_Orient2,
			S_Done
		};

		E_State m_State;
		f32 m_fTimer;
		f32 m_fInterpolator;
		f32 m_fSrcRotation;
		f32 m_fDestRotation;
		Handle<Actor> m_Hook;
		Handle<Entity3D> m_Rope;
	};

	//! Boss2 State_Idle
	class Boss2StateIdle : public FSMState<Boss2>
	{
	public:
		void Begin(s32 iPreviousState);
		void Update();
		void End(s32 iNextState);
	};

	//! Boss2 State_Retreat
	class Boss2StateRetreat : public FSMState<Boss2>
	{
	public:
		void Begin(s32 iPreviousState);
		void Update();
		void End(s32 iNextState);
	};

	//! Boss2 State_Exploding
	class Boss2StateExploding : public FSMState<Boss2>
	{
	public:
		void Begin(s32 iPreviousState);
		void Update();
		void End(s32 iNextState);

	private:
		f32 m_fTimer;
	};

	//! Boss2 State_Inactive
	class Boss2StateInactive : public FSMState<Boss2>
	{
	public:
		void Begin(s32 iPreviousState) { }
		void Update() { }
		void End(s32 iNextState) { }
	};
}

#endif // _BOSS2_FSM_H_INCLUDED_

