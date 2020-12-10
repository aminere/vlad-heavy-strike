//
//  EAGLView.h
//  ShootTest
//
//  Created by Amine Rehioui on 10-08-04.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>

#import "ESRenderer.h"

// This class wraps the CAEAGLLayer from CoreAnimation into a convenient UIView subclass.
// The view content is basically an EAGL surface you render your OpenGL scene into.
// Note that setting the view non-opaque will only work if the EAGL surface has an alpha channel.
@interface EAGLView : UIView <UIKeyInput>
{    
@private
    id <ESRenderer> renderer;
    BOOL animating;    
    NSTimer *animationTimer;
    UITouch *currentTouch;
	
    std::string m_VKeyboardText;
}

@property (readonly, nonatomic, getter=isAnimating) BOOL animating;
@property (nonatomic, getter=isMultipleTouchEnabled) BOOL multipleTouchEnabled;

- (void)startAnimation;
- (void)stopAnimation;
- (void)drawView:(id)sender;

@end
