/* 

Amine Rehioui
Created: July 23rd 2011

*/

#ifndef _EVENT_MANAGER_H_INCLUDED_
#define _EVENT_MANAGER_H_INCLUDED_

#include "Event.h"

namespace shoot
{
	//! Event manager class
	class EventManager : public Singleton<EventManager>
	{
		DECLARE_SINGLETON(EventManager);

	public:

		//! Event Handler signature
		typedef void EventHandler(void* pListener, Event* pEvent);

		//! Event Listener Info
		struct ListenerInfo
		{
			ListenerInfo(const std::string& _strEventName, void* _pListener, EventHandler* _pHandler)
				: strEventName(_strEventName)
				, pListener(_pListener)
				, pHandler(_pHandler)
			{
			}

			std::string strEventName;
			void* pListener;
			EventHandler* pHandler;
		};

		//! Register a listener
		template <class EventType>
		void RegisterListener(void* pListener, EventHandler* pHandler)
		{
			SHOOT_ASSERT(GetListenerIndex(EventType::GetStaticClassName(), pListener) == -1, "Trying to register the same listener twice for an event");
			ListenerInfo* pInfo = snew ListenerInfo(EventType::GetStaticClassName(), pListener, pHandler);			
			m_aListeners.push_back(pInfo);
		}

		//! Unregister a listener
		template <class EventType>
		void UnregisterListener(void* pListener)
		{
			s32 index = GetListenerIndex(EventType::GetStaticClassName(), pListener);
			SHOOT_ASSERT(index >= 0, "Trying to unregister an unexisting listener for an event");

			// make sure no pending events remain for this listener
			for(std::vector<PendingEventInfo>::iterator it = m_aPendingEventInfo.begin();
				it != m_aPendingEventInfo.end();)
			{
				if((*it).pListenerInfo->pListener == pListener)
				{
					it = m_aPendingEventInfo.erase(it);
				}
				else
				{
					++it;
				}
			}

			ListenerInfo* pInfo = *(m_aListeners.begin()+index);
			sdelete(pInfo);
			m_aListeners.erase(m_aListeners.begin()+index);
		}

		//! returns true if a listener is registered
		bool IsRegistered(void *pListener);

		//! Sends an event
		void SendEvent(Event* pEvent);

		//! Update events
		void Update();

	private:

		//! Finds a listener index
		s32 GetListenerIndex(const char* strEventName, void* pListener);

		std::vector<ListenerInfo*> m_aListeners;

		//! pending event info
		struct PendingEventInfo
		{
			ListenerInfo* pListenerInfo;
			Reference<Event> event;
		};

		std::vector<PendingEventInfo> m_aPendingEventInfo;
	};
}

#endif // _EVENT_MANAGER_H_INCLUDED_

