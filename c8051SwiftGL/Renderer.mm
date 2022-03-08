//
//  Copyright © Borna Noureddin. All rights reserved.
//

#import "Renderer.h"
#import <Foundation/Foundation.h>
#import <GLKit/GLKit.h>
#include <chrono>
#include "GLESRenderer.hpp"

@interface Renderer () {
    GLKView *theView;               // to access some iOS-specific parameters needed to set up OpenGL
    GLESRenderer *glesRenderer;     // the OpenGL ES C++ render class
}

@end

@implementation Renderer

@synthesize isRotating; // each public property needs to be synthesized

- (void)dealloc
{
    delete glesRenderer;    // free up the memory of the GLESRenderer object
}

- (void)setup:(GLKView *)view
{
    // These lines of code set up the OpenGL context using Objective-C/iOS code
    view.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    if (!view.context) {
        NSLog(@"Failed to create ES context");
    }
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    theView = view;
    [EAGLContext setCurrentContext:view.context];

    // The next lines of code also use the Core Graphics library in iOS to read in a
    //  texture image so that it can be passed on to the C++ class.
    // This code could be moved into the C++ class, which would then need to
    //  load in the image (possibly using some other API/library).
    // Note in that case the need still to construct the path to the file in the iOS bundle
    //  just like in the case of the shader files (see below).
    CGImageRef textureImage = [UIImage imageNamed:@"dirt.jpg"].CGImage;
    if (!textureImage) {
        NSLog(@"Failed to load image %@", @"dirt.jpg");
        exit(1);
    }
    size_t textureWidth = CGImageGetWidth(textureImage);
    size_t textureHeight = CGImageGetHeight(textureImage);
    GLubyte *textureData = (GLubyte *) calloc(textureWidth*textureHeight*4, sizeof(GLubyte));
    CGContextRef textureContext = CGBitmapContextCreate(textureData, textureWidth, textureHeight, 8, textureWidth*4, CGImageGetColorSpace(textureImage), kCGImageAlphaPremultipliedLast);
    CGContextDrawImage(textureContext, CGRectMake(0, 0, textureWidth, textureHeight), textureImage);
    CGContextRelease(textureContext);
    
    // Construct the C++ object, passing in the path to the shader files and the one texture data
    glesRenderer = new GLESRenderer([[[NSBundle mainBundle] pathForResource:[[NSString stringWithUTF8String:"Shader.vsh"] stringByDeletingPathExtension] ofType:[[NSString stringWithUTF8String:"Shader.vsh"] pathExtension]] cStringUsingEncoding:1],
                                    [[[NSBundle mainBundle] pathForResource:[[NSString stringWithUTF8String:"Shader.fsh"] stringByDeletingPathExtension] ofType:[[NSString stringWithUTF8String:"Shader.fsh"] pathExtension]] cStringUsingEncoding:1],
                                    textureData, textureWidth, textureHeight);
    isRotating = glesRenderer->isRotating;

    // Once we have passed on the data from the image file for the texture, we can free up the memory
    free(textureData);
}

- (void)update
{
    glesRenderer->SetViewport(theView.drawableWidth, theView.drawableHeight);
    glesRenderer->isRotating = isRotating;
    glesRenderer->Update();
}

- (void)draw:(CGRect)drawRect;
{
    glesRenderer->Draw();
}

@end
