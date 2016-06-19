/* 

Amine Rehioui
Created: April 11th 2010

*/

#ifndef _SIZE_H_INCLUDED_
#define _SIZE_H_INCLUDED_

namespace shoot
{
	//! 2D size class
	class Size
	{
	public:

		//! constructor
		Size() : Width(0), Height(0)
		{
		}

		//! constructor
		Size(s32 w, s32 h) : Width(w), Height(h)
		{
		}		

		// operators
		bool operator == (const Size& other) const { return (Width == other.Width && Height == other.Height); }
		inline bool operator != (const Size& other) const { return !((*this) == other); }

		s32 Width, Height;
	};
}

#endif // _SIZE_H_INCLUDED_

