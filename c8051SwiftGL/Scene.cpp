//
//  Scene.cpp
//  c8051SwiftGL
//
//  Created by Michael Sinclair on 2022-03-17.
//

#include "Scene.hpp"


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

void Scene::moveBall(float x, float y) {

}


//Update the transform when this scene is updated, and specify when the last frame was calculated (based on current time)
void Scene::update(){
    cout << "Checked goal condition";
    checkGoalCondition();
    cout << "Checked goal condition";
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
    camera = Camera::GetInstance();
    reset();
}




//___________ Maze Scene functions and attributes (all a child of the maze class, as defined in scene.hpp) _____________


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
    
    coinDrawable->globalTransform->setPosition(vec3(posX, 1.f, posY));
    coinDrawable->assignAnimator(new Animator(vec3(0, 0.00000005f, 0.00000005f)));
    coinDrawable->anim->setEnabled(true);
}

//Add new UI Timer to the maze
void MazeScene::addTimer(bool horizontal, float posX, float posY, float alternateScale, int textureListIndex)
{
    //Add new drawable with texture element
    addDrawable(new UITimer(0));
    int lindex = drawables.size() - 1;
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
    drawables[0]->globalTransform->setScale(vec3(2.f, 0.25f, 2.f));
    
    srand (time(NULL));
    float wallNum = rand() % 4 + 8;
    maze = new Maze(wallNum);//random maze size
    maze->print();
    
    float sector = 2.f / wallNum;
    addWall(true, 0.f, 2.f, 2.f);
    addWall(false, -2.f, -sector, 2.f - sector);
    
    cout << "\nCurrent goal condition: " << maze->goalCondition << "\n";
    
    //HARD CODE GOAL CONDITION, REMOVE THIS AFTER TESTING!!!!!!  *******************
    maze->goalCondition = 0;
    
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
            
            bool coinExists = rand() % 8 == 0; // coin generator
            if (coinExists) {
                addCoin(centerX, centerY, sector / 2, 0.015, 2);
            }
        }
    }
    
    ballDrawable = new Sphere(1, 0.15f, 10, 10);
    addDrawable(ballDrawable);
    ballDrawable->globalTransform->setPosition(vec3(0, 1.f, 0.f));
    ballDrawable->assignAnimator(new Animator(vec3(0, 0.000001f, 0.000001f)));
    ballDrawable->anim->setEnabled(true);
    
    printf("loadModels");
}


// ---------- Other scene specific functions -------------

//Translate ball to x,y instead of current x,y position.
void MazeScene::moveBall(float x, float y) {
    Scene::moveBall(x, y);
    
    if (!ballDrawable) {
        return;
    }
    
    ballDrawable->globalTransform->setPosition(vec3(x, 1.0, y));
    
    // check collision
    for (int i = 0; i < coinDrawables.size(); i++) {
        Drawable *drawable = coinDrawables[i];
        vec3 position = drawable->globalTransform->getPosition();
        float deltaX = position.x - x;
        float deltaY = position.z - y;
        float distance = sqrt(deltaX * deltaX + deltaY * deltaY);
        if (distance < 0.2) {
            // remove collide coin
            coinDrawables.erase(coinDrawables.begin() + i);
            remove(drawables.begin(), drawables.end(), drawable);
        }
    }
}

bool MazeScene::wallCheck(float posX, float posY)
{
    bool collision = false;
    
    int row = floor(posX * 2 + 4);
    int column = floor(posY * 2 + 4);
    
    MazeSector sector = maze->getSector(row, column);
    for(int i  = 0; i < 4; i++)
    {
        if(!sector.getWallHidden(i))
        {
        }
    }
    
    return collision;
}

bool MazeScene::isAllCoinsCollected() {
    return coinDrawables.empty();
}

void MazeScene::checkGoalCondition()
{
    cout << "No win :<\n yet ;)\n";
    
   switch(maze->goalCondition)
   {
    //Collect the coins!
       case 0:
           if(coinDrawables.empty())
               cout << "You win!";
           break;
       //Escape the maze!
       case 1:
           cout << "You win! But not really because this needs to be set once collisions are in.";
           break;
       default:
           cout<<"Error: MazeScene does not have a goalCondition set. Please check Scene.cpp, as the current condition is " << maze->goalCondition;
           break;
           
           
   }
}
