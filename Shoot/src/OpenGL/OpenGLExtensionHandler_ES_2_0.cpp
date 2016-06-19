/* 

Amine Rehioui
Created: August 15th 2010

Uses code from the Irrlicht engine extension handler

*/

#include "Shoot.h"

#include "OpenGLExtensionHandlerES_2_0.h"



#if defined(USE_GLES_2_0)

namespace shoot
{
	const char* const OpenGLExtensionHandler_ES_2_0::ms_strExtensionStrings[OpenGLExtensionHandler_ES_2_0::E_Count] =
	{
		"GL_OES_framebuffer_object", // E_FBO
		"Native", // E_MultiTexture
		"GL_OES_texture_cube_map", // E_CubeMap
		"None", // "Native", // E_PointSprite // Disabled for consistency with DX11
		"Native", // E_PointParams
		"GL_OES_vertex_buffer_object", // E_VBO
		"GL_OES_vertex_array_object", // E_VAO
		"Native", // E_VertexBuffer
		"Native", // E_FragmentShader
		"Native", // E_RectangleTexture
	};

	//! Constructor
	OpenGLExtensionHandler_ES_2_0::OpenGLExtensionHandler_ES_2_0()
		: super(ms_strExtensionStrings)		
	{
	}

	// extensions
	void OpenGLExtensionHandler_ES_2_0::extGlGenFramebuffers(GLsizei n, GLuint *framebuffers)
	{
		GL_CHECK(glGenFramebuffersOES(n, framebuffers));		
	}
	
	void OpenGLExtensionHandler_ES_2_0::extGlBindFramebuffer(GLenum target, GLuint framebuffer)
	{
		GL_CHECK(glBindFramebufferOES(target, framebuffer));
	}

	void OpenGLExtensionHandler_ES_2_0::extGlFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
	{
		glFramebufferTexture2DOES(target, attachment, textarget, texture, level);
	}

	void OpenGLExtensionHandler_ES_2_0::extGlDeleteFramebuffers(GLsizei n, const GLuint *framebuffers)
	{
		GL_CHECK(glDeleteFramebuffersOES(n, framebuffers));
	}

	void OpenGLExtensionHandler_ES_2_0::extGlBindRenderbuffer(GLenum target, GLuint renderbuffer)
	{
		GL_CHECK(glBindRenderbufferOES(target, renderbuffer));
	}

	void OpenGLExtensionHandler_ES_2_0::extGlDeleteRenderbuffers(GLsizei n, const GLuint *renderbuffers)
	{
		GL_CHECK(glDeleteRenderbuffersOES(n, renderbuffers));
	}

	void OpenGLExtensionHandler_ES_2_0::extGlGenRenderbuffers(GLsizei n, GLuint *renderbuffers)
	{
		GL_CHECK(glGenRenderbuffersOES(n, renderbuffers));
	}

	void OpenGLExtensionHandler_ES_2_0::extGlRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
	{
		GL_CHECK(glRenderbufferStorageOES(target, internalformat, width, height));
	}

	void OpenGLExtensionHandler_ES_2_0::extGlFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
	{
		GL_CHECK(glFramebufferRenderbufferOES(target, attachment, renderbuffertarget, renderbuffer));
	}

	void OpenGLExtensionHandler_ES_2_0::extGlActiveTexture(GLenum texture)
	{
		GL_CHECK(glActiveTexture(texture));
	}

	void OpenGLExtensionHandler_ES_2_0::extGlClientActiveTexture(GLenum texture)
	{
		GL_CHECK(glClientActiveTexture(texture));
	}

	void OpenGLExtensionHandler_ES_2_0::extGlPointParameterf(GLenum pname, GLfloat param)
	{
		GL_CHECK(glPointParameterf(pname, param));
	}

	void OpenGLExtensionHandler_ES_2_0::extGlPointParameterfv(GLenum pname, const GLfloat *params)
	{
		GL_CHECK(glPointParameterfv(pname, params));
	}

	void OpenGLExtensionHandler_ES_2_0::extGlBindBuffer(GLenum target, GLuint buffer)
	{
		GL_CHECK(glBindBuffer(target, buffer));
	}

	void OpenGLExtensionHandler_ES_2_0::extGlDeleteBuffers(GLsizei n, const GLuint *buffers)
	{
		GL_CHECK(glDeleteBuffers(n, buffers));
	}

	void OpenGLExtensionHandler_ES_2_0::extGlGenBuffers(GLsizei n, GLuint *buffers)
	{
		GL_CHECK(glGenBuffers(n, buffers));
		SHOOT_ASSERT(GL_CHECK(glGetError() == GL_NO_ERROR, "glGenBuffers failed"));
	}

	void OpenGLExtensionHandler_ES_2_0::extGlBufferData(GLenum target, GLsizei size, const void* data, GLenum usage)
	{
		GL_CHECK(glBufferData(target, size, data, usage));
	}

	void OpenGLExtensionHandler_ES_2_0::extGlCreateShader(E_ShaderType type, GLuint *shader)
	{
		static const GLenum glTypes[ST_Count] = 
		{
			GL_VERTEX_SHADER,
			GL_FRAGMENT_SHADER
		};
		
		*shader = GL_CHECK(glCreateShader(glTypes[type]));
	}

	void OpenGLExtensionHandler_ES_2_0::extGlDeleteShader(GLuint shader)
	{
		GL_CHECK(glDeleteShader(shader));
	}

	void OpenGLExtensionHandler_ES_2_0::extGlShaderSource(GLuint shader, GLsizei count, const char* *string, const GLint *length)
	{
		GL_CHECK(glShaderSource(shader, count, string, length));
	}

	void OpenGLExtensionHandler_ES_2_0::extGlCompileShader(GLuint shader)
	{
		GL_CHECK(glCompileShader(shader));
	}

	void OpenGLExtensionHandler_ES_2_0::extGlCreateProgram(GLuint* program)
	{
		*program = GL_CHECK(glCreateProgram());
	}

	void OpenGLExtensionHandler_ES_2_0::extGlDeleteProgram(GLuint program)
	{
		GL_CHECK(glDeleteProgram(program));
	}

	void OpenGLExtensionHandler_ES_2_0::extGlAttachShader(GLuint program, GLuint shader)
	{
		GL_CHECK(glAttachShader(program, shader));
	}

	void OpenGLExtensionHandler_ES_2_0::extGlDetachShader(GLuint program, GLuint shader)
	{
		GL_CHECK(glDetachShader(program, shader));
	}

	void OpenGLExtensionHandler_ES_2_0::extGlLinkProgram(GLuint program)
	{
		GL_CHECK(glLinkProgram(program));
	}

	void OpenGLExtensionHandler_ES_2_0::extGlUseProgram(GLuint program)
	{
		GL_CHECK(glUseProgram(program));
	}

	bool OpenGLExtensionHandler_ES_2_0::extGlCheckCompileStatus(GLuint shader)
	{
		GLint status = GL_TRUE;
		GL_CHECK(glGetShaderiv(shader, GL_COMPILE_STATUS, &status));
		if(status != GL_TRUE)
		{
			GLint logsize = 0;
			pGlGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logsize);
			char *log = snew char[logsize+1];
			memset(log, 0, logsize+1);
			pGlGetShaderInfoLog(shader, logsize, &logsize, log);
			SHOOT_ASSERT(false, "Shader compile error: %s", log);
			sdelete_array(log);
			return false;
		}
		return true;
	}

	bool OpenGLExtensionHandler_ES_2_0::extGlCheckLinkStatus(GLuint program)
	{
		GLint status = GL_TRUE;
		GL_CHECK(glGetProgramiv(program, GL_LINK_STATUS, &status));
		if(status != GL_TRUE)
		{
			GLint logsize = 0;
			GL_CHECK(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logsize));
			char *log = snew char[logsize+1];
			memset(log, 0, logsize+1);
			GL_CHECK(glGetProgramInfoLog(program, logsize, &logsize, log));
			Log << "Program " << program << " link error:\n" << log << "\n";
			sdelete_array(log);
			return false;
		}
		return true;
	}
	
	void OpenGLExtensionHandler_ES_2_0::extGlGetUniformLocation(GLuint program, const char* name, GLuint* location)
	{
		*location = pGlGetUniformLocationARB(program, name);
	}
	
	void OpenGLExtensionHandler_ES_2_0::extGlUniform1fv(GLint location, GLsizei count, const GLfloat *value)
	{
		pGlUniform1fvARB(location, count, value);
	}
	
	void OpenGLExtensionHandler_ES_2_0::extGlUniform2fv(GLint location, GLsizei count, const GLfloat *value)
	{
		pGlUniform2fvARB(location, count, value);
	}
	
	void OpenGLExtensionHandler_ES_2_0::extGlUniform3fv(GLint location, GLsizei count, const GLfloat *value)
	{
		pGlUniform3fvARB(location, count, value);
	}
	
	void OpenGLExtensionHandler_ES_2_0::extGlUniform4fv(GLint location, GLsizei count, const GLfloat *value)
	{
		pGlUniform4fvARB(location, count, value);
	}
	
	void OpenGLExtensionHandler_ES_2_0::extGlUniform1iv(GLint location, GLsizei count, const GLint *value)
	{
		pGlUniform1ivARB(location, count, value);
	}
	
	void OpenGLExtensionHandler_ES_2_0::extGlUniform2iv(GLint location, GLsizei count, const GLint *value)
	{
		pGlUniform2ivARB(location, count, value);
	}
	
	void OpenGLExtensionHandler_ES_2_0::extGlUniform3iv(GLint location, GLsizei count, const GLint *value)
	{
		pGlUniform3ivARB(location, count, value);
	}
	
	void OpenGLExtensionHandler_ES_2_0::extGlUniform4iv(GLint location, GLsizei count, const GLint *value)
	{
		pGlUniform4ivARB(location, count, value);
	}
	
	void OpenGLExtensionHandler_ES_2_0::extGlUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
	{
		pGlUniformMatrix2fvARB(location, count, transpose, value);
	}
	
	void OpenGLExtensionHandler_ES_2_0::extGlUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
	{
		pGlUniformMatrix3fvARB(location, count, transpose, value);
	}
	
	void OpenGLExtensionHandler_ES_2_0::extGlUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
	{
		pGlUniformMatrix4fvARB(location, count, transpose, value);
	}
	
	void OpenGLExtensionHandler_ES_2_0::extGlGetAttribLocation(GLuint program, const char* name, GLint* location)
	{
		*location = pGlGetAttribLocationARB(program, name);
	}
	
	void OpenGLExtensionHandler_ES_2_0::extGlVertexAttrib1fv(GLuint index, const GLfloat *v)
	{
		pGlVertexAttrib1fvARB(index, v);
	}
	
	void OpenGLExtensionHandler_ES_2_0::extGlVertexAttrib2fv(GLuint index, const GLfloat *v)
	{
		pGlVertexAttrib2fvARB(index, v);
	}
	
	void OpenGLExtensionHandler_ES_2_0::extGlVertexAttrib3fv(GLuint index, const GLfloat *v)
	{
		pGlVertexAttrib3fvARB(index, v);
	}
	
	void OpenGLExtensionHandler_ES_2_0::extGlVertexAttrib4fv(GLuint index, const GLfloat *v)
	{
		pGlVertexAttrib4fvARB(index, v);
	}
	
	void OpenGLExtensionHandler_ES_2_0::extGlEnableVertexAttribArray(GLuint index)
	{
		pGlEnableVertexAttribArrayARB(index);
	}
	
	void OpenGLExtensionHandler_ES_2_0::extGlVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer)
	{
		pGlVertexAttribPointerARB(index, size, type, normalized, stride, pointer);
	}
}

#endif // USE_GLES_2_0

