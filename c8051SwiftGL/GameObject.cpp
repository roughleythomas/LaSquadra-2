//
//  GameObject.cpp
//  c8051SwiftGL
//
//  Created by Thomas on 2022-02-22.
//

#include "GameObject.hpp"

GameObject::GameObject()
:transform(new Transform())
{
    
}

GameObject::GameObject(const GameObject& obj)
:transform(new Transform(*(obj.transform)))
{
    
}

GameObject::~GameObject()
{
    delete transform;
}

Transform* GameObject::getTransform() { return transform; }
