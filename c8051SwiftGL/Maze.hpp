//
//  Maze.hpp
//  c8051SwiftGL
//
//  Created by Michael Sinclair on 2022-03-07.
//

#ifndef Maze_hpp
#define Maze_hpp

#include <stdio.h>
#include <vector>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <algorithm>

using namespace std;

class MazeSector{
    /* Type:
     0 - top left corner (4 sides)
     1 - top row (3 sides, no left wall)
     2 - left column (3 sides, no top wall)
     3 - Remaining sectors (2 sides, no top or left wall)
     */
    int type;
    vector<bool> hidden;//clockwise arrangement of the walls
    
public:
    MazeSector(int);
    void setWallHidden(int);
    int getType();
    void print();
    bool printTop();
};

class Maze{
    vector<int> stack;
    vector<int> visited;
    vector<MazeSector> maze;
    int size;
    int bridgeUnvisitedCell(int);
    void load();
    
public:
    Maze(int);
    void print();
};

#endif /* Maze_hpp */
