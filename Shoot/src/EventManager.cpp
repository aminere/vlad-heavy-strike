/* 

Amine Rehioui
Created: July 23rd 2011

*/

#include "Shoot.h"

#include "EventManager.h"

namespace shoot
{
	//! Constructor
	EventManager::EventManager()
	{
	}

	//! Destructor
	EventManager::~EventManager()
	{
		SHOOT_ASSERT(m_aListeners.size() == 0, "Destroying the EventManager while some listeners are still registered");
	}

	//! Sends an event
	void EventManager::SendEvent(Event* pEvent)
	{
		u32 numHandlers = 0;
		for(u32 i=0; i<m_aListeners.size(); ++i)
		{
			ListenerInfo* pListenerInfo = m_aListeners[i];
			if(pListenerInfo->strEventName == pEvent->GetClassName())
			{
				PendingEventInfo info = { pListenerInfo, Reference<Event>(pEvent) };
				m_aPendingEventInfo.push_back(info);
				numHandlers++;
			}
		}

		if(numHandlers == 0)
		{
			Log << "Event " << pEvent->GetClassName() << " has no registered listeners and will not be handled\n";
		}		
	}

	//! Update events
	void EventManager::Update()
	{
		if(m_aPendingEventInfo.size() > 0)
		{
			for(u32 i=0; i<m_aPendingEventInfo.size(); ++i)
			{
				PendingEventInfo& pendingInfo = m_aPendingEventInfo[i];
				ListenerInfo* pListenerInfo = pendingInfo.pListenerInfo;
				(pListenerInfo->pHandler)(pListenerInfo->pListener, pendingInfo.event);				
			}

			m_aPendingEventInfo.clear();
		}		
	}

	//! Finds a listener index
	s32 EventManager::GetListenerIndex(const char* strEventName, void* pListener)
	{
		for(u32 i=0; i<m_aListeners.size(); ++i)
		{
			ListenerInfo* pInfo = m_aListeners[i];
			if(pInfo->strEventName == strEventName && pInfo->pListener == pListener)
			{
				return i;
			}
		}
		return -1;
	}

	//! returns true if a listener is registered
	bool EventManager::IsRegistered(void *pListener)
	{
		for(u32 i=0; i<m_aListeners.size(); ++i)
		{
			if(m_aListeners[i]->pListener == pListener)
			{
				return true;
			}
		}
		return false;
	}	
}
