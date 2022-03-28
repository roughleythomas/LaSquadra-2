//
//  Cylinder.cpp
//  c8051SwiftGL
//

#include "Cylinder.hpp"

Cylinder::Cylinder(int textureListIndex, float radius, float thickness, int sectorCount)
:Drawable(textureListIndex, 24, 24, 24, 36)
{
    this->radius = radius;
    this->thickness = thickness;
    this->sectorCount = sectorCount;
    loadAll();
    
    //loadVertices(0);
    //loadNormals(0);
    //loadTextureCoords(0);
    //loadIndices(0);
}

void Cylinder::loadAll(){
    float x, y;
    float nx, ny, lengthInv = 1.f / radius;
    
    float sectorStep = 2 * M_PI / sectorCount;
    float sectorAngle;
    
    // bottom-center
    vertices.push_back(0);
    vertices.push_back(0);
    vertices.push_back(0);
    normals.push_back(0);
    normals.push_back(0);
    normals.push_back(-1);
    texCoords.push_back(0);
    
    // bottom circumference
    for(int i = 0; i <= sectorCount; i++) {
        sectorAngle = i * sectorStep;
        x = radius * cosf(sectorAngle);
        y = radius * sinf(sectorAngle);
        
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(0);
        
        nx = x * lengthInv;
        ny = y * lengthInv;
        normals.push_back(nx);
        normals.push_back(ny);
        normals.push_back(-1);
        
        texCoords.push_back(0);
    }
    
    // top-center
    vertices.push_back(0);
    vertices.push_back(0);
    vertices.push_back(thickness);
    normals.push_back(0);
    normals.push_back(0);
    normals.push_back(1);
    texCoords.push_back(0);
    
    // top circumference
    for(int i = 0; i <= sectorCount; i++) {
        sectorAngle = i * sectorStep;
        x = radius * cosf(sectorAngle);
        y = radius * sinf(sectorAngle);
        
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(thickness);
        
        nx = x * lengthInv;
        ny = y * lengthInv;
        normals.push_back(nx);
        normals.push_back(ny);
        normals.push_back(1);
        
        texCoords.push_back(0);
        texCoords.push_back(0);
    }
    
    // side
    for(int i = 0; i <= sectorCount; i++){
        sectorAngle = i * sectorStep;
        x = radius * cosf(sectorAngle);
        y = radius * sinf(sectorAngle);
        
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(0);
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(thickness);
        
        nx = x * lengthInv;
        ny = y * lengthInv;
        normals.push_back(nx);
        normals.push_back(ny);
        normals.push_back(0);
        normals.push_back(nx);
        normals.push_back(ny);
        normals.push_back(thickness);
        
        texCoords.push_back(0);
        texCoords.push_back(0);
    }
    
    //indices
    for(int i = 0; i < sectorCount; i++){
        // sector of bottom side
        indices.push_back(0);
        indices.push_back(1 + i);
        indices.push_back(1 + i + 1);
        
        // sector of top side
        indices.push_back(sectorCount + 2);
        indices.push_back((sectorCount + 2) + 1 + i);
        indices.push_back((sectorCount + 2) + 1 + i + 1);
    }
    
    int offset = 2 + 2 * (sectorCount + 1);
    for(int i = 0; i < sectorCount; i++){ // side
        indices.push_back(offset + i * 2);
        indices.push_back(offset + i * 2 + 2);
        indices.push_back(offset + i * 2 + 1);
        
        indices.push_back(offset + i * 2 + 1);
        indices.push_back(offset + i * 2 + 2);
        indices.push_back(offset + i * 2 + 3);
    }
}

bool Cylinder::loadVertices(void* info)
{
    /*for(int i = 0; i < numVertices * 3; i++)
    {
        vertices[i] = cubeVerts[i];
    }*/
    
    return true;
}

bool Cylinder::loadNormals(void* info)
{
    /*for(int i = 0; i < numNormals * 3; i++)
    {
        normals[i] = cubeNormals[i];
    }*/
    
    return true;
}

bool Cylinder::loadTextureCoords(void* info)
{
    /*for(int i = 0; i < numTexCoords * 2; i++)
    {
        texCoords[i] = cubeTex[i];
    }*/
    
    return true;
}

bool Cylinder::loadIndices(void* info)
{
    /*for(int i = 0; i < numIndices; i++)
    {
        indices[i] = cubeIndices[i];
    }*/
    
    return true;
}
