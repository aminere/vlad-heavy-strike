/* 

Amine Rehioui
Created: Marche 20th 2011

*/

#ifndef _OPENGL_VERTEX_BUFFER_H_INCLUDED_
#define _OPENGL_VERTEX_BUFFER_H_INCLUDED_

#include "VertexBuffer.h"

namespace shoot
{
	//! OpenGL vertex buffer
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:

		//! constructor
		OpenGLVertexBuffer();

		//! destructor
		virtual ~OpenGLVertexBuffer();

		//! Load into video memory
		virtual void GraphicLoad();

		//! Unload from video memory
		virtual void GraphicUnload();

		//! returns loaded status
		virtual bool IsLoaded() { return m_VAO_ID != 0; }

		//! updates the vertex buffer data
		virtual void UpdateData();

		//! begins rendering using this vertex buffer
		virtual void Begin();

		//! default render method
		virtual void Draw();

		//! instanced render method
		virtual void Draw(const std::vector<Matrix44>& aWorldTransforms, const std::vector<Matrix44>& aTextureTransforms, Material* pMaterial);

		//! ends rendering by this vertex buffer
		virtual void End();

		//! create a copy of the object
		virtual Object* Copy() const;

	protected:

		u32 m_VAO_ID;
		u32 m_VBO_ID;
		u32 m_IBO_ID;
	};
}

#endif // _OPENGL_VERTEX_BUFFER_H_INCLUDED_

