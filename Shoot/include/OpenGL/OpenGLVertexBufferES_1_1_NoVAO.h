/* 

Amine Rehioui
Created: July 11th 2013

*/

#ifndef _OPENGL_VERTEX_BUFFER_ES_1_1_NoVAO_H_INCLUDED_
#define _OPENGL_VERTEX_BUFFER_ES_1_1_NoVAO_H_INCLUDED_

#include "OpenGLVertexBufferES_1_1.h"

namespace shoot
{
	//! OpenGL vertex buffer ES 1.1 No VAO
	class OpenGLVertexBufferES_1_1_NoVAO : public OpenGLVertexBufferES_1_1
	{
	public:

		//! Load into video memory
		void GraphicLoad();

		//! returns loaded status
		bool IsLoaded() { return m_VBO_ID != 0; }

		//! updates the vertex buffer data
		void UpdateData();

		//! begins rendering using this vertex buffer
		void Begin();

		//! ends rendering by this vertex buffer
		void End();

		//! create a copy of the object
		Object* Copy() const;
	};
}

#endif // _OPENGL_VERTEX_BUFFER_ES_1_1_NoVAO_H_INCLUDED_

