//
//  PhysicsWrapper.hpp
//  c8051SwiftGL
//
//  Created by Thomas Roughley on 2022-02-16.
//  Static wrapper class for translating 3d game data into 2d physics data

#ifndef PhysicsWrapper_hpp
#define PhysicsWrapper_hpp

#include <stdio.h>
#include "box2d-main/include/box2d/box2d.h"

class PhysicsWrapper
{
public:
    //D'tor
    ~PhysicsWrapper();
    
    //Gets singleton instance
    static PhysicsWrapper& GetInstance();
    
    //Converts 3D coordinates to 2D physics coordinates
    b2Vec2 from3Dto2D(float* vec3d);
    //Converts 2D physics coordinates into 3D coordinates using the fixed
    //distance to fill in the missing value
    float* from2Dto3D(b2Vec2);
    
    //------------Getters & Setters------------
    float getFixedDistance();
    void setFixedDistance(float distance);
    void setWorld(b2World* world);
    b2World* getWorld();
protected:
private:
    //Private c'tor to prevent accidental instatiation
    PhysicsWrapper();
    //Seperate init just in case
    void Instantiate();
    
    //Static singleton
    static PhysicsWrapper INSTANCE;
    //Distance used to fill in missing values
    float fixedDistance;
    //Physics world in which the objects preside
    //Used to access underlying conversions
    //Should be changed to something more succinct if available
    b2World* theWorld;
};

#endif /* PhysicsWrapper_hpp */
