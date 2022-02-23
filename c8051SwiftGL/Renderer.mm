//
//  Copyright © Borna Noureddin. All rights reserved.
//

#import "Renderer.h"
#import <Foundation/Foundation.h>
#import <GLKit/GLKit.h>
#include <chrono>
#include "GLESRenderer.hpp"

#include "Camera.hpp"
#include "CubeRender.hpp"
#include "GOController.hpp"

// These are GL indices for uniform variables used by GLSL shaders.
// You can add additional ones, for example for a normal matrix,
//  textures, toggles, or for any other data to pass on to the shaders.
enum
{
    UNIFORM_MODELVIEWPROJECTION_MATRIX,
    // ### insert additional uniforms here
    UNIFORM_NORMAL_MATRIX,
    UNIFORM_PASSTHROUGH,
    UNIFORM_SHADEINFRAG,
    UNIFORM_TEXTURE,
    NUM_UNIFORMS
};
GLint uniforms[NUM_UNIFORMS];

// These are GL indices for vertex attributes
//  (e.g., vertex normals, texture coordinates, etc.)
enum
{
    ATTRIB_VERTEX,
    // ### insert additional vertex data here
    ATTRIB_NORMAL,
    NUM_ATTRIBUTES
};

@interface Renderer () {
    GLKView *theView;   // used to access view properties (e.g., its size)
    GLESRenderer glesRenderer;  // our GLES renderer object
    GLuint programObject;   // GLSL shader program that has the vertex and fragment shaders
    std::chrono::time_point<std::chrono::steady_clock> lastTime;

    // GL variables to associate with uniforms
    GLKMatrix4 mvp;
    // ### add additional ones (e.g., texture IDs, normal matrices, etc.) here
    GLKMatrix3 normalMatrix;
    GLuint crateTexture;

    // GL vertex data (minimum X,Y,Z location)
    float *vertices;
    // ### add additional vertex data (e.g., vertex normals, texture coordinates, etc.) here
    float *normals, *texCoords;
    int *indices, numIndices;
    
    // ### add any other variables (e.g., current rotation angle for auto-rotating cube) here
    float rotAngle;
    
    CubeRender* cube;
    GOController* cubeController;
}

@end

@implementation Renderer

@synthesize isRed;
// ### add any other properties that need to be synthesized
@synthesize isRotating;
@synthesize rotX;
@synthesize rotY;
@synthesize panX;
@synthesize panY;
@synthesize scale;

- (void)dealloc
{
    glDeleteProgram(programObject);
}

- (void)loadModels
{
//    numIndices = glesRenderer.GenSquare(1.0f, &vertices, &indices);
    // ### instead of a cube, you can load any other model
    //numIndices = glesRenderer.GenCube(1.0f, &vertices, &normals, &texCoords, &indices);
}

- (void)setup:(GLKView *)view
{
    view.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    
    if (!view.context) {
        NSLog(@"Failed to create ES context");
    }
    
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    theView = view;
    [EAGLContext setCurrentContext:view.context];
    if (![self setupShaders])
        return;

    isRed = true;
    // ### any other properties and variables that need to be initialized (e.g., auto-rotation toggle value, initial rotation angle, etc.)
    rotAngle = 0.0f;
    rotX = 0.0;
    rotY = 0.0;
    isRotating = 1;
    scale = 1;

    // ### you should also load any textures needed here (you can use the setupTexture method below to load in a JPEG image and assign it to a GL texture)
    crateTexture = [self setupTexture:@"crate.jpg"];

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, crateTexture);
    glUniform1i(uniforms[UNIFORM_TEXTURE], 0);

    glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f ); // background color
    glEnable(GL_DEPTH_TEST);
    lastTime = std::chrono::steady_clock::now();
    
    //float aspect = (float)theView.drawableWidth / (float)theView.drawableHeight;
    float fov = 60.0f * M_PI / 180.0f;
    
    Camera* camera = Camera::GetInstance();
    camera->setFarZ(20.0f);
    camera->setNearZ(1.0f);
    //camera->setAspectRatio(aspect);
    camera->setFieldOfView(fov);
    camera->getTransform()->translate(glm::vec4(0, 0, -5.0f, 0));
    
    cube = new CubeRender();
    cubeController = new GOController(new GameObject(), cube);
    
    /*vertices = new float[24 * 3];
    normals = new float[24 * 3];
    texCoords = new float[24 * 2];
    indices = new int[36];*/
    //for(int i = 0; i < cube->getNumberOfVertices(); i++) { vertices[i] = cube->getVertices()[i]; }
    /*for(int i = 0; i < cube->getNumberOfNormals(); i++) { normals[i] = cube->getNormals()[i]; }
    for(int i = 0; i < cube->getNumberOfTexCoordinates(); i++) { texCoords[i] = cube->getTextureCoordinates()[i]; }
    for(int i = 0; i < cube->getNumberOfIndices(); i++) { indices[i] = cube->getIndices()[i]; }*/
}

- (void)update
{
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();
    lastTime = currentTime;
    
    Camera* camera = camera->GetInstance();
    // ### do any other updating (e.g., changing the rotation angle of an auto-rotating cube) here
    if (isRotating)
    {
        rotAngle += 0.001f * elapsedTime;
        if (rotAngle >= 360.0f)
            rotAngle = 0.0f;
    }

    // Set up a perspective view
    glm::vec4 position = camera->getTransform()->getPosition();
    mvp = GLKMatrix4Translate(GLKMatrix4Identity, position.x, position.y, position.z);
    //mvp = GLKMatrix4Translate(GLKMatrix4Identity, 0.0, 0.0, -5.0);
    // ### add any other transformations here (e.g., adding a rotation for a cube, or setting up a normal matrix for the shader)
    mvp = GLKMatrix4Translate(mvp, panX/80, panY/-80, 0.0);
    mvp = GLKMatrix4Scale(mvp, scale, scale, scale);
    

    if (isRotating) {
        mvp = GLKMatrix4Rotate(mvp, rotAngle, 0.0, 1.0 , 0.0);
    }
    else {
        bool isInvertible;
        if (rotX != 0.0){
            // multiply matrix by inverse to place in world space instead of object space and avoid gimble lock
            GLKVector3 xAxis = GLKMatrix4MultiplyVector3(GLKMatrix4Invert(mvp, &isInvertible), GLKVector3Make(0, 1, 0));
            // do the rotation
            mvp = GLKMatrix4Rotate(mvp, rotX/100, xAxis.x, xAxis.y, 0.0);
        }
        if (rotY != 0.0){
            // multiply matrix by inverse to place in world space instead of object space and avoid gimble lock
            GLKVector3 yAxis = GLKMatrix4MultiplyVector3(GLKMatrix4Invert(mvp, &isInvertible), GLKVector3Make(1, 0, 0));
            // do the rotation
            mvp = GLKMatrix4Rotate(mvp, rotY/100, yAxis.x, yAxis.y, 0.0);
        }
    }
    
    normalMatrix = GLKMatrix3InvertAndTranspose(GLKMatrix4GetMatrix3(mvp), NULL);

    //Camera* camera = Camera::GetInstance();
    
    float aspect = (float)theView.drawableWidth / (float)theView.drawableHeight;
    camera->setAspectRatio(aspect);
    //GLKMatrix4 perspective = GLKMatrix4MakePerspective(60.0f * M_PI / 180.0f, aspect, 1.0f, 20.0f);
    GLKMatrix4 perspective = GLKMatrix4MakePerspective(camera->getFieldOfView(), camera->getAspectRatio(), camera->getNearZ(), camera->getFarZ());
    
    mvp = GLKMatrix4Multiply(perspective, mvp);
}

- (void)draw:(CGRect)drawRect;
{
    vertices = cubeController->getRenderable()->getVertices();
    normals = cubeController->getRenderable()->getNormals();
    texCoords = cubeController->getRenderable()->getTextureCoordinates();
    indices = cubeController->getRenderable()->getIndices();
    numIndices = cubeController->getRenderable()->getNumberOfIndices();
    /*vertices = cube->getVertices();
    normals = cube->getNormals();
    texCoords = cube->getTextureCoordinates();
    indices = cube->getIndices();
    numIndices = cube->getNumberOfIndices();*/
    
    glUniformMatrix4fv(uniforms[UNIFORM_MODELVIEWPROJECTION_MATRIX], 1, FALSE, (const float *)mvp.m);
    // ### load any additional uniforms with relevant data here
    glUniformMatrix3fv(uniforms[UNIFORM_NORMAL_MATRIX], 1, 0, normalMatrix.m);
    glUniform1i(uniforms[UNIFORM_PASSTHROUGH], false);
    glUniform1i(uniforms[UNIFORM_SHADEINFRAG], true);

    glViewport(0, 0, (int)theView.drawableWidth, (int)theView.drawableHeight);
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glUseProgram ( programObject );

    glVertexAttribPointer ( 0, 3, GL_FLOAT,
                           GL_FALSE, 3 * sizeof ( GLfloat ), vertices);
    glEnableVertexAttribArray ( 0 );
    // ### set up and enable any additional vertex attributes (e.g., normals, texture coordinates, etc.) here

    glVertexAttrib4f ( 1, 1.0f, 0.0f, 0.0f, 1.0f );

    glVertexAttribPointer ( 2, 3, GL_FLOAT,
                           GL_FALSE, 3 * sizeof ( GLfloat ), normals );
    glEnableVertexAttribArray ( 2 );

    glVertexAttribPointer ( 3, 2, GL_FLOAT,
                           GL_FALSE, 2 * sizeof ( GLfloat ), texCoords );
    glEnableVertexAttribArray ( 3 );
    if (isRed)
        glVertexAttrib4f ( 1, 1.0f, 0.0f, 0.0f, 1.0f );
    else
        glVertexAttrib4f ( 1, 0.0f, 1.0f, 0.0f, 1.0f );
    glUniformMatrix4fv(uniforms[UNIFORM_MODELVIEWPROJECTION_MATRIX], 1, FALSE, (const float *)mvp.m);
    glDrawElements ( GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, indices );
}


- (bool)setupShaders
{
    // Load shaders
    char *vShaderStr = glesRenderer.LoadShaderFile([[[NSBundle mainBundle] pathForResource:[[NSString stringWithUTF8String:"Shader.vsh"] stringByDeletingPathExtension] ofType:[[NSString stringWithUTF8String:"Shader.vsh"] pathExtension]] cStringUsingEncoding:1]);
    char *fShaderStr = glesRenderer.LoadShaderFile([[[NSBundle mainBundle] pathForResource:[[NSString stringWithUTF8String:"Shader.fsh"] stringByDeletingPathExtension] ofType:[[NSString stringWithUTF8String:"Shader.fsh"] pathExtension]] cStringUsingEncoding:1]);
    programObject = glesRenderer.LoadProgram(vShaderStr, fShaderStr);
    if (programObject == 0)
        return false;
    
    // Set up uniform variables
    uniforms[UNIFORM_MODELVIEWPROJECTION_MATRIX] = glGetUniformLocation(programObject, "modelViewProjectionMatrix");
    // ### set up any additional uniform variables here
    uniforms[UNIFORM_NORMAL_MATRIX] = glGetUniformLocation(programObject, "normalMatrix");
    uniforms[UNIFORM_PASSTHROUGH] = glGetUniformLocation(programObject, "passThrough");
    uniforms[UNIFORM_SHADEINFRAG] = glGetUniformLocation(programObject, "shadeInFrag");
    uniforms[UNIFORM_TEXTURE] = glGetUniformLocation(programObject, "texSampler");

    return true;
}


// Load in and set up texture image (adapted from Ray Wenderlich)
- (GLuint)setupTexture:(NSString *)fileName
{
    CGImageRef spriteImage = [UIImage imageNamed:fileName].CGImage;
    if (!spriteImage) {
        NSLog(@"Failed to load image %@", fileName);
        exit(1);
    }
    
    size_t width = CGImageGetWidth(spriteImage);
    size_t height = CGImageGetHeight(spriteImage);
    
    GLubyte *spriteData = (GLubyte *) calloc(width*height*4, sizeof(GLubyte));
    
    CGContextRef spriteContext = CGBitmapContextCreate(spriteData, width, height, 8, width*4, CGImageGetColorSpace(spriteImage), kCGImageAlphaPremultipliedLast);
    
    CGContextDrawImage(spriteContext, CGRectMake(0, 0, width, height), spriteImage);
    
    CGContextRelease(spriteContext);
    
    GLuint texName;
    glGenTextures(1, &texName);
    glBindTexture(GL_TEXTURE_2D, texName);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, spriteData);
    
    free(spriteData);
    return texName;
}

@end

