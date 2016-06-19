/* 

Amine Rehioui
Created: July 23rd 2011

*/

#ifndef _EVENT_H_INCLUDED_
#define _EVENT_H_INCLUDED_

namespace shoot
{
	//! Event class
	class Event : public Object
	{
		DECLARE_OBJECT(Event, Object);
	public:

		virtual ~Event()
		{
		}
	};

	//! trigger event
	class TriggerEvent : public Event
	{
		DECLARE_OBJECT(TriggerEvent, Event);
	public:

		Reference<Object> Target;
		std::string Name;
	};

	//! UI event
	class UIEvent : public Event
	{
		DECLARE_OBJECT(UIEvent, Event);
	public:

		Reference<Object> m_Sender;
		std::string m_Command;
		bool m_bChecked;
	};
}

#endif // _EVENT_H_INCLUDED_

