//
//  Sphere.cpp
//  c8051SwiftGL
//
//  Created by Michael Sinclair on 2022-03-16.
//

#include "Sphere.hpp"

Sphere::Sphere(float radius, int stackCount, int sectorCount)
:Drawable(24, 24, 24, 36)
{
    this->radius = radius;
    this->stackCount = stackCount;
    this->sectorCount = sectorCount;
    loadAll();
    
    //loadVertices(0);
    //loadNormals(0);
    //loadTextureCoords(0);
    //loadIndices(0);
}

void Sphere::loadAll(){
    
}

/*bool Cube::loadVertices(void* info)
{
    for(int i = 0; i < numVertices * 3; i++)
    {
        vertices[i] = cubeVerts[i];
    }
    
    return true;
}

bool Cube::loadNormals(void* info)
{
    for(int i = 0; i < numNormals * 3; i++)
    {
        normals[i] = cubeNormals[i];
    }
    
    return true;
}

bool Cube::loadTextureCoords(void* info)
{
    for(int i = 0; i < numTexCoords * 2; i++)
    {
        texCoords[i] = cubeTex[i];
    }
    
    return true;
}

bool Cube::loadIndices(void* info)
{
    for(int i = 0; i < numIndices; i++)
    {
        indices[i] = cubeIndices[i];
    }
    
    return true;
}*/
