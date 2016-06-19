/* 

Amine Rehioui
Created: July 14th 2010

*/

#ifndef _OPENGL_H_INCLUDED_
#define _OPENGL_H_INCLUDED_

#define GL_CHECK(x) (x); SHOOT_LOG_WARNING(glGetError() == GL_NO_ERROR, #x" failed");

#if SHOOT_PLATFORM == SHOOT_PLATFORM_IOS
#if defined(USE_GLES_1_1)
	#include <OpenGLES/ES1/gl.h>
	#include <OpenGLES/ES1/glext.h>
#elif defined(USE_GLES_2_0)
	#include <OpenGLES/ES2/gl.h>
	#include <OpenGLES/ES2/glext.h>
#endif
#elif SHOOT_PLATFORM == SHOOT_PLATFORM_ANDROID
#include <GLES/gl.h>
#include <GLES/glext.h>
// ES 2.0 TODO
// #include <GLES2/gl2.h>
// #include <GLES2/gl2ext.h>
#else // desktop OGL
#include <GL/gl.h>
#include <GL/glu.h>
#include "glext.h"
#endif

#endif // _OPENGL_INCLUDES_H_INCLUDED_

