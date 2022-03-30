//
//  MazeEnemy.hpp
//  c8051SwiftGL
//
//  Created by Martin Van Laethem on 2022-03-29.
//

#ifndef MazeEnemy_hpp
#define MazeEnemy_hpp

#include <stdio.h>
#include "Drawable.hpp"

using namespace std;

class MazeEnemy {
    int rowNum;
    int colNum;
    
public:
    MazeEnemy(Drawable*d, int rowNum, int colNum);
    ~MazeEnemy();
    void update();
//    Animator();
//    Animator(Transform*);
//    void assignTransform(Transform*);
//    void assignTransformSpeed(Transform*);
//    void update();
//    void setEnabled(bool);
//    void setBuildupSpeed(float);
//    Transform* getTransformSpeed();
//    bool isMoving();
};

#endif /* MazeEnemy_hpp */
