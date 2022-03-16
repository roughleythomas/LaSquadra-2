//
//  Transform.cpp
//  c8051SwiftGL
//
//  Created by Thomas Roughley on 2022-02-22.
//  Basic transformations for a game object

#include "Transform.hpp"
#include "glm-master/glm/ext/quaternion_trigonometric.hpp"

using namespace glm;

Transform::~Transform() {}

Transform::Transform(const Transform& other)
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

vec3 Transform::getPosition() { return pos; }
vec3 Transform::getAngles() { return angles; }
vec3 Transform::getScale() { return scale; }

glm::mat4x4 getTransformMatrix()
{
    glm::mat4x4 transform(1.0f);
    
    transform = glm::scale(transform, scale);
    transform = glm::rotate(transform, angles.x, vec)
    transform = glm::translate(transform, pos);
    
    return transform;
}
