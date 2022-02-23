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
#include "glm-master/glm/ext/quaternion_float.hpp"

class Transform
{
public:
    Transform();
    Transform(const Transform& obj);
    ~Transform();
    
    //Translates object by input
    glm::vec4 translate(glm::vec4 delta);
    //Rotates object about axis by angle
    glm::quat rotate(float angle, glm::vec3 axis);
    //Scales object along the given axis (x,y,z) by inputed amount
    glm::vec4 resize(glm::vec4 weights);
    
    //---- Getters & Setters ----
    void setPosition(glm::vec4 position);
    void setQuaternion(glm::quat quaternion);
    void setScale(glm::vec4 scale);
    
    glm::vec4& getPosition();
    glm::quat& getQuaternion();
    glm::vec4& getScale();
protected:
private:
    //Location of the game object
    glm::vec4 position;
    //Quaternion used for rotation calculations
    glm::quat quaternion;
    //Scale of game object
    glm::vec4 scale;
};

#endif /* Transform_hpp */
