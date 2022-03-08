//
//  Copyright © Borna Noureddin. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <iostream>
#include "glm-master/glm/gtx/string_cast.hpp"
#include "GLESRenderer.hpp"
#include "CubeRender.hpp"


// ----------------------------------------------------------------
// Constructor and destructor
// ----------------------------------------------------------------
GLESRenderer::GLESRenderer(const char *vertexShaderFile, const char *fragmentShaderFile,
                           GLubyte *spriteData, size_t width, size_t height)
{
    LoadModels();

    if (vertexShaderFile && fragmentShaderFile)
    {
        if (!SetupShaders(vertexShaderFile, fragmentShaderFile))
            return;
    }

    if (spriteData && (width > 0) && (height > 0))
    {
        groundTexture = SetupTexture(spriteData, width, height);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, groundTexture);
        glUniform1i(uniforms[UNIFORM_TEXTURE], 0);
    }

    glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );
    glEnable(GL_DEPTH_TEST);

    rotAngle = 0.0f;
    isRotating = true;

    lastTime = std::chrono::steady_clock::now();
}

GLESRenderer::~GLESRenderer()
{
    glDeleteProgram(programObject);
}

// ----------------------------------------------------------------
// Update and Draw public methods
// ----------------------------------------------------------------
void GLESRenderer::Update()
{
    if(!updatedOnce){
        updatedOnce = true;
        
        /*
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();
        lastTime = currentTime;
        
        if (isRotating)
        {
            rotAngle += 0.001f * elapsedTime;
            if (rotAngle >= 360.0f)
                rotAngle = 0.0f;
        }
        */
        rotAngle = 0.f;

        //update view transform
        mvp = glm::translate(glm::mat4(1.0), glm::vec3(0, 0, -5));
        mvp = glm::rotate(mvp, rotAngle, glm::vec3(1, 0, 0));
        normalMatrix = glm::inverseTranspose(glm::mat3(mvp));
        float aspect = (float)vpWidth / (float)vpHeight;
        glm::mat4 perspective = glm::perspective(60.0f * glm::pi<float>() / 180.f, aspect, 1.0f, 20.0f);
        mvp = perspective * mvp;
    }
}

void GLESRenderer::Draw()
{
    glUniformMatrix4fv(uniforms[UNIFORM_MODELVIEWPROJECTION_MATRIX], 1, GL_FALSE, glm::value_ptr(mvp));
    glUniformMatrix3fv(uniforms[UNIFORM_NORMAL_MATRIX], 1, GL_FALSE, glm::value_ptr(normalMatrix));
    glUniform1i(uniforms[UNIFORM_PASSTHROUGH], false);
    glUniform1i(uniforms[UNIFORM_SHADEINFRAG], true);

    glViewport(0, 0, vpWidth, vpHeight);
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glUseProgram ( programObject );

    for(GOController object : objects){
        Renderable* r = object.getRenderable();
        float *vertices = r->getVertices(), *normals = r->getNormals(), *texCoords = r->getTextureCoords();
        int *indices = r->getIndices(), numIndices = r->getNumIndices();
        
        glVertexAttribPointer ( 0, 3, GL_FLOAT,
                               GL_FALSE, 3 * sizeof ( GLfloat ), vertices );
        glEnableVertexAttribArray ( 0 );

        glVertexAttrib4f ( 1, 1.0f, 0.0f, 0.0f, 1.0f );

        glVertexAttribPointer ( 2, 3, GL_FLOAT,
                               GL_FALSE, 3 * sizeof ( GLfloat ), normals );
        glEnableVertexAttribArray ( 2 );

        glVertexAttribPointer ( 3, 2, GL_FLOAT,
                               GL_FALSE, 2 * sizeof ( GLfloat ), texCoords );
        glEnableVertexAttribArray ( 3 );
        
        //object's properties
        glm::mat4 position = glm::translate(mvp, object.transform->getPosition())
            * glm::mat4(object.transform->getQuaternion());
        glm::mat4 transform = glm::scale(position, object.transform->getScale());
        
        glUniformMatrix4fv(uniforms[UNIFORM_MODELVIEWPROJECTION_MATRIX], 1, GL_FALSE, glm::value_ptr(transform));
        glDrawElements ( GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, indices );
    }
}

void GLESRenderer::addObject(Renderable* r){
    objects.push_back(GOController(new GameObject(), r));
}


// ========================================================================================


// ----------------------------------------------------------------
// Model loading - ADD RENDERABLES HERE
// ----------------------------------------------------------------
void GLESRenderer::LoadModels()
{
    addObject(new CubeRender());
    objects[0].transform->translate(glm::vec3(0.5, 1, 0));
    objects[0].transform->setScale(glm::vec3(1.f, 0.5f, 0.5f));
    /*Maze* maze = new Maze(5);
    maze->print();*/
}

// ========================================================================================


// ----------------------------------------------------------------
// Shader loading
// ----------------------------------------------------------------
bool GLESRenderer::SetupShaders(const char *vertexShaderFile, const char *fragmentShaderFile)
{
    // Load shaders
    char *vShaderStr = LoadShaderFile(vertexShaderFile);
    char *fShaderStr = LoadShaderFile(fragmentShaderFile);
    programObject = LoadProgram(vShaderStr, fShaderStr);
    if (programObject == 0)
        return false;
    
    // Set up uniform variables
    uniforms[UNIFORM_MODELVIEWPROJECTION_MATRIX] = glGetUniformLocation(programObject, "modelViewProjectionMatrix");
    uniforms[UNIFORM_NORMAL_MATRIX] = glGetUniformLocation(programObject, "normalMatrix");
    uniforms[UNIFORM_PASSTHROUGH] = glGetUniformLocation(programObject, "passThrough");
    uniforms[UNIFORM_SHADEINFRAG] = glGetUniformLocation(programObject, "shadeInFrag");
    uniforms[UNIFORM_TEXTURE] = glGetUniformLocation(programObject, "texSampler");

    return true;
}

GLuint GLESRenderer::SetupTexture(GLubyte *spriteData, size_t width, size_t height)
{
    GLuint texName;
    glGenTextures(1, &texName);
    glBindTexture(GL_TEXTURE_2D, texName);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, spriteData);
    
    return texName;
}

char *GLESRenderer::LoadShaderFile(const char *shaderFileName)
{
    FILE *fp = fopen(shaderFileName, "rb");
    if (fp == NULL)
        return NULL;

    fseek(fp , 0 , SEEK_END);
    long totalBytes = ftell(fp);
    fclose(fp);

    char *buf = (char *)malloc(totalBytes+1);
    memset(buf, 0, totalBytes+1);

    fp = fopen(shaderFileName, "rb");
    if (fp == NULL)
        return NULL;

    size_t bytesRead = fread(buf, totalBytes, 1, fp);
    fclose(fp);
    if (bytesRead < 1)
        return NULL;

    return buf;
}

GLuint GLESRenderer::LoadShader(GLenum type, const char *shaderSrc)
{
    GLuint shader = glCreateShader(type);
    if (shader == 0)
        return 0;
    
    glShaderSource(shader, 1, &shaderSrc, NULL);
    glCompileShader(shader);

    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled)
    {
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1)
        {
            char *infoLog = (char *)malloc(sizeof ( char ) * infoLen);
            glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
            std::cerr << "*** SHADER COMPILE ERROR:" << std::endl;
            std::cerr << infoLog << std::endl;
            free(infoLog);
        }
        glDeleteShader ( shader );
        return 0;
    }
    
    return shader;
}

GLuint GLESRenderer::LoadProgram(const char *vertShaderSrc, const char *fragShaderSrc)
{
    GLuint vertexShader = LoadShader(GL_VERTEX_SHADER, vertShaderSrc);
    if (vertexShader == 0)
        return 0;
    
    GLuint fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fragShaderSrc);
    if (fragmentShader == 0)
    {
        glDeleteShader(vertexShader);
        return 0;
    }
    
    GLuint programObject = glCreateProgram();
    if (programObject == 0)
    {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return 0;
    }
    
    glAttachShader(programObject, vertexShader);
    glAttachShader(programObject, fragmentShader);
    glLinkProgram(programObject);
    
    GLint linked;
    glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
    if (!linked)
    {
        GLint infoLen = 0;
        glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1)
        {
            char *infoLog = (char *)malloc(sizeof(char) * infoLen);
            glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
            std::cerr << "*** SHADER LINK ERROR:" << std::endl;
            std::cerr << infoLog << std::endl;
            free(infoLog);
        }
        glDeleteProgram(programObject);
        return 0;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return programObject;
}
