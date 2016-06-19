/* 

Amine Rehioui
Created: May 30th 2010

*/

#ifndef _RANGE_H_INCLUDED_
#define _RANGE_H_INCLUDED_

namespace shoot
{
	//! Range class
	class Range
	{
	public:

		//! constructor
		Range() : Min(0.0f), Max(1.0f)
		{
		}

		//! constructor
		Range(f32 min, f32 max) : Min(min), Max(max)
		{
		}	

		inline bool operator == (const Range& other) const
		{
			return Math::FEqual(Min, other.Min) && Math::FEqual(Max, other.Max);
		}

		inline bool operator != (const Range& other) const
		{
			return !this->operator==(other);
		}

		f32 Min, Max;
	};
}

#endif // _RANGE_H_INCLUDED_

