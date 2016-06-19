/* 

Amine Rehioui
Created: August 16th 2010

Uses code from the Irrlicht engine extension handler

*/

#ifndef _OPENGL_EXTENSION_HANDLER_ES_1_1_H_INCLUDED_
#define _OPENGL_EXTENSION_HANDLER_ES_1_1_H_INCLUDED_

#include "OpenGLExtensionHandler.h"

namespace shoot
{
	//! OpenGL Extension handler
	class OpenGLExtensionHandler_ES_1_1 : public OpenGLExtensionHandler
	{
		typedef OpenGLExtensionHandler super;

	public:

		//! constructor
		OpenGLExtensionHandler_ES_1_1();

		// extensions
		void extGlGenFramebuffers(GLsizei n, GLuint *framebuffers);
		void extGlBindFramebuffer(GLenum target, GLuint framebuffer);
		void extGlFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
		void extGlDeleteFramebuffers(GLsizei n, const GLuint *framebuffers);
		GLenum extGlCheckFramebufferStatus(GLenum target);
		void extGlBindRenderbuffer(GLenum target, GLuint renderbuffer);
		void extGlDeleteRenderbuffers(GLsizei n, const GLuint *renderbuffers);
		void extGlGenRenderbuffers(GLsizei n, GLuint *renderbuffers);
		void extGlRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
		void extGlFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
		void extGlActiveTexture(GLenum texture);
		void extGlClientActiveTexture(GLenum texture);
		void extGlPointParameterf(GLenum pname, GLfloat param);
		void extGlPointParameterfv(GLenum pname, const GLfloat *params);
		void extGlBindBuffer(GLenum target, GLuint buffer);
		void extGlDeleteBuffers(GLsizei n, const GLuint *buffers);
		void extGlGenBuffers(GLsizei n, GLuint *buffers);
		void extGlBufferData(GLenum target, GLsizei size, const void* data, GLenum usage);
		void* extGlMapBuffer(GLenum target, GLenum access);
		GLboolean extGlUnmapBuffer(GLenum target);
		void extGlGenVertexArrays(GLsizei n, GLuint *arrays);
		void extGlBindVertexArray(GLuint array);
		void extGlDeleteVertexArrays(GLsizei n, const GLuint *arrays);
		void extGlCreateShader(E_ShaderType type, GLuint *shader);
		void extGlDeleteShader(GLuint shader);
		void extGlShaderSource(GLuint shader, GLsizei count, const char* *string, const GLint *length);
		void extGlCompileShader(GLuint shader);
		void extGlCreateProgram(GLuint* program);
		void extGlDeleteProgram(GLuint program);
		void extGlAttachShader(GLuint program, GLuint shader);
		void extGlDetachShader(GLuint program, GLuint shader);
		void extGlLinkProgram(GLuint program);
		void extGlUseProgram(GLuint program);
		bool extGlCheckCompileStatus(GLuint shader);
		bool extGlCheckLinkStatus(GLuint program);
		void extGlGetUniformLocation(GLuint program, const char* name, GLuint* location);
		void extGlUniform1fv(GLint location, GLsizei count, const GLfloat *value);
		void extGlUniform2fv(GLint location, GLsizei count, const GLfloat *value);
		void extGlUniform3fv(GLint location, GLsizei count, const GLfloat *value);
		void extGlUniform4fv(GLint location, GLsizei count, const GLfloat *value);
		void extGlUniform1iv(GLint location, GLsizei count, const GLint *value);
		void extGlUniform2iv(GLint location, GLsizei count, const GLint *value);
		void extGlUniform3iv(GLint location, GLsizei count, const GLint *value);
		void extGlUniform4iv(GLint location, GLsizei count, const GLint *value);
		void extGlUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
		void extGlUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
		void extGlUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
		void extGlGetAttribLocation(GLuint program, const char* name, GLint* location);
		void extGlVertexAttrib1fv(GLuint index, const GLfloat *v);
		void extGlVertexAttrib2fv(GLuint index, const GLfloat *v);
		void extGlVertexAttrib3fv(GLuint index, const GLfloat *v);
		void extGlVertexAttrib4fv(GLuint index, const GLfloat *v);
		void extGlEnableVertexAttribArray(GLuint index);
		void extGlVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
		void extGlVertexAttribDivisor(GLuint index, GLuint divisor);

	private:

		static const char* const ms_strExtensionStrings[E_Count];
	};
}

#endif // _OPENGL_EXTENSION_HANDLER_ES_1_1_H_INCLUDED_

