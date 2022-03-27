#include "Cube.hpp"

Cube::Cube(int textureListIndex)
:Drawable(textureListIndex, 24, 24, 24, 36)
{
    loadVertices(0);
    loadNormals(0);
    loadTextureCoords(0);
    loadIndices(0);
}

bool Cube::loadVertices(void* info)
{
    float cubeVerts[] =
    {
        -1.f, -1.f, -1.f,
        -1.f, -1.f,  1.f,
        1.f, -1.f,  1.f,
        1.f, -1.f, -1.f,
        -1.f,  1.f, -1.f,
        -1.f,  1.f,  1.f,
        1.f,  1.f,  1.f,
        1.f,  1.f, -1.f,
        -1.f, -1.f, -1.f,
        -1.f,  1.f, -1.f,
        1.f,  1.f, -1.f,
        1.f, -1.f, -1.f,
        -1.f, -1.f, 1.f,
        -1.f,  1.f, 1.f,
        1.f,  1.f, 1.f,
        1.f, -1.f, 1.f,
        -1.f, -1.f, -1.f,
        -1.f, -1.f,  1.f,
        -1.f,  1.f,  1.f,
        -1.f,  1.f, -1.f,
        1.f, -1.f, -1.f,
        1.f, -1.f,  1.f,
        1.f,  1.f,  1.f,
        1.f,  1.f, -1.f,
    };
    
    for(int i = 0; i < numVertices * 3; i++)
    {
        //vertices[i] = cubeVerts[i];
        vertices.push_back(cubeVerts[i]);
    }
    
    return true;
}

bool Cube::loadNormals(void* info)
{
    float cubeNormals[] =
    {
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
    };
    
    for(int i = 0; i < numNormals * 3; i++)
    {
        //normals[i] = cubeNormals[i];
        normals.push_back(cubeNormals[i]);
    }
    
    return true;
}

bool Cube::loadTextureCoords(void* info)
{
    float cubeTex[] =
    {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
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

bool Cube::loadIndices(void* info)
{
    int cubeIndices[] =
    {
        0, 2, 1,
        0, 3, 2,
        4, 5, 6,
        4, 6, 7,
        8, 9, 10,
        8, 10, 11,
        12, 15, 14,
        12, 14, 13,
        16, 17, 18,
        16, 18, 19,
        20, 23, 22,
        20, 22, 21
    };
    
    for(int i = 0; i < numIndices; i++)
    {
        //indices[i] = cubeIndices[i];
        indices.push_back(cubeIndices[i]);
    }
    
    return true;
}
