//
//  Sphere.cpp
//  c8051SwiftGL
//
//  Created by Michael Sinclair on 2022-03-16.
//

#include "Sphere.hpp"

#define PI 3.14159265

Sphere::Sphere(int textureListIndex, float radius, int stackCount, int sectorCount)
:Drawable(textureListIndex, 24, 24, 24, 36)
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
    float x, y, z, xy;
    float nx, ny, nz, lengthInv = 1.f / radius;
    float s, t;
    
    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;
    
    for(int i = 0; i <= stackCount; ++i){
        stackAngle = PI / 2 - i * stackStep;
        xy = radius * cosf(stackAngle);
        z = radius * sinf(stackAngle);
        
        for(int j = 0; j <= sectorCount; ++j){
            sectorAngle = j * sectorStep;
            
            x = xy * cosf(sectorAngle);
            y = xy * sinf(sectorAngle);
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
            
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            normals.push_back(nx);
            normals.push_back(ny);
            normals.push_back(nz);
            
            s = (float)j / sectorCount;
            t = (float)i / stackCount;
            texCoords.push_back(s);
            texCoords.push_back(t);
        }
    }
    
    //indices
    int k1, k2;
    for(int i = 0; i < stackCount; ++i){
        k1 = i * (sectorCount + 1);
        k2 = k1 + sectorCount + 1;
        
        for(int j = 0; j < sectorCount; ++j, ++k1, ++k2){
            if(i != 0){
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }
            
            if(i != (stackCount - 1)){
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
}

bool Sphere::loadVertices(void* info)
{
    /*for(int i = 0; i < numVertices * 3; i++)
    {
        vertices[i] = cubeVerts[i];
    }*/
    
    return true;
}

bool Sphere::loadNormals(void* info)
{
    /*for(int i = 0; i < numNormals * 3; i++)
    {
        normals[i] = cubeNormals[i];
    }*/
    
    return true;
}

bool Sphere::loadTextureCoords(void* info)
{
    /*for(int i = 0; i < numTexCoords * 2; i++)
    {
        texCoords[i] = cubeTex[i];
    }*/
    
    return true;
}

bool Sphere::loadIndices(void* info)
{
    /*for(int i = 0; i < numIndices; i++)
    {
        indices[i] = cubeIndices[i];
    }*/
    
    return true;
}
