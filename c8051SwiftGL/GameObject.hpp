#ifndef GameObject_hpp
#define GameObject_hpp

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Transform.hpp"

class GameObject
{
public:
    GameObject();
    Transform *globalTransform, *localTransform;
};

#endif
