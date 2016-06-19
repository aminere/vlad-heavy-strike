/* 

Amine Rehioui
Created: May 1st 2010

*/

#include "Shoot.h"	

#include "Color.h"

namespace shoot
{
	// static variables initialization
	Color Color::Red = Color(1.0f, 0.0f, 0.0f, 1.0f);
	Color Color::Green = Color(0.0f, 1.0f, 0.0f, 1.0f);
	Color Color::Blue = Color(0.0f, 0.0f, 1.0f, 1.0f);
	Color Color::White = Color(1.0f, 1.0f, 1.0f, 1.0f);
	Color Color::Black = Color(0.0f, 0.0f, 0.0f, 1.0f);
	Color Color::Yellow = Color(1.0f, 1.0f, 0.0f, 1.0f);	
	Color Color::Zero = Color(0.0f, 0.0f, 0.0f, 0.0f);	

	//! constructor
	Color::Color() : R(1.0f), G(1.0f), B(1.0f), A(1.0f)
	{
	}

	bool Color::operator == (const Color& other) const
	{
		return (Math::FEqual(R, other.R)
			 && Math::FEqual(G, other.G)
			 && Math::FEqual(B, other.B)
			 && Math::FEqual(A, other.A));
	}

	Color Color::operator + (const Color& other) const
	{
		Color result(R+other.R, G+other.G, B+other.B, A+other.A);
		return result;
	}

	Color Color::operator - (const Color& other) const
	{
		Color result(R-other.R, G-other.G, B-other.B, A-other.A);
		return result;
	}

	Color Color::operator * (f32 fValue) const
	{
		Color result(R*fValue, G*fValue, B*fValue, A*fValue);
		return result;
	}
}

