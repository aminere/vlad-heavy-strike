/* 

Amine Rehioui
Created: July 14th 2012

*/

#include "Shoot.h"	

#include "ReceiveTriggerVisitor.h"

#include "EventManager.h"

namespace shoot
{
	DEFINE_OBJECT(ReceiveTriggerVisitor);

	//! Reads/Writes struct properties from/to a stream
	void ReceiveTriggerVisitor::Serialize(PropertyStream& stream)
	{
		stream.Serialize(PT_String, "TriggerName", &m_TriggerName);
		stream.SerializeReference("VisitorToTrigger", &m_VisitorToTrigger);
	}

	//! visits a particular entity
	void ReceiveTriggerVisitor::Visit(Entity* pTarget)
	{
		EventManager::Instance()->RegisterListener<TriggerEvent>(this, &HandleTriggerEvent);

		super::Visit(pTarget);
	}

	//! Leaves the visited entity
	void ReceiveTriggerVisitor::Leave()
	{
		EventManager::Instance()->UnregisterListener<TriggerEvent>(this);

		if(m_VisitorToTrigger.IsValid())		
		{
			m_VisitorToTrigger->Leave();
		}

		super::Leave();
	}

	//! updates the visitor		
	void ReceiveTriggerVisitor::Update()
	{
		if(!m_bActive)
		{
			return;
		}

		if(m_VisitorToTrigger.IsValid()
		&& m_VisitorToTrigger->IsActive())
		{
			m_VisitorToTrigger->Update();
		}
	}

	//! shoot event listeners
	void ReceiveTriggerVisitor::HandleTriggerEvent(void* pListener, Event* pEvent)
	{
		TriggerEvent* pTrigger = static_cast<TriggerEvent*>(pEvent);
		ReceiveTriggerVisitor* pTriggerReceiver = static_cast<ReceiveTriggerVisitor*>(pListener);

		if(pTriggerReceiver->GetParent() == pTrigger->Target.Get()
		&& pTriggerReceiver->m_TriggerName == pTrigger->Name
		&& pTriggerReceiver->m_VisitorToTrigger.IsValid())
		{
			pTriggerReceiver->m_VisitorToTrigger->Visit(pTriggerReceiver->GetParent());
		}
	}
}

