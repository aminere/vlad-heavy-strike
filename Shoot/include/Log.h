/* 

Amine Rehioui
Created: April 1st 2011

*/

#ifndef _LOG_H_INCLUDED_
#define _LOG_H_INCLUDED_

namespace shoot
{
	//! Log class
	class BaseLog
	{
	public:

		//! Prints to available output streams
		void Print(const char* format, ...);

		//! Print operators
		BaseLog& operator << (const char* str);
		BaseLog& operator << (u32 _u32);
		BaseLog& operator << (s32 _s32);
		BaseLog& operator << (f32 _f32);

	private:

		//! Prints to available output streams
		void PrintInternal(const char* str);
	};

	extern BaseLog Log;
}

#endif // _LOG_H_INCLUDED_

