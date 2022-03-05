//
//  GOController.hpp
//  c8051SwiftGL
//
//  Created by Thomas Roughley on 2022-02-23.
//  Controller of a game object with reference to the renderable object the game object represents

#ifndef GOController_hpp
#define GOController_hpp

#include <stdio.h>
#include <chrono>

#include "GameObject.hpp"
#include "Renderable.hpp"

class GOController
{
public:
    //Should be private with factory method invocation
    GOController(GameObject* gameObject, Renderable* renderable);
    //~GOController();
    
    //Update object based on
    virtual void update(std::chrono::milliseconds& deltaTime);
    virtual void draw(std::chrono::milliseconds& deltaTime);
    
    //TEMPORARY accessor methods
    GameObject* getGameObject();
    Renderable* getRenderable();
protected:
private:
    GameObject* gameObject;
    Renderable* renderable;
};

#endif /* GOController_hpp */
