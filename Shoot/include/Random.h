/* 

Amine Rehioui
Created: May 30th 2010

Implementation based on http://www.codeproject.com/KB/recipes/SimpleRNG.aspx

*/

#ifndef _RANDOM_H_INCLUDED_
#define _RANDOM_H_INCLUDED_

namespace shoot
{
	//! Random number generator
	class Random
	{
	public:

		//! initializes the random number generator
		static void Initialize();

		//! explicitly changes the seed
		static void SetSeed(u32 seed);

		//! returns a number in range, exclusive
		static f32 GetFloat(f32 min, f32 max);

		//! returns an integer number in a range, inclusive
		static u32 GetInt(u32 min, u32 max);

		//! returns a random number between 0 and 1
		static f32 GetFloat();

		//! Uses George Marsaglia's MWC algorithm to produce an unsigned integer
		static u32 NextU32();

	private:

		static u32 m_w;
		static u32 m_z;
	};
}

#endif // _RANDOM_H_INCLUDED_

