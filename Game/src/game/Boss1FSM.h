/* 

Amine Rehioui
Created: May 10th 2013

*/

#ifndef _BOSS1_FSM_H_INCLUDED_
#define _BOSS1_FSM_H_INCLUDED_

#include "FSM.h"
#include "Boss1.h"

namespace shoot
{
	//! Boss1FSM
	class Boss1FSM : public FSM<Boss1>
	{
		typedef FSM<Boss1> super;
	public:

		//! states
		enum E_State
		{
			State_Intro,
			State_DualCannons,
			State_SpinAttack,
			State_PostSpinSetup,
			State_Retreat,
			State_Exploding,
			State_Inactive,
			NumStates
		};

		//! constructor
		Boss1FSM(Boss1* pBoss1) : super(pBoss1) 
		{
		}

		//! Initializes the state table
		void Init();
	};

	// Boss1 States:
	//! Boss1 State_Intro
	class Boss1StateIntro : public FSMState<Boss1>
	{
	public:
		void Begin(s32 iPreviousState);
		void Update();
		void End(s32 iNextState);
	};

	//! Boss1 State_DualCannons
	class Boss1StateDualCannons : public FSMState<Boss1>
	{
	public:
		void Begin(s32 iPreviousState);
		void Update();
		void End(s32 iNextState);
	private:
		u32 m_CurrentCannon;
		f32 m_fGlobalTimer;
		f32 m_fActiveTimer;
		f32 m_fInactiveTimer;
		f32 m_fShootTimer;
		bool m_bActive;
	};

	//! Boss1 State_SpinAttack
	class Boss1StateSpinAttack : public FSMState<Boss1>
	{
	public:
		void Begin(s32 iPreviousState);
		void Update();
		void End(s32 iNextState);
	};

	//! Boss1 State_PostSpinSetup
	class Boss1StatePostSpinSetup : public FSMState<Boss1>
	{
	public:
		void Begin(s32 iPreviousState);
		void Update();
		void End(s32 iNextState);
	private:
		bool m_bActive;
		f32 m_fSrcRotation;
		f32 m_fDestRotation;
		f32 m_fInterpolator;
	};

	//! Boss1 State_Retreat
	class Boss1StateRetreat : public FSMState<Boss1>
	{
	public:
		void Begin(s32 iPreviousState);
		void Update();
		void End(s32 iNextState);
	};

	//! Boss1 State_Exploding
	class Boss1StateExploding : public FSMState<Boss1>
	{
	public:
		void Begin(s32 iPreviousState);
		void Update();
		void End(s32 iNextState);

	private:
		f32 m_fTimer;
	};

	//! Boss1 State_Inactive
	class Boss1StateInactive : public FSMState<Boss1>
	{
	public:
		void Begin(s32 iPreviousState) { }
		void Update() { }
		void End(s32 iNextState) { }
	};
}

#endif // _BOSS1_FSM_H_INCLUDED_

