//
//  EAGLView.m
//  ShootTest
//
//  Created by Amine Rehioui on 10-08-04.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "EAGLView.h"

#import "ES1Renderer.h"

#include "Engine.h"
#include "InputManager.h"
#include "Mutex.h"
#include "GameManager.h"

extern shoot::InputManager::TouchState g_TouchState;
extern shoot::Mutex* g_spInputMutex;

@implementation EAGLView

@synthesize animating;
@synthesize multipleTouchEnabled;

// You must implement this method
+ (Class)layerClass
{
    return [CAEAGLLayer class];
}

//The EAGL view is stored in the nib file. When it's unarchived it's sent -initWithCoder:
- (id)initWithCoder:(NSCoder*)coder
{    
    if ((self = [super initWithCoder:coder]))
    {
        // Get the layer
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;

        eaglLayer.opaque = TRUE;
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];

        renderer = [[ES1Renderer alloc] init];
       
        animating = FALSE;
        multipleTouchEnabled = FALSE;
        animationTimer = nil;
        currentTouch = nil;
    }

    return self;
}

- (void)drawView:(id)sender
{
	if(!animating)
	{
		return;
	}

    [renderer render];
    
    if(shoot::InputManager::Instance()->IsVKeyboardRequested())
    {
        if(!self.isFirstResponder)
        {
            m_VKeyboardText.clear();
            [self becomeFirstResponder];
        }
    }
    else
    {
        if(self.isFirstResponder)
        {
            [self resignFirstResponder];
        }
    }
    
    if(shoot::GameManager::Instance()->HasGetFullGameRequest())
    {
        [[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"https://itunes.apple.com/app/id741421913?ls=1&mt=8"]];
                                                    
        shoot::GameManager::Instance()->ResetGetFullGameRequest();
    }
}

- (void)layoutSubviews
{
    [renderer resizeFromLayer:(CAEAGLLayer*)self.layer];
    [self drawView:nil];
}

- (void)startAnimation
{    
    if (!animating)
    {
		animationTimer = [NSTimer scheduledTimerWithTimeInterval:(NSTimeInterval)((1.0/40.0)) target:self selector:@selector(drawView:) userInfo:nil repeats:TRUE];
        animating = TRUE;
        
        shoot::Engine::Instance()->OnResume();
        [renderer resetDeltaTime];
    }
}

- (void)stopAnimation
{    
    if (animating)
    {
        animating = FALSE;        
		[animationTimer invalidate];
		animationTimer = nil;
        
        shoot::Engine::Instance()->OnPause();
    }
}

- (void)dealloc
{
    [renderer release];
    [super dealloc];
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch* touch = [[event allTouches] anyObject];    
    if(currentTouch) return;
    
	currentTouch = touch;    
	CGPoint location = [currentTouch locationInView:currentTouch.view];
	g_spInputMutex->Lock();
	g_TouchState.bPressed = true;
	g_TouchState.vPosition.Set(location.x, location.y);
	g_spInputMutex->Unlock();
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    if(currentTouch)
    {
        currentTouch = nil;
        CGPoint location = [currentTouch locationInView:currentTouch.view];
        g_spInputMutex->Lock();
        g_TouchState.bPressed = false;
        g_TouchState.vPosition.Set(location.x, location.y);
        g_spInputMutex->Unlock();
    }   
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    if(currentTouch)
    {
        currentTouch = nil;
        CGPoint location = [currentTouch locationInView:currentTouch.view];
        g_spInputMutex->Lock();
        g_TouchState.bPressed = false;
        g_TouchState.vPosition.Set(location.x, location.y);
        g_spInputMutex->Unlock();        
    }
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch* touch = [[event allTouches] anyObject];
    if(!currentTouch || touch != currentTouch) return;
	
	CGPoint location = [currentTouch locationInView:currentTouch.view];
	g_spInputMutex->Lock();
	g_TouchState.bPressed = true;
	g_TouchState.vPosition.Set(location.x, location.y);
	g_spInputMutex->Unlock();
}

- (void)insertText:(NSString *)text
{
	if(shoot::InputManager::Instance()->HasVKClearFlag())
	{
		m_VKeyboardText.clear();
		shoot::InputManager::Instance()->ResetVKClearFlag();
	}
	
	const char* strText = [text UTF8String];
    std::string stdText(strText);
    if(stdText == "\n")
	{
		shoot::InputManager::Instance()->OnVKeyboardUpdated(m_VKeyboardText, true);
	}
	else
    {
        m_VKeyboardText += stdText;
        shoot::InputManager::Instance()->OnVKeyboardUpdated(m_VKeyboardText, false);
    }    
}

- (void)deleteBackward
{
	if(shoot::InputManager::Instance()->HasVKClearFlag())
	{
		m_VKeyboardText.clear();
		shoot::InputManager::Instance()->ResetVKClearFlag();
	}
	
    int length = m_VKeyboardText.length();
    if(length > 0)
    {
        m_VKeyboardText = m_VKeyboardText.substr(0, length-1);
        shoot::InputManager::Instance()->OnVKeyboardUpdated(m_VKeyboardText, false);
    }
}

- (BOOL)hasText
{    
    return YES;
}

- (BOOL)canBecomeFirstResponder
{
    return YES;
}

@end
