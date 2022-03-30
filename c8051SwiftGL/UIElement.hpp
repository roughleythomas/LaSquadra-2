//
//  UIElement.hpp
//  c8051SwiftGL
//
//  Created by Kevin Vilanova on 2022-03-28.
//
#ifndef UIElement_hpp
#define UIElement_hpp

#include <stdio.h>
#include "Drawable.hpp"

class UIElement: public Drawable
{
public:
    UIElement(int);
    
    bool loadVertices(void* info);
    bool loadNormals(void* info);
    bool loadTextureCoords(void* info);
    bool loadIndices(void* info);
protected:
private:
};

#endif
