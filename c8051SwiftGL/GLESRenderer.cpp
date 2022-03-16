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
    if(panX != 0 || panY != 0){
        auto curTime = chrono::steady_clock::now();
        auto elapsedTime = chrono::duration_cast<std::chrono::milliseconds>(curTime - lastFrame).count();
        float ratio = elapsedTime * 0.000005f;
        
        if(abs(panY) >= abs(panX)){
            cameraPos.x -= panY * ratio;
            //cameraPos.y -= panY * ratio * 0.3f;
        } else{
            float hyp = sqrt(pow(panX, 2) + pow(panY, 2));
            float asin = panX / hyp;
            cameraAngles.z -= asin;
        }
    }
    
    updateTransform();
    lastFrame = std::chrono::steady_clock::now();
}

void GLESRenderer::updateTransform(){
    for(Drawable* drawable : objects)
        drawable->updateTransform();
}

void GLESRenderer::Draw()
{
    glUniform1i(uniforms[UNIFORM_PASSTHROUGH], false);
    glUniform1i(uniforms[UNIFORM_SHADEINFRAG], true);

    glViewport(0, 0, vpWidth, vpHeight);
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glUseProgram ( programObject );

    mvp = translate(glm::mat4(1.0), cameraPos);
    //mvp = rotate(mvp, radians(cameraAngles.x), vec3(1, 0, 0));
    //mvp = rotate(mvp, radians(cameraAngles.y), vec3(0, 1, 0));
    //mvp = rotate(mvp, radians(cameraAngles.z), vec3(0, 0, 1));
    
    for(Drawable *drawable : objects){
        int *indices = drawable->getIndices(), numIndices = drawable->getNumIndices();
        mat4 transform = drawable->draw(mvp);
        
        //Update(&transform);
        normalMatrix = glm::inverseTranspose(glm::mat3(transform));
        float aspect = (float)vpWidth / (float)vpHeight;
        mat4 perspective = glm::perspective(60.0f * glm::pi<float>() / 180.f, aspect, 1.0f, 20.0f);
        perspective = glm::rotate(perspective, radians(cameraAngles.x), vec3(1, 0, 0));
        perspective = glm::rotate(perspective, radians(cameraAngles.y), vec3(0, 1, 0));
        perspective = glm::rotate(perspective, radians(cameraAngles.z), vec3(0, 0, 1));
        transform = perspective * transform;
        
        glUniformMatrix4fv(uniforms[UNIFORM_MODELVIEWPROJECTION_MATRIX], 1, GL_FALSE, value_ptr(transform));
        glUniformMatrix3fv(uniforms[UNIFORM_NORMAL_MATRIX], 1, GL_FALSE, glm::value_ptr(normalMatrix));
        glDrawElements ( GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, indices );
    }
}

void GLESRenderer::addDrawable(Drawable* d){
    objects.push_back(d);
}

void GLESRenderer::addWall(bool horizontal, float posX, float posY, float alternateScale){
    addDrawable(new Cube());
    int lindex = objects.size() - 1;
    objects[lindex]->setPosition(glm::vec3(posX, posY, 0.125f));
    if(horizontal)
        objects[lindex]->setScale(glm::vec3(alternateScale, 0.01f, 0.25f));
    else
        objects[lindex]->setScale(glm::vec3(0.01f, alternateScale, 0.25f));
}

void GLESRenderer::reset(){
    cameraAngles = vec3(-45.f, 0.f, 90.f);
    cameraPos = vec3(2.5f, 0, -1.75f);
}

// ========================================================================================


// ----------------------------------------------------------------
// Model loading - ADD RENDERABLES HERE
// ----------------------------------------------------------------
void GLESRenderer::LoadModels()
{
    reset();
    
    //floor
    
    addDrawable(new Cube());
    objects[0]->setScale(vec3(2.f, 2.f, 0.1f));
    
    float wallNum = 10;
    Maze* maze = new Maze(wallNum);
    maze->print();
    
    float sector = 2.f / wallNum;
    addWall(true, 0.f, 2.f, 2.f);
    addWall(false, -2.f, -sector, 2.f - sector);
    for(int i = 0; i < wallNum; i++){
        int wallTypeHor = ((i > 0) ? 1 : 2),
            wallTypeVer = ((i > 0) ? 0 : 1);
        
        for(int j = 0; j < wallNum; j++){
            if(!maze->maze[i * wallNum + j].getWallHidden(wallTypeHor))
                addWall(true, -2.f + 2 * sector * (j + 1) - sector, 2.f - 2 * sector * (i + 1), sector);
            if(!maze->maze[i * wallNum + j].getWallHidden(wallTypeVer))
                addWall(false, -2.f + 2 * sector * (j + 1), 2.f - 2 * sector * (i + 1) + sector, sector);
        }
    }
    
    addDrawable(new Sphere(0.15f, 10, 10));
    objects[objects.size() - 1]->setPosition(vec3(0, 0.15f, 0.5f));
    objects[objects.size() - 1]->assignAnimator(new Animator(vec3(0, 0, 0.000001f)));
    objects[objects.size() - 1]->anim->setEnabled(true);
    
    updateTransform();
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
