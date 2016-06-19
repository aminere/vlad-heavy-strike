/* 

Amine Rehioui
Created: May 4th 2010

*/

#include "Types.h"

#include "MarioTrampolineManager.h"

#include "MarioTrampolineFSM.h"

//! Constructor
MarioTrampolineManager::MarioTrampolineManager()
: m_pFSM(NULL)
{
}

//! serializes the entity to/from a PropertyStream
void MarioTrampolineManager::Serialize(shoot::PropertyStream& stream)
{
	super::Serialize(stream);
}

//! called during the initialization of the entity
void MarioTrampolineManager::Init()
{
	m_pFSM = new MarioTrampolineFSM(this);
	m_pFSM->Init();

	super::Init();
}

//! called during the update of the entity
/** \param fDeltaTime: time since last Update */
void MarioTrampolineManager::Update(shoot::f32 fDeltaTime)
{
	m_pFSM->Update(fDeltaTime);

	super::Update(fDeltaTime);
}

// called during the unitialization of the entity
void MarioTrampolineManager::Uninit()
{
	delete m_pFSM;

	super::Uninit();
}

