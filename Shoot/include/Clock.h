/* 

Amine Rehioui
Created: December 2nd 2011

*/

#ifndef _CLOCK_H_INCLUDED_
#define _CLOCK_H_INCLUDED_

namespace shoot
{
	// forwards
	class TimeManager;

	//! Clock class
	class Clock
	{
	public:

		//! constructor
		Clock();

		//! destructor
		~Clock();

		//! starts the clock
		void Start();

		//! stops the clock
		void Stop();

		//! returns started status
		inline bool IsStarted() const { return m_bStarted; }

		//! returns stopped status
		inline bool IsStopped() const { return !m_bStarted; }

		//! return the time since the clock start
		inline f32 GetTime() const { return m_fTime; }

		//! advances the clock
		void Advance(f32 fDeltaTime);

	private:

		f32 m_fTime;
		bool m_bStarted;
	};
}

#endif // _CLOCK_H_INCLUDED_

