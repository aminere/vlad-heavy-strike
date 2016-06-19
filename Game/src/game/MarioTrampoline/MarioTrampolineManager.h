/* 

Amine Rehioui
Created: May 4th 2010

*/

#ifndef _MARIO_TRAMPOLINE_MANAGER_H_INCLUDED_
#define _MARIO_TRAMPOLINE_MANAGER_H_INCLUDED_

#include "Entity.h"

// forwards
class MarioTrampolineFSM;

class MarioTrampolineManager : public shoot::Entity
{
	typedef shoot::Entity super;

public:

	//! Macro to enable registration whithin the factory
	DEFINE_ENTITY(MarioTrampolineManager);

	//! Constructor
	MarioTrampolineManager();

	//! serializes the entity to/from a PropertyStream
	void Serialize(shoot::PropertyStream& stream);

	//! called during the initialization of the entity
	void Init();

	//! called during the update of the entity
	/** \param fDeltaTime: time since last Update */
	void Update(shoot::f32 fDeltaTime);

	// called during the unitialization of the entity
	void Uninit();

private:
	
	MarioTrampolineFSM* m_pFSM;
};

#endif // _MARIO_TRAMPOLINE_MANAGER_H_INCLUDED_