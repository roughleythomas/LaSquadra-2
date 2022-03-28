//
//  Animator.cpp
//  c8051SwiftGL
//
//  Created by Michael Sinclair on 2022-03-16.
//

#include "Animator.hpp"

Animator::Animator(){
    transformSpeed = new Transform();
    transformSpeed->setScale(vec3(0.f, 0.f, 0.f));
}

Animator::Animator(Transform* transformSpeed){
    this->transformSpeed = transformSpeed;
}

void Animator::assignTransform(Transform* contextTransform){
    this->contextTransform = contextTransform;
}

void Animator::update(){
    if(enabled){
        auto elapsedTime = chrono::duration_cast<std::chrono::milliseconds>(chrono::steady_clock::now() - lastFrame).count() * 0.00000001f;
        vec3 positionSpeed  = transformSpeed->getPosition(), rotationSpeed = transformSpeed->getAngles();
        
        contextTransform->translate(vec3(positionSpeed.x * elapsedTime, positionSpeed.y * elapsedTime, positionSpeed.z * elapsedTime));
        contextTransform->rotate(vec3(rotationSpeed.x * elapsedTime, rotationSpeed.y * elapsedTime, rotationSpeed.z * elapsedTime));
    }
}

void Animator::setEnabled(bool enabled){
    this->enabled = enabled;
}
