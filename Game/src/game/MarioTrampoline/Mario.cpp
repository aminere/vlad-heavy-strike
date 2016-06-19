/* 

Amine Rehioui
Created: May 4th 2010

*/

#include "Types.h"

#include "Mario.h"

#include "MarioFSM.h"

//! Constructor
Mario::Mario()
: m_pFSM(NULL)
{
}

//! serializes the entity to/from a PropertyStream
void Mario::Serialize(shoot::PropertyStream& stream)
{
	super::Serialize(stream);
}

//! called during the initialization of the entity
void Mario::Init()
{
	m_pFSM = new MarioFSM(this);
	m_pFSM->Init();

	super::Init();
}

//! called during the update of the entity
/** \param fDeltaTime: time since last Update */
void Mario::Update(shoot::f32 fDeltaTime)
{
	m_pFSM->Update(fDeltaTime);

	super::Update(fDeltaTime);
}

// called during the unitialization of the entity
void Mario::Uninit()
{
	delete m_pFSM;

	super::Uninit();
}

