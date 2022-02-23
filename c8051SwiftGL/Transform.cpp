//
//  Transform.cpp
//  c8051SwiftGL
//
//  Created by Thomas Roughley on 2022-02-22.
//  Basic transformations for a game object

#include "Transform.hpp"
#include "glm-master/glm/ext/quaternion_trigonometric.hpp"

Transform::Transform() {}

Transform::~Transform() {}

Transform::Transform(Transform& other)
{
    position = other.position;
    quaternion = other.quaternion;
    scale = other.scale;
}

glm::vec4 Transform::translate(glm::vec4 delta)
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

void Transform::setPosition(glm::vec4 location)
{
    position = location;
}

void Transform::setQuaternion(glm::quat rotations)
{
    quaternion = rotations;
}

glm::vec4& Transform::getPosition() { return position; }
glm::quat& Transform::getQuaternion() { return quaternion; }
glm::vec4& Transform::getScale() { return scale; }
