//
//  GOController.cpp
//  c8051SwiftGL
//
//  Created by Thomas Roughley on 2022-02-23.
//

#include "GOController.hpp"

/*GOController::GOController(GameObject* object, Renderable* render)
:gameObject(object), renderable(render)
{
    
}*/

GOController::GOController(GameObject* gameObject, Renderable* renderable){
    this->gameObject = gameObject;
    this->renderable = renderable;
    transform = this->gameObject->getTransform();
}

/*GOController::~GOController()
{
    delete gameObject;
    delete renderable;
}*/

void GOController::update(std::chrono::milliseconds& deltaTime) {}
void GOController::draw(std::chrono::milliseconds& deltaTime) {}

GameObject* GOController::getGameObject() { return gameObject; }
Renderable* GOController::getRenderable() { return renderable; }
