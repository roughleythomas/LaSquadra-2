//
//  Renderable.cpp
//  c8051SwiftGL
//
//  Created by Thomas on 2022-02-22.
//

#include "Renderable.hpp"

Renderable::Renderable(int vertNum, int normNum, int texNum, int indexNum)
:vertices(0), normals(0), texCoords(0), indices(0), numIndices(indexNum), numVertices(vertNum), numNormals(normNum), numTexCoords(texNum)
{
    if(vertNum > 0)
        vertices = new float[vertNum * 3];
    if(normNum > 0)
        normals = new float[normNum * 3];
    if(texNum > 0)
        texCoords = new float[texNum * 2];
    if(indexNum > 0)
        indices = new int[indexNum];
}

Renderable::Renderable(const Renderable& obj)
:vertices(0), normals(0), texCoords(0), indices(0), numIndices(obj.numIndices), numVertices(obj.numVertices), numNormals(obj.numNormals), numTexCoords(obj.numTexCoords)
{
    if(obj.vertices != 0)
    {
        vertices = new float[obj.numVertices * 3];
        for(int i = 0; i < obj.numVertices * 3; i++)
        {
            vertices[i] = obj.vertices[i];
        }
    }
    if(obj.normals != 0)
    {
        normals = new float[obj.numNormals * 3];
        for(int i = 0; i < obj.numNormals * 3; i++)
        {
            normals[i] = obj.normals[i];
        }
    }
    if(obj.texCoords != 0)
    {
        texCoords = new float[obj.numTexCoords * 2];
        for(int i = 0; i < obj.numTexCoords * 2; i++)
        {
            texCoords[i] = obj.texCoords[i];
        }
    }
    if(obj.indices != 0)
    {
        indices = new int[obj.numIndices];
        for(int i = 0; i < obj.numIndices * 3; i++)
        {
            indices[i] = obj.indices[i];
        }
    }
}

Renderable::~Renderable()
{
    if(vertices)
        delete vertices;
    if(normals)
        delete normals;
    if(texCoords)
        delete texCoords;
}

int Renderable::getNumVertices() { return numVertices; }
int Renderable::getNumNormals() { return numNormals; }
int Renderable::getNumTexCoords() { return numTexCoords; }
int Renderable::getNumIndices() { return numIndices; }
int* Renderable::getIndices() { return indices; }
float* Renderable::getVertices() { return vertices; }
float* Renderable::getNormals() { return normals; }
float* Renderable::getTextureCoords() { return texCoords; }
