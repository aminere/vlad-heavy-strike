/* 

Amine Rehioui
Created: October 10th 2011

*/

#ifndef _PLAYER_FSM_H_INCLUDED_
#define _PLAYER_FSM_H_INCLUDED_

#include "FSM.h"

namespace shoot
{
	//! PlayerFSM
	class PlayerFSM : public FSM<Player>
	{
		typedef FSM<Player> super;
	public:

		//! states
		enum E_State
		{
			State_Idle,
			State_Exploding,
			State_FollowingPath,
			State_Respawning,
			NumStates
		};

		//! constructor
		PlayerFSM(Player* pPlayer) : super(pPlayer) 
		{
		}

		//! Initializes the state table
		void Init();
	};

	// Player States:
	//! Player State_Idle
	class PlayerStateIdle : public FSMState<Player>
	{
	public:
		void Begin(s32 iPreviousState);
		void Update();
		void End(s32 iNextState);
	};

	//! Player State_Exploding
	class PlayerStateExploding : public FSMState<Player>
	{
	public:
		void Begin(s32 iPreviousState);
		void Update();
		void End(s32 iNextState);
	private:
		f32 m_fTimer;
	};

	//! Player State_FollowingPath
	class PlayerStateFollowingPath : public FSMState<Player>
	{
	public:
		void Begin(s32 iPreviousState);
		void Update();
		void End(s32 iNextState);
	};

	//! Player State_Respawning
	class PlayerStateRespawning : public FSMState<Player>
	{
	public:
		void Begin(s32 iPreviousState);
		void Update();
		void End(s32 iNextState);
	};
}

#endif // _PLAYER_FSM_H_INCLUDED_

