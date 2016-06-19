/* 

Amine Rehioui
Created: April 7th 2013

*/

#ifndef _SEMAPHORE_H_INCLUDED_
#define _SEMAPHORE_H_INCLUDED_

namespace shoot
{
	//! Semaphore class
	class Semaphore
	{
	public:

		//! constructor
		static Semaphore* Create(u32 value);

		//! destructor
		virtual ~Semaphore()
		{
		}

		//! Decrement the semaphore and block the thread if necessary
		virtual void Wait() = 0;

		//! Increment the semaphore and unlock waiting threads if necessary
		virtual void Post() = 0;
	};
}

#endif // _SEMAPHORE_H_INCLUDED_

