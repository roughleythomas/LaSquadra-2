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

void Transform::translate(vec3 pos){
    this->pos = vec3(this->pos.x + pos.x, this->pos.y + pos.y, this->pos.z + pos.z);
}

void Transform::rotate(vec3 angles){
    this->angles = vec3(this->angles.x + angles.x, this->angles.y + angles.y, this->angles.z + angles.z);
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

mat4 Transform::getMatrix(){
    mat4 pos = glm::translate(glm::mat4(1.f), getPosition());
    mat4 rot = glm::rotate(glm::mat4(1.f), glm::radians(angles.x), vec3(1, 0, 0));
    rot = glm::rotate(rot, glm::radians(angles.y), vec3(0, 1, 0));
    rot = glm::rotate(rot, glm::radians(angles.z), vec3(0, 0, 1));
    mat4 scale = glm::scale(glm::mat4(1.f), getScale());
    //This lines actually performs the scaling FIRST, and THEN the rotation, and THEN the translation - http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/
    return pos * rot * scale;
}
