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
		BaseLog& operator << (size_t);
		BaseLog& operator << (float);

	private:

		//! Prints to available output streams
		void PrintInternal(const char* str);
	};

	extern BaseLog Log;
}

#endif // _LOG_H_INCLUDED_

