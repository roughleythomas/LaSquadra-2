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

Renderable::~Renderable()
{
    if(vertices)
        delete vertices;
    if(normals)
        delete normals;
    if(texCoords)
        delete texCoords;
}

int Renderable::getNumberOfVertices() { return numVertices; }
int Renderable::getNumberOfNormals() { return numNormals; }
int Renderable::getNumberOfTexCoordinates() { return numTexCoords; }
int Renderable::getNumberOfIndices() { return numIndices; }
int* Renderable::getIndices() { return indices; }
float* Renderable::getVertices() { return vertices; }
float* Renderable::getNormals() { return normals; }
float* Renderable::getTextureCoordinates() { return texCoords; }
