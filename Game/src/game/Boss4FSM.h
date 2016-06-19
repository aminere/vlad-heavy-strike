/* 

Amine Rehioui
Created: August 20th 2013

*/

#ifndef _BOSS4_FSM_H_INCLUDED_
#define _BOSS4_FSM_H_INCLUDED_

#include "FSM.h"
#include "Boss4.h"

namespace shoot
{
	//! Boss4FSM
	class Boss4FSM : public FSM<Boss4>
	{
		typedef FSM<Boss4> super;
	public:

		//! states
		enum E_State
		{
			State_Intro,
			State_Idle,
			State_Lasers,
			State_Missiles,
			State_Retreat,
			State_Exploding,
			State_Inactive,
			NumStates
		};

		//! constructor
		Boss4FSM(Boss4* pBoss4) : super(pBoss4) 
		{
		}

		//! Initializes the state table
		void Init();
	};

	// Boss4 States:
	//! Boss4 State_Intro
	class Boss4StateIntro : public FSMState<Boss4>
	{
	public:
		void Begin(s32 iPreviousState);
		void Update();
		void End(s32 iNextState);
	};

	//! Boss4 State_Idle
	class Boss4StateIdle : public FSMState<Boss4>
	{
	public:
		void Begin(s32 iPreviousState);
		void Update();
		void End(s32 iNextState);

	private:
		f32 m_fTimer;
	};

	//! Boss4 State_Lasers
	class Boss4StateLasers : public FSMState<Boss4>
	{
	public:
		void Begin(s32 iPreviousState);
		void Update();
		void End(s32 iNextState);

	private:

		f32 m_fTimer;
		f32 m_fShootTimer;
		Vector3 m_vOriginalCannonPos[Boss4::m_sNumLaserCannons];
	};	

	//! Boss4 State_Missiles
	class Boss4StateMissiles : public FSMState<Boss4>
	{
	public:
		void Begin(s32 iPreviousState);
		void Update();
		void End(s32 iNextState);

	private:

		f32 m_fTimer;
		f32 m_fShootTimer;
		Vector3 m_vOriginalCannonPos[Boss4::m_sNumMissileCannons];
	};

	//! Boss4 State_Retreat
	class Boss4StateRetreat : public FSMState<Boss4>
	{
	public:
		void Begin(s32 iPreviousState);
		void Update();
		void End(s32 iNextState);
	};

	//! Boss4 State_Exploding
	class Boss4StateExploding : public FSMState<Boss4>
	{
	public:
		void Begin(s32 iPreviousState);
		void Update();
		void End(s32 iNextState);

	private:
		f32 m_fTimer;
	};

	//! Boss4 State_Inactive
	class Boss4StateInactive : public FSMState<Boss4>
	{
	public:
		void Begin(s32 iPreviousState) { }
		void Update() { }
		void End(s32 iNextState) { }
	};
}

#endif // _BOSS4_FSM_H_INCLUDED_

