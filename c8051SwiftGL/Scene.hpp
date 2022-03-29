//
//  Scene.hpp
//  c8051SwiftGL
//
//  Created by Michael Sinclair on 2022-03-17.
//

#ifndef Scene_hpp
#define Scene_hpp

#include <OpenGLES/ES3/gl.h>
#include "glm-master/glm/ext.hpp"
#include <stdio.h>
#include <chrono>
#include "Cube.hpp"
#include "Sphere.hpp"
#include "Cylinder.hpp"
#include "UITimer.hpp"
#include "Camera.hpp"
#include "Maze.hpp"

using namespace glm;

class Scene {
    const float PLAYER_SPEED = 0.01f, PLAYER_SLIP = 0.001f;
    Camera* camera;
    std::chrono::time_point<std::chrono::steady_clock> lastFrame;
    mat4 mvp;
    mat3 normalMatrix;
    void updateTransform();
    
protected:
    Drawable* playerDrawable;
    int playerDir = -1;
    vector<Drawable*> drawables;
    void addDrawable(Drawable* d);
    
public:
    ~Scene();
    void reset();
    int sceneGoalCondition;
    bool sceneWon = false;
    virtual void pan(float, float);
    virtual void movePlayer(int);
    virtual void update();
    virtual bool achievedGoal() = 0;
    virtual void draw(vector<GLuint>, float, GLint, GLint);
    virtual void loadModels();
    virtual bool isAllCoinsCollected() { return false; }
};

class MazeScene : public Scene {
    
public:
    void loadModels() override;
    void update() override;
    void movePlayer(int) override;
    bool isAllCoinsCollected() override;
    bool achievedGoal() override;
    void addWall(bool, float, float, float, int = 1);
    void addTimer(bool, float, float, float, int = 1);
    void addCoin(float, float, float, float, int, int = 30);
    int collisionCheck(float posX, float posY);
    //Given position and cell is there a collision?
    int wallCheck(int row, int column, float posX, float posY);
protected:
    vector<Drawable*> coinDrawables;
    Maze* maze;
};

#endif /* Scene_hpp */
