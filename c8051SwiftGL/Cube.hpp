#ifndef CubeRender_hpp
#define CubeRender_hpp

#include <stdio.h>
#include "Drawable.hpp"

class Cube: public Drawable
{
public:
    Cube(int);
    
    bool loadVertices(void* info);
    bool loadNormals(void* info);
    bool loadTextureCoords(void* info);
    bool loadIndices(void* info);
protected:
private:
};

#endif
