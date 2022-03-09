//
//  Transform.cpp
//  c8051SwiftGL
//
//  Created by Thomas Roughley on 2022-02-22.
//  Basic transformations for a game object

#include "Transform.hpp"
#include "glm-master/glm/ext/quaternion_trigonometric.hpp"

Transform::Transform() {
    position.x = 0;
    position.y = 0;
    position.z = 0;
    scale.x = 1;
    scale.y = 1;
    scale.z = 1;
    quaternion.x = 0;
    quaternion.y = 0;
    quaternion.z = 0;
    angles.x = 0;
    angles.y = 0;
    angles.z = 0;
}

Transform::~Transform() {}

Transform::Transform(const Transform& other)
{
    position = other.position;
    quaternion = other.quaternion;
    scale = other.scale;
}

glm::vec3 Transform::translate(glm::vec3 delta)
{
    position += delta;
    
    return position;
}

glm::quat Transform::rotate(float angle, glm::vec3 axis)
{
    glm::quat rotation = glm::angleAxis(angle, axis);
    quaternion *= rotation;
    
    return quaternion;
}

glm::vec3 Transform::rotate(glm::vec3 angles){
    this->angles += angles;
    return this->angles;
}

void Transform::setPosition(glm::vec3 location)
{
    position = location;
}

void Transform::setQuaternion(glm::quat rotations)
{
    quaternion = rotations;
}

void Transform::setScale(glm::vec3 scale){
    this->scale = scale;
}

glm::vec3& Transform::getPosition() { return position; }
glm::quat& Transform::getQuaternion() { return quaternion; }
glm::vec3& Transform::getAngles() { return angles; }
glm::vec3& Transform::getScale() { return scale; }
