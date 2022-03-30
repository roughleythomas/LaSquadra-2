//
//  Maze.cpp
//  c8051SwiftGL
//
//  Created by Michael Sinclair on 2022-03-07.
//

#include "Maze.hpp"

MazeSector::MazeSector(int type){
    this->type = type;
    int wallNo = 0;
    switch(type){
        case 0:
            wallNo = 4;
            break;
        case 1:
        case 2:
            wallNo = 3;
            break;
        case 3:
            wallNo = 2;
            break;
    }
    for(int i = 0; i < wallNo; i++)
        hidden.push_back(false);
}

void MazeSector::setWallHidden(int wall, bool isHidden){
    hidden[wall] = isHidden;
}

bool MazeSector::getWallHidden(int wall){
    return hidden[wall];
}

int MazeSector::getType(){
    return type;
}

bool MazeSector::printTop(){
    switch(type){
        case 0:
        case 1:
            if(!hidden[0]){
                cout << " _";
                return true;
            }
    }
    
    return false;
}

void MazeSector::print(){
    string s = "";
    //print the walls that aren't hidden
    switch(type){
        case 0:
            if(!hidden[3])
                s += "|";
            else
                s += " ";
            if(!hidden[2])
                s += "_";
            else
                s += " ";
            if(!hidden[1])
                s += "|";
            else
                s += " ";
            break;
        case 1:
            if(!hidden[2])
                s += "_";
            else
                s += " ";
            if(!hidden[1])
                s += "|";
            else
                s += " ";
            break;
        case 2:
            if(!hidden[2])
                s += "|";
            else
                s += " ";
            if(!hidden[1])
                s += "_";
            else
                s += " ";
            if(!hidden[0])
                s += "|";
            else
                s += " ";
            break;
        case 3:
        default:
            if(!hidden[1])
                s += "_";
            else
                s += " ";
            if(!hidden[0])
                s += "|";
            else
                s += " ";
            break;
    }
    cout << s;
}

Maze::Maze(int size){
    if(size < 2)
        throw 30;
    
    this->size = size;
    maze.push_back(MazeSector(0));
    int row = 0, col = 1;
    while(row < size){
        while(col < size){
            if(row == 0)
                maze.push_back(MazeSector(1));
            else
                maze.push_back(MazeSector(3));
            
            col++;
        }
        
        if(++row < size){
            maze.push_back(MazeSector(2));
            col = 1;
        }
    }
    load();
}

//On unvisited cells, generate cell.
int Maze::bridgeUnvisitedCell(int curCell){
    //process navigatable directions (clockwise)
    bool directionsAvailable[4] = {
        (curCell - size >= 0),
        (curCell + 1) % size != 0,
        (curCell + size < size * size),
        (curCell % size != 0)
    };
    
    //choose one of the unvisited neighbours
    int dir, secNo;
    bool wallsTested[4] = { false, false, false, false };//walls verified if visited
    //treat the unavailable directions as tested walls
    for(int i = 0; i < 4; i++){
        if(!directionsAvailable[i])
            wallsTested[i] = true;
    }
    do{
        //check direction and test if its available
        dir = rand() % 4;
        while(!directionsAvailable[dir])
            dir = rand() % 4;
        
        if(wallsTested[0] && wallsTested[1] && wallsTested[2] && wallsTested[3])
            return -1;
        switch(dir){
            case 0:
                secNo = curCell - size;
                break;
            case 1:
                secNo = curCell + 1;
                break;
            case 2:
                secNo = curCell + size;
                break;
            case 3:
                secNo = curCell - 1;
                break;
        }
        wallsTested[dir] = true;
    } while(count(visited.begin(), visited.end(), secNo));//check if the neighbour is visited
    
    //remove the wall between the current cell and the chosen cell
    int secType = maze[secNo].getType(), lastType = maze[curCell].getType();
    switch(dir){
        case 0:
            if(secType == 0 || secType == 1)
                maze[secNo].setWallHidden(2);
            else
                maze[secNo].setWallHidden(1);
            break;
        case 1:
            if(lastType == 0 || lastType == 1)
                maze[curCell].setWallHidden(1);
            else
                maze[curCell].setWallHidden(0);
            break;
        case 2:
            if(lastType == 0 || lastType == 1)
                maze[curCell].setWallHidden(2);
            else
                maze[curCell].setWallHidden(1);
            break;
        case 3:
            if(secType == 0 || secType == 1)
                maze[secNo].setWallHidden(1);
            else
                maze[secNo].setWallHidden(0);
            break;
    }
    
    return secNo;
}


//Instantiate maze scene, and generate maze.
void Maze::load(){
    maze[0].setWallHidden(3);
    maze[maze.size() - 1].setWallHidden(0);
    
    srand(time(NULL));
    stack.clear();
    visited.clear();
    stack.push_back(0);
    visited.push_back(0);
    
    //generates maze using the recursive implementation
    do{
        int neighbour = bridgeUnvisitedCell(stack[stack.size() - 1]);
        
        //invoke the routine recursively for a chosen cell
        if(neighbour == -1)
            stack.pop_back();
        else{
            stack.push_back(neighbour);
            visited.push_back(neighbour);
        }
    } while(stack[stack.size() - 1] != 0);
}

void Maze::print(){
    for(int i = 0; i < size; i++){
        bool topPrinted;
        for(int j = 0; j < size; j++){
            if(maze[i * size + j].printTop())
                topPrinted = true;//topPrinted is assigned true even if only one instance is true
        }
        if(topPrinted){
            cout << endl;
            topPrinted = false;
        }
        for(int j = 0; j < size; j++)
            maze[i * size + j].print();
        cout << endl;
    }
}

MazeSector Maze::getSector(int row, int column)
{
    int index = column * size + row;
    
    if(index < 0 || index >= maze.size())
    {
        index = 0;
    }
    
    return maze[index];
}

int Maze::getSize() { return size; }
