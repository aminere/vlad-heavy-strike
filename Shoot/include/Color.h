/* 

Amine Rehioui
Created: April 18th 2010

*/

#ifndef _COLOR_H_INCLUDED_
#define _COLOR_H_INCLUDED_

namespace shoot
{
	//! Color class
	class Color
	{
	public:

		//! constructor
		Color();

		//! constructor
		Color(f32 r, f32 g, f32 b, f32 a = 1.0f) : R(r), G(g), B(b), A(a)
		{
		}

		//! sets the color
		void Set(f32 r, f32 g, f32 b, f32 a = 1.0f)
		{
			R = r;
			G = g;
			B = b;
			A = a;
		}

		// static constants
		static Color Red;
		static Color Green;
		static Color Blue;
		static Color White;
		static Color Black;
		static Color Yellow;
		static Color Zero;

		// operators
		bool operator == (const Color& other) const;
		inline bool operator != (const Color& other) { return !((*this == other)); }
		Color operator + (const Color& other) const;
		Color operator - (const Color& other) const;
		Color operator * (f32 fValue) const;

		union
		{
			struct
			{
				f32 R, G, B, A;
			};

			f32 v[4];
		};		
	};
}

#endif // _COLOR_H_INCLUDED_

