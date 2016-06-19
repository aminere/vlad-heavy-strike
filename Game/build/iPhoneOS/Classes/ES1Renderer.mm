//
//  ES1Renderer.m
//  ShootTest
//
//  Created by Amine Rehioui on 10-08-04.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "ES1Renderer.h"

#include "File.h"

#include "MemoryManager.h"

#include "GameThreadiOS.h"

#include "InputManager.h"

extern void RegisterObjects();

@implementation ES1Renderer

static ES1Renderer* g_spRenderer = NULL;
static CFTimeInterval m_PreviousTime;
shoot::InputManager::TouchState g_TouchState;
shoot::Mutex* g_spInputMutex;

static void SetContextCallback()
{
	[EAGLContext setCurrentContext:g_spRenderer->context];
}

static shoot::f32 GetDeltaTimeCallback()
{
	CFTimeInterval currentTime = CFAbsoluteTimeGetCurrent();
	float deltaTimeSecs = currentTime - m_PreviousTime;
	m_PreviousTime = currentTime;
	return deltaTimeSecs;
}

static void PresentCallback()
{
	[g_spRenderer->context presentRenderbuffer:GL_RENDERBUFFER_OES];
}

static void ApplyInputCallback()
{
	g_spInputMutex->Lock();
	shoot::InputManager::Instance()->SetTouchState(g_TouchState);
	g_spInputMutex->Unlock();
	
	shoot::InputManager::Instance()->Update();
}

static bool IsRunningCallback()
{
	return g_spRenderer->m_bRunning;
}

// Create an OpenGL ES 1.1 context
- (id)init
{
    if ((self = [super init]))
    {
		g_spRenderer = self;
		
		m_bRunning = true;
		m_bReady = false;
		g_spInputMutex = shoot::Mutex::Create();
        context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];		

        if (!context || ![EAGLContext setCurrentContext:context])
        {
            [self release];
            return nil;
        }
		
		// set resource paths
		const char* strResourcePath = [[[NSBundle mainBundle] resourcePath] UTF8String];
		NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
		NSString *documentsDirectory = [paths objectAtIndex:0];
		const char* strWritablePath = [documentsDirectory UTF8String];
		shoot::File::SetBasePath(strResourcePath);
		shoot::File::SetWritablePath(strWritablePath);

		m_pEngine = new shoot::GameEngine(&RegisterObjects, shoot::GraphicsDriver::DT_OpenGLES_1_1, shoot::InputManager::IMT_Touch);
		m_pEngine->Init();
												
        // Create important OpenGL ES buffers
        glGenFramebuffersOES(1, &defaultFramebuffer);
        glGenRenderbuffersOES(1, &colorRenderbuffer);
		glGenRenderbuffersOES(1, &depthRenderbuffer);
        glBindFramebufferOES(GL_FRAMEBUFFER_OES, defaultFramebuffer);
        glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);
        glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, colorRenderbuffer);
        
		m_PreviousTime = CFAbsoluteTimeGetCurrent();
		
		/*shoot::GameThreadiOS::Info threadInfo;
		threadInfo.frameBufferID = defaultFramebuffer;
		threadInfo.colorBufferID = colorRenderbuffer;
		threadInfo.pSetContextCallback = &SetContextCallback;
		threadInfo.pGetDeltaTimeCallback = &GetDeltaTimeCallback;
		threadInfo.pPresentCallback = &PresentCallback;
		threadInfo.pApplyInputCallback = &ApplyInputCallback;
		threadInfo.pIsRunningCallback = &IsRunningCallback;	
		shoot::GameThreadiOS::Start(threadInfo);*/
	}
	
    return self;
}

- (void)resetDeltaTime
{
   m_PreviousTime = CFAbsoluteTimeGetCurrent();
}

- (void)render
{
	if(m_bReady)
	{
        //shoot::GameThreadiOS::m_spSemaphore->Post();    
        glBindFramebufferOES(GL_FRAMEBUFFER_OES, defaultFramebuffer);
        glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);
        shoot::f32 fDeltaTime = GetDeltaTimeCallback();
        ApplyInputCallback();
        m_pEngine->Update(fDeltaTime);
        m_pEngine->Render();        
        [context presentRenderbuffer:GL_RENDERBUFFER_OES];		
	}
}

- (BOOL)resizeFromLayer:(CAEAGLLayer *)layer
{	
    // Allocate color buffer backing based on the current layer size
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);
    [context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:layer];
	
	GLint width, height;
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &width);
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &height);
		
	// Setup and attach depth buffer
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, depthRenderbuffer);
	glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, width, height);
	glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, depthRenderbuffer);
	
    if (glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES)
    {
        NSLog(@"Failed to make complete framebuffer object %x", glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
        return NO;
    }	
    
	m_pEngine->ResizeScreen(shoot::Size(width, height));
    
    [EAGLContext setCurrentContext:context];
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, defaultFramebuffer);
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    [context presentRenderbuffer:GL_RENDERBUFFER_OES];
    
	m_bReady = true;
    return YES;
}

- (void)dealloc
{
	m_bRunning = false;
	m_bReady = false;
	delete g_spInputMutex;
	delete m_pEngine;
	
	shoot::MemoryManager::LeakCheck();
	
    // Tear down GL
    if (defaultFramebuffer)
    {
        glDeleteFramebuffersOES(1, &defaultFramebuffer);
        defaultFramebuffer = 0;
    }

    if (colorRenderbuffer)
    {
        glDeleteRenderbuffersOES(1, &colorRenderbuffer);
        colorRenderbuffer = 0;
    }
	
	if (depthRenderbuffer)
    {
        glDeleteRenderbuffersOES(1, &depthRenderbuffer);
        depthRenderbuffer = 0;
    }

    // Tear down context
    if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];

    [context release];
    context = nil;

    [super dealloc];
}

@end
