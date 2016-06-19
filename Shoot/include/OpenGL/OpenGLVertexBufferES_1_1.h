/* 

Amine Rehioui
Created: Marche 20th 2011

*/

#ifndef _OPENGL_VERTEX_BUFFER_ES_1_1_H_INCLUDED_
#define _OPENGL_VERTEX_BUFFER_ES_1_1_H_INCLUDED_

#include "OpenGLVertexBuffer.h"

namespace shoot
{
	//! OpenGL vertex buffer ES 1.1
	class OpenGLVertexBufferES_1_1 : public OpenGLVertexBuffer
	{
	public:

		//! instanced render method
		void Draw(const std::vector<Matrix44>& aWorldTransforms, const std::vector<Matrix44>& aTextureTransforms, Material* pMaterial);

		//! create a copy of the object
		virtual Object* Copy() const;
	};
}

#endif // _OPENGL_VERTEX_BUFFER_ES_1_1_H_INCLUDED_

