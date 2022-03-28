//
//  UITimer.cpp
//  c8051SwiftGL
//
//  Created by Kevin Vilanova on 2022-03-28.
//

#include "UITimer.hpp"

//Constructor for UITimer, which is a child of drawable.
//When the constructor is called, ensure you specify the textureID that the texture is stored within (based on the texture list in glesrenderer or renderer.mm),
//and the verts, normals, texture coords, and indices array size.
UITimer::UITimer(int textureListIndex):Drawable(textureListIndex, 24, 24, 24, 36)
{
    loadVertices(0);
    loadNormals(0);
    loadTextureCoords(0);
    loadIndices(0);
}

bool UITimer::loadVertices(void* info)
{
    float cubeVerts[] =
    {// X Y Z
        -1.f, -1.f, 0.0f,
        -1.f, -1.f,  0.0f,
        1.f, -1.f,  0.0f,
        1.f, -1.f, 0.0f,
    };
    
    for(int i = 0; i < numVertices * 3; i++)
    {
        //vertices[i] = cubeVerts[i];
        vertices.push_back(cubeVerts[i]);
    }
    
    return true;
}

bool UITimer::loadNormals(void* info)
{
    float cubeNormals[] =
    {
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
    };
    
    for(int i = 0; i < numNormals * 3; i++)
    {
        //normals[i] = cubeNormals[i];
        normals.push_back(cubeNormals[i]);
    }
    
    return true;
}

bool UITimer::loadTextureCoords(void* info)
{
    float cubeTex[] =
    {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
    };
    
    for(int i = 0; i < numTexCoords * 2; i++)
    {
        //texCoords[i] = cubeTex[i];
        texCoords.push_back(cubeTex[i]);
    }
    
    return true;
}

bool UITimer::loadIndices(void* info)
{
    int cubeIndices[] =
    {
        0, 2, 1,
        0, 3, 2,
    };
    
    for(int i = 0; i < numIndices; i++)
    {
        //indices[i] = cubeIndices[i];
        indices.push_back(cubeIndices[i]);
    }
    
    return true;
}
