//
//  Copyright © Borna Noureddin. All rights reserved.
//

#ifndef GLESRenderer_hpp
#define GLESRenderer_hpp

#include <stdlib.h>
#include <chrono>

#include <OpenGLES/ES3/gl.h>

#include "glm/ext.hpp"

// Uniform variables
enum
{
    UNIFORM_MODELVIEWPROJECTION_MATRIX,
    UNIFORM_NORMAL_MATRIX,
    UNIFORM_PASSTHROUGH,
    UNIFORM_SHADEINFRAG,
    UNIFORM_TEXTURE,
    NUM_UNIFORMS
};

// Attributes
enum
{
    ATTRIB_VERTEX,
    ATTRIB_NORMAL,
    NUM_ATTRIBUTES
};

class GLESRenderer
{
public:
    bool isRotating;    // publicly available member variable (in this case whether the cube is auto-rotating

    // Constructor and destructor
    GLESRenderer(const char *vertexShaderFile = NULL, const char *fragmentShaderFile = NULL,
                 GLubyte *spriteData = 0, size_t width = -1, size_t height = -1);
    ~GLESRenderer();
    
    void SetViewport(int width, int height) { vpWidth = width; vpHeight = height; }
    void Update();
    void Draw();

private:
    int vpWidth, vpHeight;  // viewport width/height

    // GL variables
    GLint uniforms[NUM_UNIFORMS];
    GLuint programObject;
    GLuint crateTexture;
    glm::mat4 mvp;
    glm::mat3 normalMatrix;

    // model variables
    float *vertices, *normals, *texCoords;
    int *indices, numIndices;

    // used to calculate elapsed time between frames
    std::chrono::time_point<std::chrono::steady_clock> lastTime;

    // angle of rotation of cube
    float rotAngle;

    void LoadModels();
    int GenCube(float scale, float **vertices, float **normals,
                float **texCoords, int **indices);
    int GenSquare(float scale, float **vertices, int **indices);

    bool SetupShaders(const char *vertexShaderFile, const char *fragmentShaderFile);
    GLuint SetupTexture(GLubyte *spriteData, size_t width, size_t height);

    char *LoadShaderFile(const char *shaderFileName);
    GLuint LoadShader(GLenum type, const char *shaderSrc);
    GLuint LoadProgram(const char *vertShaderSrc, const char *fragShaderSrc);
};

#endif /* GLESRenderer_hpp */
