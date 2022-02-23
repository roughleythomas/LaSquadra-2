//
//  CubeRender.hpp
//  c8051SwiftGL
//
//  Created by Thomas on 2022-02-22.
//  Renderable of a 1 x 1 x 1 cube

#ifndef CubeRender_hpp
#define CubeRender_hpp

#include <stdio.h>
#include "Renderable.hpp"

class CubeRender: public Renderable
{
public:
    CubeRender();
    ~CubeRender();
    
    bool loadVertices(void* info);
    bool loadNormals(void* info);
    bool loadTextureCoords(void* info);
    bool loadIndices(void* info);
protected:
private:
};

#endif /* CubeRender_hpp */
