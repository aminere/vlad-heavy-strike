/* 

Amine Rehioui
Created: August 15th 2010

Uses code from the Irrlicht engine extension handler

*/

#include "Shoot.h"

#include "OpenGLExtensionHandlerES_1_1.h"

namespace shoot
{
	const char* const OpenGLExtensionHandler_ES_1_1::ms_strExtensionStrings[OpenGLExtensionHandler_ES_1_1::E_Count] =
	{
		"GL_OES_framebuffer_object", // E_FBO
		"Native", // E_MultiTexture
		"None", // Disable Cubemaps until fully tested on ES1.1 devices
		"None", // "Native", // E_PointSprite // Disabled for consistency with DX11
		"Native", // E_PointParams
		"Native", // E_VBO
#if SHOOT_PLATFORM == SHOOT_PLATFORM_ANDROID
		"None", // disable VAOs on Android, doesn't even link
#else
		"GL_OES_vertex_array_object", // E_VAO
#endif
		"None", // E_VertexShader
		"None", // E_FragmentShader
		"GL_OES_texture_npot", // E_RectangleTexture
	};

	//! Constructor
	OpenGLExtensionHandler_ES_1_1::OpenGLExtensionHandler_ES_1_1()
		: super(ms_strExtensionStrings)
	{
	}

	// extensions
	void OpenGLExtensionHandler_ES_1_1::extGlGenFramebuffers(GLsizei n, GLuint *framebuffers)
	{
		GL_CHECK(glGenFramebuffersOES(n, framebuffers));
	}
	
	void OpenGLExtensionHandler_ES_1_1::extGlBindFramebuffer(GLenum target, GLuint framebuffer)
	{
		GL_CHECK(glBindFramebufferOES(target, framebuffer));
	}

	void OpenGLExtensionHandler_ES_1_1::extGlFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
	{
		glFramebufferTexture2DOES(target, attachment, textarget, texture, level);
	}

	void OpenGLExtensionHandler_ES_1_1::extGlDeleteFramebuffers(GLsizei n, const GLuint *framebuffers)
	{
		GL_CHECK(glDeleteFramebuffersOES(n, framebuffers));
	}

	GLenum OpenGLExtensionHandler_ES_1_1::extGlCheckFramebufferStatus(GLenum target)
	{
		return glCheckFramebufferStatusOES(target);
	}

	void OpenGLExtensionHandler_ES_1_1::extGlBindRenderbuffer(GLenum target, GLuint renderbuffer)
	{
		GL_CHECK(glBindRenderbufferOES(target, renderbuffer));
	}

	void OpenGLExtensionHandler_ES_1_1::extGlDeleteRenderbuffers(GLsizei n, const GLuint *renderbuffers)
	{
		GL_CHECK(glDeleteRenderbuffersOES(n, renderbuffers));
	}

	void OpenGLExtensionHandler_ES_1_1::extGlGenRenderbuffers(GLsizei n, GLuint *renderbuffers)
	{
		GL_CHECK(glGenRenderbuffersOES(n, renderbuffers));
	}

	void OpenGLExtensionHandler_ES_1_1::extGlRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
	{
		GL_CHECK(glRenderbufferStorageOES(target, internalformat, width, height));
	}

	void OpenGLExtensionHandler_ES_1_1::extGlFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
	{
		GL_CHECK(glFramebufferRenderbufferOES(target, attachment, renderbuffertarget, renderbuffer));
	}

	void OpenGLExtensionHandler_ES_1_1::extGlActiveTexture(GLenum texture)
	{
		GL_CHECK(glActiveTexture(texture));
	}

	void OpenGLExtensionHandler_ES_1_1::extGlClientActiveTexture(GLenum texture)
	{
		GL_CHECK(glClientActiveTexture(texture));
	}

	void OpenGLExtensionHandler_ES_1_1::extGlPointParameterf(GLenum pname, GLfloat param)
	{
		GL_CHECK(glPointParameterf(pname, param));
	}

	void OpenGLExtensionHandler_ES_1_1::extGlPointParameterfv(GLenum pname, const GLfloat *params)
	{
		GL_CHECK(glPointParameterfv(pname, params));
	}

	void OpenGLExtensionHandler_ES_1_1::extGlBindBuffer(GLenum target, GLuint buffer)
	{
		GL_CHECK(glBindBuffer(target, buffer));
	}

	void OpenGLExtensionHandler_ES_1_1::extGlDeleteBuffers(GLsizei n, const GLuint *buffers)
	{
		GL_CHECK(glDeleteBuffers(n, buffers));
	}

	void OpenGLExtensionHandler_ES_1_1::extGlGenBuffers(GLsizei n, GLuint *buffers)
	{
		GL_CHECK(glGenBuffers(n, buffers));
	}

	void OpenGLExtensionHandler_ES_1_1::extGlBufferData(GLenum target, GLsizei size, const void* data, GLenum usage)
	{
		GL_CHECK(glBufferData(target, size, data, usage));
	}
	
	void* OpenGLExtensionHandler_ES_1_1::extGlMapBuffer(GLenum target, GLenum access)
	{
		return glMapBufferOES(target, access);
	}

	GLboolean OpenGLExtensionHandler_ES_1_1::extGlUnmapBuffer(GLenum target)
	{
		return glUnmapBufferOES(target);
	}

	void OpenGLExtensionHandler_ES_1_1::extGlGenVertexArrays(GLsizei n, GLuint *arrays)
	{
#if SHOOT_PLATFORM == SHOOT_PLATFORM_ANDROID
		SHOOT_ASSERT(false, "Unsupported glGenVertexArraysOES");
#else
		GL_CHECK(glGenVertexArraysOES(n, arrays));
#endif
	}

	void OpenGLExtensionHandler_ES_1_1::extGlBindVertexArray(GLuint array)
	{
#if SHOOT_PLATFORM == SHOOT_PLATFORM_ANDROID
		SHOOT_ASSERT(false, "Unsupported glBindVertexArrayOES");
#else
		GL_CHECK(glBindVertexArrayOES(array));
#endif
	}

	void OpenGLExtensionHandler_ES_1_1::extGlDeleteVertexArrays(GLsizei n, const GLuint *arrays)
	{
#if SHOOT_PLATFORM == SHOOT_PLATFORM_ANDROID
		SHOOT_ASSERT(false, "Unsupported glDeleteVertexArraysOES");
#else
		GL_CHECK(glDeleteVertexArraysOES(n, arrays));
#endif
	}

	void OpenGLExtensionHandler_ES_1_1::extGlCreateShader(E_ShaderType type, GLuint *shader)
	{
		SHOOT_ASSERT(false, "Unsupported under GLES 1.1");
	}
	
	void OpenGLExtensionHandler_ES_1_1::extGlDeleteShader(GLuint shader)
	{
		SHOOT_ASSERT(false, "Unsupported under GLES 1.1");
	}
	
	void OpenGLExtensionHandler_ES_1_1::extGlShaderSource(GLuint shader, GLsizei count, const char* *string, const GLint *length)
	{
		SHOOT_ASSERT(false, "Unsupported under GLES 1.1");
	}
	
	void OpenGLExtensionHandler_ES_1_1::extGlCompileShader(GLuint shader)
	{
		SHOOT_ASSERT(false, "Unsupported under GLES 1.1");
	}
	
	void OpenGLExtensionHandler_ES_1_1::extGlCreateProgram(GLuint* program)
	{
		SHOOT_ASSERT(false, "Unsupported under GLES 1.1");
	}
	
	void OpenGLExtensionHandler_ES_1_1::extGlDeleteProgram(GLuint program)
	{
		SHOOT_ASSERT(false, "Unsupported under GLES 1.1");
	}
	
	void OpenGLExtensionHandler_ES_1_1::extGlAttachShader(GLuint program, GLuint shader)
	{
		SHOOT_ASSERT(false, "Unsupported under GLES 1.1");
	}
	
	void OpenGLExtensionHandler_ES_1_1::extGlDetachShader(GLuint program, GLuint shader)
	{
		SHOOT_ASSERT(false, "Unsupported under GLES 1.1");
	}
	
	void OpenGLExtensionHandler_ES_1_1::extGlLinkProgram(GLuint program)
	{
		SHOOT_ASSERT(false, "Unsupported under GLES 1.1");
	}
	
	void OpenGLExtensionHandler_ES_1_1::extGlUseProgram(GLuint program)
	{
		SHOOT_ASSERT(false, "Unsupported under GLES 1.1");
	}
	
	bool OpenGLExtensionHandler_ES_1_1::extGlCheckCompileStatus(GLuint shader)
	{
		SHOOT_ASSERT(false, "Unsupported under GLES 1.1");
		return false;
	}
	
	bool OpenGLExtensionHandler_ES_1_1::extGlCheckLinkStatus(GLuint program)
	{
		SHOOT_ASSERT(false, "Unsupported under GLES 1.1");
		return false;
	}
	
	void OpenGLExtensionHandler_ES_1_1::extGlGetUniformLocation(GLuint program, const char* name, GLuint* location)
	{
		SHOOT_ASSERT(false, "Unsupported under GLES 1.1");		
	}
	
	void OpenGLExtensionHandler_ES_1_1::extGlUniform1fv(GLint location, GLsizei count, const GLfloat *value)
	{
		SHOOT_ASSERT(false, "Unsupported under GLES 1.1");		
	}
	
	void OpenGLExtensionHandler_ES_1_1::extGlUniform2fv(GLint location, GLsizei count, const GLfloat *value)
	{
		SHOOT_ASSERT(false, "Unsupported under GLES 1.1");		
	}
	
	void OpenGLExtensionHandler_ES_1_1::extGlUniform3fv(GLint location, GLsizei count, const GLfloat *value)
	{
		SHOOT_ASSERT(false, "Unsupported under GLES 1.1");		
	}
	
	void OpenGLExtensionHandler_ES_1_1::extGlUniform4fv(GLint location, GLsizei count, const GLfloat *value)
	{
		SHOOT_ASSERT(false, "Unsupported under GLES 1.1");		
	}
	
	void OpenGLExtensionHandler_ES_1_1::extGlUniform1iv(GLint location, GLsizei count, const GLint *value)
	{
		SHOOT_ASSERT(false, "Unsupported under GLES 1.1");		
	}
	
	void OpenGLExtensionHandler_ES_1_1::extGlUniform2iv(GLint location, GLsizei count, const GLint *value)
	{
		SHOOT_ASSERT(false, "Unsupported under GLES 1.1");		 
	}
	
	void OpenGLExtensionHandler_ES_1_1::extGlUniform3iv(GLint location, GLsizei count, const GLint *value)
	{
		SHOOT_ASSERT(false, "Unsupported under GLES 1.1");		
	}
	
	void OpenGLExtensionHandler_ES_1_1::extGlUniform4iv(GLint location, GLsizei count, const GLint *value)
	{
		SHOOT_ASSERT(false, "Unsupported under GLES 1.1");		
	}
	
	void OpenGLExtensionHandler_ES_1_1::extGlUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
	{
		SHOOT_ASSERT(false, "Unsupported under GLES 1.1");		
	}
	
	void OpenGLExtensionHandler_ES_1_1::extGlUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
	{
		SHOOT_ASSERT(false, "Unsupported under GLES 1.1");		
	}
	
	void OpenGLExtensionHandler_ES_1_1::extGlUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
	{
		SHOOT_ASSERT(false, "Unsupported under GLES 1.1");		
	}
	
	void OpenGLExtensionHandler_ES_1_1::extGlGetAttribLocation(GLuint program, const char* name, GLint* location)
	{
		SHOOT_ASSERT(false, "Unsupported under GLES 1.1");		
	}
	
	void OpenGLExtensionHandler_ES_1_1::extGlVertexAttrib1fv(GLuint index, const GLfloat *v)
	{
		SHOOT_ASSERT(false, "Unsupported under GLES 1.1");		
	}
	
	void OpenGLExtensionHandler_ES_1_1::extGlVertexAttrib2fv(GLuint index, const GLfloat *v)
	{
		SHOOT_ASSERT(false, "Unsupported under GLES 1.1");		
	}
	
	void OpenGLExtensionHandler_ES_1_1::extGlVertexAttrib3fv(GLuint index, const GLfloat *v)
	{
		SHOOT_ASSERT(false, "Unsupported under GLES 1.1");		
	}
	
	void OpenGLExtensionHandler_ES_1_1::extGlVertexAttrib4fv(GLuint index, const GLfloat *v)
	{
		SHOOT_ASSERT(false, "Unsupported under GLES 1.1");		
	}
	
	void OpenGLExtensionHandler_ES_1_1::extGlEnableVertexAttribArray(GLuint index)
	{
		SHOOT_ASSERT(false, "Unsupported under GLES 1.1");		
	}
	
	void OpenGLExtensionHandler_ES_1_1::extGlVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer)
	{
		SHOOT_ASSERT(false, "Unsupported under GLES 1.1");
	}
	
	void OpenGLExtensionHandler_ES_1_1::extGlVertexAttribDivisor(GLuint index, GLuint divisor)
	{
		SHOOT_ASSERT(false, "Unsupported under GLES 1.1");
	}
}

