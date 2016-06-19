//
//  ES1Renderer.h
//  ShootTest
//
//  Created by Amine Rehioui on 10-08-04.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "ESRenderer.h"

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

#include "Types.h"
#include "GameEngine.h"

@interface ES1Renderer : NSObject <ESRenderer>
{
@public
    EAGLContext *context;

    // Important OpenGL ES buffers
    GLuint defaultFramebuffer, colorRenderbuffer, depthRenderbuffer;
	
	shoot::GameEngine* m_pEngine;
	bool m_bRunning;
	bool m_bReady;
}

- (void)render;
- (BOOL)resizeFromLayer:(CAEAGLLayer *)layer;
- (void)resetDeltaTime;

@end
