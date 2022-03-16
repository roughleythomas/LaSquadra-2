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

void Transform::rotate(vec3 angles){
    this->angles = vec3(this->angles.x + angles.x, this->angles.y + angles.y, this->angles.z + angles.z);
}

void Transform::setScale(vec3 scale){
    this->scale = scale;
}

vec3 Transform::getPosition() { return pos; }
vec3 Transform::getAngles() { return angles; }
vec3 Transform::getScale() { return scale; }

mat4 Transform::getMatrix(){
    mat4 scale = glm::scale(mat4(1.f), getScale());
    mat4 rot = glm::rotate(glm::mat4(1.0), glm::radians(angles.x), vec3(1, 0, 0));
    rot = glm::rotate(rot, glm::radians(angles.y), vec3(0, 1, 0));
    rot = glm::rotate(rot, glm::radians(angles.z), vec3(0, 0, 1));
    return glm::translate(scale * rot, getPosition());
}
