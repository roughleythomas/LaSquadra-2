//
//  Transform.hpp
//  c8051SwiftGL
//
//  Created by Thomas Roughley on 2022-02-22.
//  Basic transformations for a game object

#ifndef Transform_hpp
#define Transform_hpp

#include <stdio.h>
#include "glm-master/glm/glm.hpp"
//#include "glm-master/glm/ext/quaternion_float.hpp"
#include "glm-master/glm/gtc/quaternion.hpp"
#include "glm-master/glm/gtx/quaternion.hpp"
#include <iostream>

class Transform
{
public:
    Transform();
    Transform(const Transform& obj);
    ~Transform();
    
    //Translates object by input
    glm::vec3 translate(glm::vec3 delta);
    //Rotates object about axis by angle
    glm::quat rotate(float angle, glm::vec3 axis);
    glm::vec3 rotate(glm::vec3 angles);
    //Scales object along the given axis (x,y,z) by inputed amount
    glm::vec3 resize(glm::vec3 weights);
    
    //---- Getters & Setters ----
    void setPosition(glm::vec3 position);
    void setQuaternion(glm::quat quaternion);
    void setScale(glm::vec3 scale);
    
    glm::vec3& getPosition();
    glm::quat& getQuaternion();
    glm::vec3& getAngles();
    glm::vec3& getScale();
protected:
private:
    //Location of the game object
    glm::vec3 position;
    //Quaternion used for rotation calculations
    glm::quat quaternion;
    glm::vec3 angles;
    //Scale of game object
    glm::vec3 scale;
};

#endif /* Transform_hpp */
