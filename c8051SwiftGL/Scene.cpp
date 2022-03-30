//
//  Scene.cpp
//  c8051SwiftGL
//
//  Created by Michael Sinclair on 2022-03-17.
//

#include "Scene.hpp"
#include "Global.h"

// _______________________   Scene Functions, a parent class to all 'scenes' in the game, as defined in scene.hpp ________________

//Delete all drawables and the camera upon a deconstruction of this class.
Scene::~Scene(){
    for(Drawable *d : drawables)
        delete d;
    delete camera;
}

//Add a new 'drawable' element to the scene to be rendered upon a 'draw' call.
void Scene::addDrawable(Drawable *d){
    drawables.push_back(d);
}

//Reset scene by resetting the camera to it's default state, thus allowing the objects on screen to render in their default state, since the
//camera is reset.
void Scene::reset(){
    //camera->getTransform()->setPosition(vec3(2.5, 0.f, -1.75f));
    //camera->getTransform()->setAngles(vec3(-45.f, 0.f, 90.f));
    camera->getTransform()->setPosition(vec3(0.f, -2.25f, -4.5f));
    camera->getTransform()->setAngles(vec3(25.f, 0.f, 0.f));
}

void Scene::pan(float panX, float panY){
    auto curTime = chrono::steady_clock::now();
    auto elapsedTime = chrono::duration_cast<std::chrono::milliseconds>(curTime - lastFrame).count();
    float ratio = elapsedTime * 0.000005f;
    
    if(abs(panY) >= abs(panX)){
        camera->getTransform()->translate(vec3(0, 0, panY * ratio));
    } else{
        float hyp = sqrt(pow(panX, 2) + pow(panY, 2));
        float sin = panX / hyp;
        camera->getTransform()->rotate(vec3(0, asin(-sin), 0));
    }
}

void Scene::movePlayer(int playerDir) {
    this->playerDir = playerDir;
    if (!playerDrawable) {
        return;
    }
    
    cout << "\nFrom scene movePlayer playerDir: " << playerDir;
    
    float playerSpeed = 0.005f;
    
    Transform* transformSpeed = playerDrawable->anim->getTransformSpeed();
    switch(playerDir){
        case -1:
            transformSpeed->setPosition(vec3(0.f, 0.f, 0.f));
            break;
        case 0:
            transformSpeed->setPosition(vec3(0.f, 0.f, -1*playerSpeed));
            break;
        case 1:
            transformSpeed->setPosition(vec3(playerSpeed, 0.f, 0.f));
            break;
        case 2:
            transformSpeed->setPosition(vec3(0.f, 0.f, playerSpeed));
            break;
        case 3:
            transformSpeed->setPosition(vec3(-1*playerSpeed, 0.f, 0.f));
            break;
    }
    playerDrawable->anim->assignTransformSpeed(transformSpeed);
}


//Update the transform when this scene is updated, and specify when the last frame was calculated (based on current time)
void Scene::update(){
    updateTransform();
    lastFrame = std::chrono::steady_clock::now();
}

//Update the transform of each drawable, which is specified in drawable's class definition.
void Scene::updateTransform(){
    for(Drawable* drawable : drawables)
        drawable->updateTransform();
}

//Draw current scene's elements, in reference to the MVP Camera position (the start position).
//Check scene's list of drawables, and for each drawable, get the indices, verts, etc, translate them in reference to the camera, so they are positioned based on where the camera is located.
void Scene::draw(vector<GLuint> textureIds, float aspect, GLint mvpMatrixUniform, GLint normalMatrixUniform){
    mvp = glm::translate(mat4(1.f), camera->getTransform()->getPosition());
    
    for(Drawable *drawable : drawables){
        int *indices = drawable->getIndices(), numIndices = drawable->getNumIndices();
        mat4 transform = drawable->draw(mvp);
        
        normalMatrix = glm::inverseTranspose(glm::mat3(transform));
        mat4 perspective = glm::perspective(60.0f * glm::pi<float>() / 180.f, aspect, 1.0f, 20.0f);
        vec3 angles = camera->getTransform()->getAngles();
        perspective = glm::rotate(perspective, radians(angles.x), vec3(1, 0, 0));
        perspective = glm::rotate(perspective, radians(angles.y), vec3(0, 1, 0));
        perspective = glm::rotate(perspective, radians(angles.z), vec3(0, 0, 1));
        transform = perspective * transform;
        
        glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, value_ptr(transform));
        glUniformMatrix3fv(normalMatrixUniform, 1, GL_FALSE, glm::value_ptr(normalMatrix));
        glBindTexture(GL_TEXTURE_2D, textureIds[drawable->getTextureListIndex()]);
        //glUniform1i(textureUniform, textureIds[drawable->getTextureListIndex()]);
        glDrawElements ( GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, indices );
    }
}

//Load models based on camera instance. The camera would be reset it to it's default state before drawables are rendered.
void Scene::loadModels(){
    playerDrawable = new Sphere(1, 0.15f, 10, 10);
    addDrawable(playerDrawable);
    Transform* transformSpeed = new Transform();
    transformSpeed->setPosition(vec3(0.f, 0.f, 0.f));
    transformSpeed->setScale(vec3(0.f, 0.f, 0.f));
    transformSpeed->setAngles(vec3(0, 0.1f, 0.1f));
    playerDrawable->assignAnimator(new Animator(transformSpeed));
    playerDrawable->anim->assignTransform(playerDrawable->globalTransform);
    playerDrawable->anim->setEnabled(true);
    camera = Camera::GetInstance();
    reset();
}

// ------- Add drawables to scene ----------
//Add new wall drawable to the maze.
void MazeScene::addWall(bool horizontal, float posX, float posY, float alternateScale, int textureListIndex){
    addDrawable(new Cube(1));
    int lindex = drawables.size() - 1;
    drawables[lindex]->globalTransform->setPosition(glm::vec3(posX, 0.25f, posY));
    if(horizontal)
        drawables[lindex]->globalTransform->setScale(glm::vec3(alternateScale, 0.25f, 0.01f));
    else
        drawables[lindex]->globalTransform->setScale(glm::vec3(0.01f, 0.25f, alternateScale));
}

//Add new coin to the maze.
void MazeScene::addCoin(float posX, float posY, float radius, float thickness, int textureListIndex, int sectorCount){
    Drawable *coinDrawable = new Cylinder(textureListIndex, radius, thickness, sectorCount);
    addDrawable(coinDrawable);
    coinDrawables.push_back(coinDrawable);
    
    coinDrawable->globalTransform->setPosition(vec3(posX, 0.5f, posY));
    Transform* transformSpeed = new Transform();
    transformSpeed->setScale(vec3(0.f, 0.f, 0.f));
    transformSpeed->setAngles(vec3(0, 0.1f, 0.1f));
    coinDrawable->assignAnimator(new Animator(transformSpeed));
    coinDrawable->anim->setEnabled(true);
}

//Add new goal object to the maze.
void MazeScene::addGoal(float posX, float posY, float radius, float thickness, int textureListIndex, int sectorCount){
    Drawable *portalDrawable = new Cylinder(textureListIndex, radius, thickness, sectorCount);
    addDrawable(portalDrawable);
    //coinDrawables.push_back(portalDrawable);
    
    goal = portalDrawable;
    
    portalDrawable->globalTransform->setPosition(vec3(posX, 0.5f, posY));
    //Transform* transformSpeed = new Transform();
    //transformSpeed->setScale(vec3(0.f, 0.f, 0.f));
    //transformSpeed->setAngles(vec3(0, 0.1f, 0.1f));
    //portalDrawable->assignAnimator(new Animator(transformSpeed));
    //portalDrawable->anim->setEnabled(true);
}

//Add new UI Timer to the maze
void MazeScene::addTimer(float posX, float posY, int textureListIndex)
{
    //Add new drawable with texture element
    addDrawable(new UIElement(textureListIndex));
    //int lindex = drawables.size() - 1;
    //drawables[lindex]->globalTransform->setPosition(glm::vec3(posX, 0.25f, posY));
    
    //if(horizontal)
        //drawables[lindex]->globalTransform->setScale(glm::vec3(alternateScale, 0.25f, 0.01f));
    //else
        //drawables[lindex]->globalTransform->setScale(glm::vec3(0.01f, 0.25f, alternateScale));
    
}

// -------- Load all drawables from scene (initializer) ------

//Load all maze models into the scnee.
void MazeScene::loadModels(){
    Scene::loadModels();
    addDrawable(new Cube(0));
    drawables[1]->globalTransform->setScale(vec3(2.f, 0.25f, 2.f));
    
    addTimer(1.0f,1.0f,3);
    
    srand (time(NULL));
    float wallNum = 8;
    Maze* maze = new Maze(wallNum);//random maze size
    maze->print();
    
    float sector = 2.f / wallNum;
    addWall(true, 0.f, 2.f, 2.f);
    addWall(false, -2.f, -sector, 2.f - sector);
    
    cout << "\nCurrent goal condition: " << maze->goalCondition << "\n";
    
    //Set the scene goal condition to whatever generated number condition was provided by maze.
    sceneGoalCondition = maze->goalCondition;
    
    
    
    bool goalNotAdded = true;
    
    for(int i = 0; i < wallNum; i++){
        int wallTypeHor = ((i > 0) ? 1 : 2),
            wallTypeVer = ((i > 0) ? 0 : 1);
        
        for(int j = 0; j < wallNum; j++){
            float centerX = -2.f + 2 * sector * (j + 1) - sector;
            float centerY = 2.f - 2 * sector * (i + 1) + sector;
            
            if(!maze->maze[i * wallNum + j].getWallHidden(wallTypeHor))
                addWall(true, centerX, centerY - sector, sector);
            if(!maze->maze[i * wallNum + j].getWallHidden(wallTypeVer))
                addWall(false, centerX + sector, centerY, sector);
            
            //Render specific objects based on goal condition
            //goal condition 0, render coins
            if(sceneGoalCondition == 0)
            {
            
                bool coinExists = rand() % 8 == 0; // coin generator
                if (coinExists) {
                    addCoin(centerX, centerY, sector / 2, 0.015, 2);
                }
            
            }//goal condition 1, render goal
            else if (sceneGoalCondition == 1 && goalNotAdded && ((i == (int)wallNum/2) && (j == (int)wallNum/2)))
            {
                addGoal(centerX, centerY, sector/2, 0.01, 3);
                goalNotAdded = false;
            }
        }
    }
    
    playerDrawable->globalTransform->setPosition(vec3(-(float)wallNum * sector + sector, 0.5f, (float)wallNum * sector - sector));
}

void MazeScene::update(){
    Scene::update();
    if(playerDir != -1){
        vec3 playerPos = playerDrawable->globalTransform->getPosition();
        for (int i = 0; i < coinDrawables.size(); i++) {
            Drawable *drawable = coinDrawables[i];
            vec3 position = drawable->globalTransform->getPosition();
            float deltaX = position.x - playerPos.x;
            float deltaY = position.z - playerPos.z;
            float distance = sqrt(deltaX * deltaX + deltaY * deltaY);
            if (distance < 0.2) {
                // remove collide coin
                coinDrawables.erase(coinDrawables.begin() + i);
                remove(drawables.begin(), drawables.end(), drawable);
            }
        }
    }
}

// ---------- Other scene specific functions -------------

//Translate ball to x,y instead of current x,y position.
void MazeScene::movePlayer(int playerDir) {
    //cout << "Player Dir: " << playerDir + "\n";
    
    Scene::movePlayer(playerDir);
    
    
    vec3 playerPos = playerDrawable->globalTransform->getPosition();
    
    //collisionCheck(playerPos.x, playerPos.z);
    // check collision for coins, if not all coins are collected.
    if(!isAllCoinsCollected())
    {
        for (int i = 0; i < coinDrawables.size(); i++)
        {
            Drawable *drawable = coinDrawables[i];
            vec3 position = drawable->globalTransform->getPosition();
            float deltaX = position.x - playerPos.x;
            float deltaY = position.z - playerPos.z;
            float distance = sqrt(deltaX * deltaX + deltaY * deltaY);
            if (distance < 0.2) {
                // remove collide coin
                coinDrawables.erase(coinDrawables.begin() + i);
                remove(drawables.begin(), drawables.end(), drawable);
            }
        }
    }
    
    //If the player should be looking for the goal, check if the goal has been collided with.
    if(sceneGoalCondition == 1)
    {
        vec3 position = goal->globalTransform->getPosition();
        
        //cout << "\nPlayer Y: " << playerPos.y;
        
        float deltaX = position.x - playerPos.x;
        float deltaY = position.z - playerPos.z;
        float distance = sqrt(deltaX * deltaX + deltaY * deltaY);
        
        //cout << distance << "\nX: " << deltaX <<"\nY: "<< deltaY;
        if (distance < 0.2) {
            cout << "goal found!";
            sceneWon = true;
        }
    }
}

int MazeScene::collisionCheck(float posX, float posY)
{
    int collision = 0;
    int row = floor(posX * 2 + 4);
    int column = floor(posY * 2 + 4);
    
    float deltaX = (row * (MAZE_CELL_SIZE / 2.0f)) - posX;
    float deltaY = (column * (MAZE_CELL_SIZE / 2.0f)) - posY;
    
    collision = wallCheck(row, column, posX, posY);
    if(deltaX > 0 && row < maze->getSize())
        collision &= wallCheck(row - 1, column, posX, posY);
    else if(deltaX < 0 && row > 0)
        collision &= wallCheck(row + 1, column, posX, posY);
    if(deltaY > 0 && column < maze->getSize())
        collision &= wallCheck(row, column - 1, posX, posY);
    else if(deltaY < 0 && column > 0)
        collision &= wallCheck(row, column + 1, posX, posY);
    
    return collision;
}

int MazeScene::wallCheck(int row, int column, float posX, float posY)
{
    int collision = 0;
    int topC = column > 0 ? column - 1 : column;
    int leftR = row > 0 ? row - 1 : row;
    
    float cellCenterX = row * 0.5 - 1.75;
    float cellCenterY = column * 0.5 - 1.75;
    
    float deltaX = posX - cellCenterX;
    float deltaY = posY - cellCenterY;
    
    MazeSector curCell = maze->getSector(row, column);
    MazeSector topCell = maze->getSector(row, topC);
    MazeSector leftCell = maze->getSector(leftR, column);
    
    switch(curCell.getType())
    {
        case 0:
            if(!curCell.getWallHidden(0))
            {
                float check = deltaY + PLAYER_RADIUS + SAFE_DISTANCE;
                if(check > MAZE_CELL_SIZE / 2.0f)
                    collision |= 2;
            }
            if(!curCell.getWallHidden(1))
            {
                float check = abs(deltaX + PLAYER_RADIUS + SAFE_DISTANCE);
                if(check > MAZE_CELL_SIZE / 2.0f)
                    collision |= 1;
            }
            if(!curCell.getWallHidden(2))
            {
                float check = abs(deltaY - PLAYER_RADIUS - SAFE_DISTANCE);
                if(check > MAZE_CELL_SIZE / 2.0f)
                    collision |= 2;
            }
            if(!curCell.getWallHidden(3))
            {
                float check = abs(deltaX - PLAYER_RADIUS - SAFE_DISTANCE);
                if(check > MAZE_CELL_SIZE / 2.0f)
                    collision |= 1;
            }
            break;
        case 1:
            if(!curCell.getWallHidden(0))
            {
                float check = deltaY + PLAYER_RADIUS + SAFE_DISTANCE;
                if(check > MAZE_CELL_SIZE / 2.0f)
                    collision |= 2;
            }
            if(!curCell.getWallHidden(1))
            {
                float check = abs(deltaX + PLAYER_RADIUS + SAFE_DISTANCE);
                if(check > MAZE_CELL_SIZE / 2.0f)
                    collision |= 1;
            }
            if(!curCell.getWallHidden(2))
            {
                float check = abs(deltaY - PLAYER_RADIUS - SAFE_DISTANCE);
                if(check > MAZE_CELL_SIZE / 2.0f)
                    collision |= 2;
            }
            if(!leftCell.getWallHidden(1))
            {
                float check = abs(deltaX - PLAYER_RADIUS - SAFE_DISTANCE);
                if(check > MAZE_CELL_SIZE / 2.0f)
                    collision |= 1;
            }
            break;
        case 2:
            if(!curCell.getWallHidden(0))
            {
                float check = deltaX + PLAYER_RADIUS + SAFE_DISTANCE;
                if(check > MAZE_CELL_SIZE / 2.0f)
                    collision |= 1;
            }
            if(!curCell.getWallHidden(1))
            {
                float check = abs(deltaY - PLAYER_RADIUS - SAFE_DISTANCE);
                if(check > MAZE_CELL_SIZE / 2.0f)
                    collision |= 2;
            }
            if(!curCell.getWallHidden(2))
            {
                float check = abs(deltaX - PLAYER_RADIUS - SAFE_DISTANCE);
                if(check > MAZE_CELL_SIZE / 2.0f)
                    collision |= 1;
            }
            if(!topCell.getWallHidden(1))
            {
                float check = deltaY + PLAYER_RADIUS + SAFE_DISTANCE;
                if(check > MAZE_CELL_SIZE / 2.0f)
                    collision |= 2;
            }
            break;
        case 3:
            if(!curCell.getWallHidden(0) && deltaX > 0)
            {
                float check = deltaX + PLAYER_RADIUS + SAFE_DISTANCE;
                if(check > MAZE_CELL_SIZE / 2.0f)
                    collision |= 1;
            }
            if(!curCell.getWallHidden(1) && deltaY < 0)
            {
                float check = abs(deltaY - PLAYER_RADIUS - SAFE_DISTANCE);
                if(check > MAZE_CELL_SIZE / 2.0f)
                    collision |= 2;
            }
            if(!topCell.getWallHidden(1))
            {
                float check = deltaY + PLAYER_RADIUS + SAFE_DISTANCE;
                if(check > MAZE_CELL_SIZE / 2.0f)
                    collision |= 2;
            }
            if(!leftCell.getWallHidden(1))
            {
                float check = abs(deltaX - PLAYER_RADIUS - SAFE_DISTANCE);
                if(check > MAZE_CELL_SIZE / 2.0f)
                    collision |= 1;
            }
            break;
    }
    
    return collision;
}

bool MazeScene::isAllCoinsCollected() {
    return coinDrawables.empty();
}

bool MazeScene::achievedGoal()
{
   switch(sceneGoalCondition)
   {
    //Collect the coins!
       case 0:
           if(coinDrawables.empty())
           {
               cout << "All coins collected!";
               sceneWon = true;
           }
           return sceneWon;
           break;
       //Escape the maze!
       case 1:
           return sceneWon;
           //cout << "You win! But not really because this needs to be set once collisions are in.";
           break;
       default:
           cout<<"Error: MazeScene does not have a goalCondition set. Please check Scene.cpp, as the current condition is " << sceneGoalCondition;
           break;
   }
    
    return sceneWon;
}
