//
//  Copyright © Borna Noureddin. All rights reserved.
//

#import "Renderer.h"
#import <Foundation/Foundation.h>
#import <GLKit/GLKit.h>
#import "Enemy.hpp"
#include <chrono>
#include "GLESRenderer.hpp"
#include <cstring>

@interface Renderer () {
    GLKView *theView;               // to access some iOS-specific parameters needed to set up OpenGL
    GLESRenderer *glesRenderer;     // the OpenGL ES C++ render class
    
    Enemy *enemy;
    
    bool    _drawEnemy;
    float   _enemyXPosition;
    float   _enemyYPosition;
    float   _enemyZPosition;
    float   _enemyXRotation;
    float   _enemyYRotation;
    float   _enemyZRotation;
    float   _enemyXScale;
    float   _enemyYScale;
    float   _enemyZScale;
    
}

@property (strong, nonatomic) GLKBaseEffect* effect;

@end

@implementation Renderer

@synthesize isRotating; // each public property needs to be synthesized
@synthesize panX;
@synthesize panY;
@synthesize score;

- (void)dealloc
{
    delete glesRenderer;    // free up the memory of the GLESRenderer object
}

- (void)setup:(GLKView *)view
{
    
    _drawEnemy = true;
    _enemyXPosition = 0.0f;
    _enemyYPosition = 0.0f;
    _enemyZPosition = 0.0f;
    _enemyXRotation = 0.0f;
    _enemyYRotation = 0.0f;
    _enemyZRotation = 0.0f;
    _enemyXScale = 0.0f;
    _enemyYScale = 0.0f;
    _enemyZScale = 0.0f;
    enemy = new Enemy();
    
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
    vector<NSString*> textureNames;
    textureNames.push_back(@"dirt.jpg");
    textureNames.push_back(@"iron.jpg");
    textureNames.push_back(@"coin.jpg");
    textureNames.push_back(@"ExitPortal.png");
    vector<GLubyte*> textureDataList;
    vector<size_t> textureWidthList, textureHeightList;
    
    for(int i = 0; i < textureNames.size(); i++){
        CGImageRef textureImage = [UIImage imageNamed:textureNames[i]].CGImage;
        if (!textureImage) {
            NSLog(@"Failed to load image %@", textureNames[i]);
            exit(1);
        }
        //size_t textureWidth = CGImageGetWidth(textureImage);
        //size_t textureHeight = CGImageGetHeight(textureImage);
        textureWidthList.push_back(CGImageGetWidth(textureImage));
        textureHeightList.push_back(CGImageGetHeight(textureImage));
        GLubyte *textureData = (GLubyte *) calloc(textureWidthList[i]*textureHeightList[i]*4, sizeof(GLubyte));
        CGContextRef textureContext = CGBitmapContextCreate(textureData, textureWidthList[i], textureHeightList[i], 8, textureWidthList[i]*4, CGImageGetColorSpace(textureImage), kCGImageAlphaPremultipliedLast);
        CGContextDrawImage(textureContext, CGRectMake(0, 0, textureWidthList[i], textureHeightList[i]), textureImage);
        CGContextRelease(textureContext);
        textureDataList.push_back(textureData);
    }
  
    // Construct the C++ object, passing in the path to the shader files and the one texture data
    glesRenderer = new GLESRenderer([[[NSBundle mainBundle] pathForResource:[[NSString stringWithUTF8String:"Shader.vsh"] stringByDeletingPathExtension] ofType:[[NSString stringWithUTF8String:"Shader.vsh"] pathExtension]] cStringUsingEncoding:1],
                                    [[[NSBundle mainBundle] pathForResource:[[NSString stringWithUTF8String:"Shader.fsh"] stringByDeletingPathExtension] ofType:[[NSString stringWithUTF8String:"Shader.fsh"] pathExtension]] cStringUsingEncoding:1],
                                    textureDataList.data(), textureWidthList.data(), textureHeightList.data());
    isRotating = glesRenderer->isRotating;
    panX = glesRenderer->panX;
    panY = glesRenderer->panY;
    score = 0;

    
    
    vector<NSString*> fontNames;
    fontNames.push_back(@"times");
    //fontNames.push_back(@"Vonique 64");
    //fontNames.push_back(@"arial");
    
    for(int i = 0; i < fontNames.size(); i++)
    {
        
        NSString *fontPath = [[NSBundle mainBundle] pathForResource:fontNames[i] ofType:@"ttf"];
        
        //if (!fontPath) {
          //  NSLog(@"Failed to load font %@", fontNames[i]);
        //}
        
        //glesRenderer->fonts.push_back((char *)[fontPath cStringUsingEncoding:NSUTF8StringEncoding]);
        
    }
    
    
    
    // Once we have passed on the data from the image file for the texture, we can free up the memory
    for(int i = 0; i < textureDataList.size(); i++)
        free(textureDataList[i]);
}

- (void)createEffect
{
    self.effect = [[GLKBaseEffect alloc] init];
    
    self.effect.light0.enabled = GL_TRUE;
    self.effect.light0.position = GLKVector4Make(1.0f, 1.0f, 1.0f, 1.0f);
    self.effect.lightingType = GLKLightingTypePerPixel;
    
    self.effect.light0.specularColor = GLKVector4Make(0.25f, 0.25f, 0.25f, 1.0f);
    self.effect.light0.diffuseColor = GLKVector4Make(0.75f, 0.75f, 0.75f, 1.0f);
}

- (void)update
{
    glesRenderer->SetViewport(theView.drawableWidth, theView.drawableHeight);
    glesRenderer->isRotating = isRotating;
    glesRenderer->panX = panX;
    glesRenderer->panY = panY;
    glesRenderer->Update();
    _enemyYRotation += 0.1f;
}

- (void)draw:(CGRect)drawRect;
{
    glesRenderer->Draw();
    
    if (_drawEnemy) {
        [self drawEnemy:_enemyXPosition :_enemyYPosition :_enemyZPosition :_enemyXRotation :_enemyYRotation :_enemyZRotation :_enemyXScale :_enemyYScale :_enemyZScale];
    }
}

- (void)reset
{
    glesRenderer->reset();
}

- (bool)achievedGoal {
    return glesRenderer->achievedGoal();
}

- (float)getGameTime {
    return glesRenderer->getGameTime();
}

- (void)setPlayerDir:(int)playerDir;
{
    glesRenderer->setPlayerDir(playerDir);
}

- (void)togglePlayerStationary{
    glesRenderer->togglePlayerStationary();
}

- (void)drawEnemy: (float)xPos: (float)yPos: (float)zPos: (float)xRot: (float)yRot: (float)zRot: (float)x_Scale: (float)y_Scale: (float)z_Scale
{
    // ModelView Matrix (Perform functions to change position, rotation and scale here)
    GLKMatrix4 modelViewMatrix = GLKMatrix4Identity;
    modelViewMatrix = GLKMatrix4Translate(modelViewMatrix, xPos, yPos, zPos); // For Position, Rotation and Scale use individual variables for each object.
    modelViewMatrix = GLKMatrix4RotateX(modelViewMatrix, GLKMathDegreesToRadians(xRot));
    modelViewMatrix = GLKMatrix4RotateY(modelViewMatrix, GLKMathDegreesToRadians(yRot));
    modelViewMatrix = GLKMatrix4RotateZ(modelViewMatrix, GLKMathDegreesToRadians(zRot));
    modelViewMatrix = GLKMatrix4Scale(modelViewMatrix, x_Scale, y_Scale, z_Scale);
    self.effect.transform.modelviewMatrix = modelViewMatrix;
    
    
    // Positions
    glEnableVertexAttribArray(GLKVertexAttribPosition);
    glVertexAttribPointer(GLKVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, 0, enemy->EnemyPositions);
    
    // Normals
    glEnableVertexAttribArray(GLKVertexAttribNormal);
    glVertexAttribPointer(GLKVertexAttribNormal, 3, GL_FLOAT, GL_FALSE, 0, enemy->EnemyNormals);
    
    for(int i=0; i<enemy->EnemyMaterials; i++)
    {
        // Set material
        self.effect.material.diffuseColor = GLKVector4Make(enemy->EnemyDiffuses[i][0], enemy->EnemyDiffuses[i][1], enemy->EnemyDiffuses[i][2], 1.0f);
        self.effect.material.specularColor = GLKVector4Make(enemy->EnemySpeculars[i][0], enemy->EnemySpeculars[i][1], enemy->EnemySpeculars[i][2], 1.0f);

        // Prepare effect
        [self.effect prepareToDraw];

        // Draw vertices
        glDrawArrays(GL_TRIANGLES, enemy->EnemyFirsts[i], enemy->EnemyCounts[i]);
    }
}

@end
