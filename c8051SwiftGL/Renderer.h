//
//  Copyright © Borna Noureddin. All rights reserved.
//

#ifndef Renderer_h
#define Renderer_h
#import <GLKit/GLKit.h>

@interface Renderer : NSObject

// To give an interface to Swift for variables, they need to be Objective-C properties. You can add additional ones, such as a toggle variable for auto-rotating a cube.
@property bool isRed;
@property bool isRotating;
@property double rotX;
@property double rotY;
@property double panX;
@property double panY;
@property double scale;

- (void)setup:(GLKView *)view;
- (void)loadModels;
- (void)update;
- (void)draw:(CGRect)drawRect;

@end

#endif /* Renderer_h */


