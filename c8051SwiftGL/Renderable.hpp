//
//  Renderable.hpp
//  c8051SwiftGL
//
//  Created by Thomas on 2022-02-22.
//  An object that stores data on how to render an object

#ifndef Renderable_hpp
#define Renderable_hpp

#include <stdio.h>

class Renderable
{
public:
    Renderable(int numVertices, int numNormals, int numTexCoords, int numIndices);
    Renderable(const Renderable& obj);
    virtual ~Renderable();
    
    // Load data for vertices, normals, and texture coordinates
    // void-pointer allows for type-casting for flexibilty in dealing with info
    virtual bool loadVertices(void* info) = 0;
    virtual bool loadNormals(void* info) = 0;
    virtual bool loadTextureCoords(void* info) = 0;
    virtual bool loadIndices(void* info) = 0;
    
    int getNumberOfVertices();
    int getNumberOfNormals();
    int getNumberOfTexCoordinates();
    int getNumberOfIndices();
    int* getIndices();
    float* getVertices();
    float* getNormals();
    float* getTextureCoordinates();
protected:
    int numVertices, numNormals, numTexCoords, numIndices;
    int* indices;
    float* vertices;
    float* normals;
    float* texCoords;
};

#endif /* Renderable_hpp */
