//
//  Sphere.hpp
//  c8051SwiftGL
//
//  Created by Michael Sinclair on 2022-03-16.
//

#ifndef Sphere_hpp
#define Sphere_hpp

#include <stdio.h>
#include "Drawable.hpp"

class Sphere : public Drawable {
    float radius;
    int stackCount, sectorCount;
    void loadAll();
    
public:
    Sphere(float, int, int);
    //bool loadVertices(void* info);
    //bool loadNormals(void* info);
    //bool loadTextureCoords(void* info);
    //bool loadIndices(void* info);
};

#endif /* Sphere_hpp */
