//
//  GameObject.hpp
//  c8051SwiftGL
//
//  Created by Thomas on 2022-02-22.
//  Defines the model of a generic object in the game

#ifndef GameObject_hpp
#define GameObject_hpp

#include <stdio.h>
#include "Transform.hpp"

class GameObject
{
public:
    GameObject();
    GameObject(const GameObject& obj);
    ~GameObject();
protected:
private:
    Transform* transform;
};

#endif /* GameObject_hpp */
