/* 

Amine Rehioui
Created: August 14th 2010

*/

#ifndef _VERTEX_3D_H_INCLUDED_
#define _VERTEX_3D_H_INCLUDED_

#include "Vector2.h"
#include "Color.h"

namespace shoot
{
	//! 3D vertex class used by the graphics driver
	class Vertex3D
	{
	public:

		//! vertex flags
		enum E_VertexFlag
		{
			VF_Pos =	0x1 << 0,
			VF_UV  =	0x1 << 1,
			VF_Normal = 0x1 << 2,
			VF_Color =	0x1 << 3,
			VF_UVW  =	0x1 << 4,
			VF_2DQuad =	0x1 << 5,
			VF_3DQuad = 0x1 << 6
		};

		Vector3 Pos;
		Vector2 UV;
		Vector3 Normal;
		Color color;
		Vector3 UVW;
	};
}

#endif // _VERTEX_3D_H_INCLUDED_

