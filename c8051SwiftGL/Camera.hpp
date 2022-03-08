//
//  Camera.hpp
//  c8051SwiftGL
//
//  Created by Thomas Roughley on 2022-02-22.
//  Simulated camera object for Open GL

#ifndef Camera_hpp
#define Camera_hpp

#include <stdio.h>
#include "glm-master/glm/glm.hpp"
#include "Transform.hpp"

class Camera
{
public:
    ~Camera();
    
    //Orients camera to look at specified point
    void lookAt(glm::vec3 point);
    //Rotates camera by specified angles (in degrees) about specified axis
    void rotate(float angle, glm::vec3 axis);
    
    //---- Getters & Setters ----
    void setAspectRatio(float ratio);
    void setFieldOfView(float fov);
    //Setting Z-plane targets will adjust the opposite plane such that nearZ <= farZ
    void setNearZ(float nearZ);
    void setFarZ(float farZ);
    
    //Should probably change this to more setter/getters that better expose transform
    Transform* getTransform();
    glm::vec3 getUp();
    glm::vec3 getRight();
    glm::vec3 getLookAt();
    float getAspectRatio();
    float getFieldOfView();
    float getNearZ();
    float getFarZ();
    
    //Singleton object getter
    static Camera* GetInstance();
    //Orthagonal vectors that describe the camera
    static glm::vec3 UP, RIGHT, FACING;
protected:
private:
    //Private C'tor to prevent accidental instantiation
    Camera();
    
    //Singleton instance
    static Camera* INSTANCE;
    
    //Transforms of camera
    Transform* transform;
    //Vector representation of defining vectors
    glm::vec3 facing, right, up;
    float aspectRatio;
    float fieldOfView;
    float nearZ, farZ;
};

#endif /* Camera_hpp */
