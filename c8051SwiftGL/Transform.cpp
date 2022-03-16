#include "Transform.hpp"
//#include "glm-master/glm/ext/quaternion_trigonometric.hpp"

Transform::Transform() {
    pos.x = 0;
    pos.y = 0;
    pos.z = 0;
    scale.x = 1;
    scale.y = 1;
    scale.z = 1;
    angles.x = 0;
    angles.y = 0;
    angles.z = 0;
}

void Transform::setPosition(vec3 pos){
    this->pos = pos;
}

void Transform::setAngles(vec3 angles){
    this->angles = angles;
}

void Transform::setScale(vec3 scale){
    this->scale = scale;
}

vec3 Transform::getPosition() { return pos; }
vec3 Transform::getAngles() { return angles; }
vec3 Transform::getScale() { return scale; }
