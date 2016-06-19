/* 

Amine Rehioui
Created: August 7th 2011

*/

#ifndef _TIMER_MANAGER_H_INCLUDED_
#define _TIMER_MANAGER_H_INCLUDED_

namespace shoot
{
	// forwards
	class Timer;
	class Clock;

	//! TimeManager class
	class TimeManager : public Singleton<TimeManager>
	{
		DECLARE_SINGLETON(TimeManager);

	public:

		//! registers a timer
		void RegisterTimer(Timer* pTimer);

		//! unregisters a timer
		void UnregisterTimer(Timer* pTimer);

		//! registers a clock
		void RegisterClock(Clock* pTimer);

		//! unregisters a clock
		void UnregisterClock(Clock* pTimer);

		//! updates the timers
		void Update();

	private:

		std::vector<Timer*> m_aTimers;
		std::vector<Clock*> m_aClocks;
	};
};

#endif // _TIMER_MANAGER_H_INCLUDED_


