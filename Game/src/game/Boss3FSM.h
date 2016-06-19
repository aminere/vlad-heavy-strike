/* 

Amine Rehioui
Created: August 13th 2013

*/

#ifndef _BOSS3_FSM_H_INCLUDED_
#define _BOSS3_FSM_H_INCLUDED_

#include "FSM.h"
#include "Boss3.h"

namespace shoot
{
	//! Boss3FSM
	class Boss3FSM : public FSM<Boss3>
	{
		typedef FSM<Boss3> super;
	public:

		//! states
		enum E_State
		{
			State_Intro,
			State_Idle,
			State_Sweep,
			State_CoreShot,
			State_Retreat,
			State_Exploding,
			State_Inactive,
			NumStates
		};

		//! constructor
		Boss3FSM(Boss3* pBoss3) : super(pBoss3) 
		{
		}

		//! Initializes the state table
		void Init();
	};

	// Boss3 States:
	//! Boss3 State_Intro
	class Boss3StateIntro : public FSMState<Boss3>
	{
	public:
		void Begin(s32 iPreviousState);
		void Update();
		void End(s32 iNextState);
	};

	//! Boss3 State_Idle
	class Boss3StateIdle : public FSMState<Boss3>
	{
	public:
		void Begin(s32 iPreviousState);
		void Update();
		void End(s32 iNextState);

	private:
		f32 m_fTimer;
	};	

	//! Boss3 State_Sweep
	class Boss3StateSweep : public FSMState<Boss3>
	{
	public:
		void Begin(s32 iPreviousState);
		void Update();
		void End(s32 iNextState);

	private:

		//! state
		enum E_State
		{
			S_Deploy,
			S_Sweep,
			S_Undeploy,
			S_Done
		};

		E_State m_State;
		f32 m_fTimer;
		f32 m_fInterpolator;
		Vector3 m_vSrcPos;
		Vector3 m_vDestPos;
		f32 m_fLaserTimer;		
	};

	//! Boss3 State_CoreShot
	class Boss3StateCoreShot : public FSMState<Boss3>
	{
	public:
		void Begin(s32 iPreviousState);
		void Update();
		void End(s32 iNextState);

	private:

		//! state
		enum E_State
		{
			S_Deploy,
			S_Shoot,
			S_Undeploy,
		};

		E_State m_State;
		f32 m_fInterpolator;
		f32 m_fSrcAngle;
		f32 m_fDestAngle;
		f32 m_fTimer;
		f32 m_fShootTimer;
	};

	//! Boss3 State_Retreat
	class Boss3StateRetreat : public FSMState<Boss3>
	{
	public:
		void Begin(s32 iPreviousState);
		void Update();
		void End(s32 iNextState);
	};

	//! Boss3 State_Exploding
	class Boss3StateExploding : public FSMState<Boss3>
	{
	public:
		void Begin(s32 iPreviousState);
		void Update();
		void End(s32 iNextState);

	private:
		f32 m_fTimer;
	};

	//! Boss3 State_Inactive
	class Boss3StateInactive : public FSMState<Boss3>
	{
	public:
		void Begin(s32 iPreviousState) { }
		void Update() { }
		void End(s32 iNextState) { }
	};
}

#endif // _Boss3_FSM_H_INCLUDED_

