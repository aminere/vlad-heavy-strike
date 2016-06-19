/* 

Amine Rehioui
Created: April 11th 2010

*/

#ifndef _POINT_H_INCLUDED_
#define _POINT_H_INCLUDED_

namespace shoot
{
	// forwards
	class Vector2; // operator = (const Vector2&)

	//! 2D point class
	class Point
	{
	public:

		//! constructor
		Point() : X(0), Y(0)
		{
		}

		//! constructor
		Point(s32 _X, s32 _Y) : X(_X), Y(_Y)
		{
		}
		
		// operators
		Point operator + (const Point& other) const;		
		Point operator - (const Point& other) const;		
		Point& operator += (const Point& other);
		Point& operator -= (const Point& other);
		Point& operator = (const Vector2& vec2D);

		inline bool operator == (const Point& other) const
		{
			return (X == other.X) && (Y == other.Y);
		}

		inline bool operator != (const Point& other) const
		{
			return !this->operator==(other);
		}

		s32 X, Y;
	};
}

#endif // _POINT_H_INCLUDED_

