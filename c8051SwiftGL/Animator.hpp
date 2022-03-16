//
//  Animator.hpp
//  c8051SwiftGL
//
//  Created by Michael Sinclair on 2022-03-16.
//

#ifndef Animator_hpp
#define Animator_hpp

#include <stdlib.h>
#include <chrono>
#include "Transform.hpp"

using namespace std;

class Animator{
    Transform* contextTransform;
    std::chrono::time_point<std::chrono::steady_clock> lastFrame;
    vec3 rotateSpeed;
    
public:
    Animator(vec3 = vec3(1.f, 1.f, 1.f));
    void assignTransform(Transform*);
    void update();
};

#endif /* Animator_hpp */
