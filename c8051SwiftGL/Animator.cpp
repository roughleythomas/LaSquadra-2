//
//  Animator.cpp
//  c8051SwiftGL
//
//  Created by Michael Sinclair on 2022-03-16.
//

#include "Animator.hpp"

Animator::Animator(vec3 rotateSpeed){
    this->rotateSpeed = rotateSpeed;
}

void Animator::assignTransform(Transform* contextTransform){
    this->contextTransform = contextTransform;
}

void Animator::update(){
    auto elapsedTime = chrono::duration_cast<std::chrono::milliseconds>(chrono::steady_clock::now() - lastFrame).count();
    contextTransform->rotate(vec3(rotateSpeed.x * elapsedTime, rotateSpeed.y * elapsedTime, rotateSpeed.z * elapsedTime));
}
