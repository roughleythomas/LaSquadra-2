//
//  Scene.hpp
//  c8051SwiftGL
//
//  Created by Michael Sinclair on 2022-03-17.
//

#ifndef Scene_hpp
#define Scene_hpp

#include <stdio.h>
#include <chrono>

class Scene{
    std::chrono::time_point<std::chrono::steady_clock> lastFrame;
};

class SceneManager{
    enum SceneName {
        MENU,
        MAZE,
        CHEMISTRY_LAB,
        OBSTACLE_COURSE
    };
};

#endif /* Scene_hpp */
