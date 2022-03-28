//
//  Cylinder.hpp
//  c8051SwiftGL
//

#ifndef Cylinder_hpp
#define Cylinder_hpp

#include <stdio.h>
#include "Drawable.hpp"

class Cylinder : public Drawable {
    float radius, thickness;
    int stackCount, sectorCount;
    void loadAll();
    
public:
    Cylinder(int, float, float, int);
    bool loadVertices(void* info);
    bool loadNormals(void* info);
    bool loadTextureCoords(void* info);
    bool loadIndices(void* info);
};

#endif /* Cylinder_hpp */
