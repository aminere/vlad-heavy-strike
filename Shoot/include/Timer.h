/* 

Amine Rehioui
Created: August 7th 2011

*/

#ifndef _TIMER_H_INCLUDED_
#define _TIMER_H_INCLUDED_

namespace shoot
{
	// forwards
	class TimeManager;

	//! Timer class
	class Timer
	{
	public:

		//! constructor
		Timer();

		//! destructor
		~Timer();

		//! starts the timer
		void Start(f32 fDuration);

		//! stops the timer
		void Stop();

		//! returns true if the timer is running
		inline bool IsRunning() const { return m_bRunning; }

		//! returns stopped status
		inline bool IsStopped() const { return !m_bRunning; }

		//! return the progress remaining
		inline f32 GetProgressRemaining() const { return m_fTimeLeft; }

		//! returns the total duration
		inline f32 GetDuration() const { return m_fDuration; }

		//! returns the progress ratio
		f32 GetProgressRatio() const;

		//! returns the progress duration
		f32 GetProgressDuration() const;

		//! advances the timer
		void Advance(f32 fDeltaTime);

	private:

		f32 m_fDuration;
		f32 m_fTimeLeft;
		bool m_bRunning;
	};
}

#endif // _TIMER_H_INCLUDED_

