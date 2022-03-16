//
//  Copyright © Borna Noureddin. All rights reserved.
//

#ifndef GLESRenderer_hpp
#define GLESRenderer_hpp

#include <stdlib.h>
#include <chrono>
#include <vector>
#include <cmath>

#include <OpenGLES/ES3/gl.h>

#include "glm-master/glm/ext.hpp"
#include "glm-master/glm/gtc/quaternion.hpp"
#include "glm-master/glm/gtx/quaternion.hpp"
#include "Drawable.hpp"
#include "Maze.hpp"
#include "Cube.hpp"
#include "Sphere.hpp"

using namespace std;
using namespace glm;

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
    bool isRotating;// publicly available member variable (in this case whether the cube is auto-rotating
    float panX = 0, panY = 0;

    // Constructor and destructor
    GLESRenderer(const char *vertexShaderFile = NULL, const char *fragmentShaderFile = NULL,
                 /*GLubyte *spriteData = 0, size_t width = -1, size_t height = -1,*/ GLubyte **spriteData = 0, size_t *width = 0, size_t *height = 0);
    ~GLESRenderer();
    
    void SetViewport(int width, int height) { vpWidth = width; vpHeight = height; }
    void Update();
    void updateTransform();
    void Draw();
    void addDrawable(Drawable* d);
    void addWall(bool, float, float, float, int = 1);
    void reset();

private:
    int vpWidth, vpHeight;  // viewport width/height
    bool updatedOnce = false;

    // GL variables
    GLint uniforms[NUM_UNIFORMS];
    GLuint programObject;
    GLuint groundTexture;
    glm::mat4 mvp;
    glm::mat3 normalMatrix;
    glm::vec3 cameraPos, cameraAngles;

    // model variables
    vector<Drawable*> objects;
    vector<GLuint> textureIds;

    // used to calculate elapsed time between frames
    std::chrono::time_point<std::chrono::steady_clock> lastFrame;

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
