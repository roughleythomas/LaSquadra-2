//
//  Copyright © Borna Noureddin. All rights reserved.
//

#ifndef GLESRenderer_hpp
#define GLESRenderer_hpp

#include <stdlib.h>
#include <chrono>
#include <vector>

#include <OpenGLES/ES3/gl.h>

#include "glm-master/glm/ext.hpp"
#include "Renderable.hpp"
#include "GOController.hpp"

using namespace std;

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
    void addObject(Renderable* r);

private:
    int vpWidth, vpHeight;  // viewport width/height
    bool updatedOnce = false;

    // GL variables
    GLint uniforms[NUM_UNIFORMS];
    GLuint programObject;
    GLuint crateTexture;
    glm::mat4 mvp;
    glm::mat3 normalMatrix;

    // model variables
    vector<GOController> objects;

    // used to calculate elapsed time between frames
    std::chrono::time_point<std::chrono::steady_clock> lastTime;

    // angle of rotation of cube
    float rotAngle;

    void LoadModels();

    bool SetupShaders(const char *vertexShaderFile, const char *fragmentShaderFile);
    GLuint SetupTexture(GLubyte *spriteData, size_t width, size_t height);

    char *LoadShaderFile(const char *shaderFileName);
    GLuint LoadShader(GLenum type, const char *shaderSrc);
    GLuint LoadProgram(const char *vertShaderSrc, const char *fragShaderSrc);
};

#endif /* GLESRenderer_hpp */
