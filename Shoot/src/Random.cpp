/* 

Amine Rehioui
Created: May 30th 2010

Implementation based on http://www.codeproject.com/KB/recipes/SimpleRNG.aspx

*/

#include "Shoot.h"



#include <time.h>

namespace shoot
{
	// static variables initialization
	u32 Random::m_z = 0;
	u32 Random::m_w = 362436069;

	//! initializes the random number generator
	void Random::Initialize()
	{
		m_z = u32(time(NULL));		
	}

	//! explicitly changes the seed
	void Random::SetSeed(u32 seed)
	{
		m_z = seed;		
	}

	//! returns a number in range, exclusive
	f32 Random::GetFloat(f32 min, f32 max)
	{
		f32 value = GetFloat();
		return min + (value * (max - min));
	}

	//! returns an integer number in a range, inclusive
	u32 Random::GetInt(u32 min, u32 max)
	{
		f32 value = GetFloat();
		return min + u32(value * (max + 1 - min));
	}

	//! returns a random number between 0 and 1
	f32 Random::GetFloat()
	{
		u32 u = NextU32();
		// The magic number below is 1/(2^32 + 2).
		// The result is strictly between 0 and 1.
		return f32(u + 1) * f32(2.328306435454494e-10);
	}

	//! Uses George Marsaglia's MWC algorithm to produce an unsigned integer
	u32 Random::NextU32()
	{
		m_z = 36969 * (m_z & 65535) + (m_z >> 16);
		m_w = 18000 * (m_w & 65535) + (m_w >> 16);
		return (m_z << 16) + (m_w & 65535);
	}
}

