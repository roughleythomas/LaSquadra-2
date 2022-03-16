#ifndef Transform_hpp
#define Transform_hpp

#include <stdio.h>
#include "glm-master/glm/glm.hpp"
#include "glm-master/glm/gtc/quaternion.hpp"
#include "glm-master/glm/gtx/quaternion.hpp"
#include <iostream>

using namespace glm;

class Transform
{
public:
    Transform();
    
    void setPosition(vec3);
    void setAngles(vec3);
    void rotate(vec3);
    void setScale(vec3);
    vec3 getPosition();
    vec3 getAngles();
    vec3 getScale();
    mat4 getMatrix();
private:
    vec3 pos;
    vec3 angles;
    vec3 scale;
};

#endif /* Transform_hpp */
