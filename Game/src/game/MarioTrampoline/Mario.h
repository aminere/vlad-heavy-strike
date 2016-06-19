/* 

Amine Rehioui
Created: May 4th 2010

*/

#ifndef _MARIO_H_INCLUDED_
#define _MARIO_H_INCLUDED_

#include "Sprite.h"

// forwards
class MarioFSM;

class Mario : public shoot::Sprite
{
	typedef shoot::Sprite super;

public:

	//! Macro to enable registration whithin the factory
	DEFINE_ENTITY(Mario);

	//! Constructor
	Mario();

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

	MarioFSM* m_pFSM;
};

#endif // _MARIO_H_INCLUDED_