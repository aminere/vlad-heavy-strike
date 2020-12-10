//
//  ESRenderer.h
//  ShootTest
//
//  Created by Amine Rehioui on 10-08-04.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>

#import <OpenGLES/EAGL.h>
#import <OpenGLES/EAGLDrawable.h>


@protocol ESRenderer <NSObject>

- (void)render;
- (BOOL)resizeFromLayer:(CAEAGLLayer *)layer;
- (void)resetDeltaTime;

@end
