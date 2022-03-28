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
#include "Camera.hpp"
#include "Maze.hpp"

using namespace glm;

class Scene {
    Camera* camera;
    std::chrono::time_point<std::chrono::steady_clock> lastFrame;
    mat4 mvp;
    mat3 normalMatrix;
    void updateTransform();
    
protected:
    vector<Drawable*> drawables;
    void addDrawable(Drawable* d);
    
public:
    ~Scene();
    void reset();
    virtual void pan(float, float);
    virtual void moveBall(float, float);
    virtual void update();
    virtual void draw(vector<GLuint>, float, GLint, GLint);
    virtual void loadModels();
    virtual bool isAllCoinsCollected() { return false; }
};

class MazeScene : public Scene {
    
public:
    void loadModels() override;
    void moveBall(float, float) override;
    bool isAllCoinsCollected() override;
    void addWall(bool, float, float, float, int = 1);
    void addCoin(float, float, float, float, int, int = 30);
    bool wallCheck(float posX, float posY);
    
protected:
    Drawable* ballDrawable;
    vector<Drawable*> coinDrawables;
    Maze* maze;
};

#endif /* Scene_hpp */
