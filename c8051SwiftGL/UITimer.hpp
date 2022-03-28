//
//  UITimer.hpp
//  c8051SwiftGL
//
//  Created by Kevin Vilanova on 2022-03-28.
//
#ifndef UITimer_hpp
#define UITimer_hpp

#include <stdio.h>
#include "Drawable.hpp"

class UITimer: public Drawable
{
public:
    UITimer(int);
    
    bool loadVertices(void* info);
    bool loadNormals(void* info);
    bool loadTextureCoords(void* info);
    bool loadIndices(void* info);
protected:
private:
};

#endif
