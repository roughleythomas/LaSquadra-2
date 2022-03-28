//
//  Scene.cpp
//  c8051SwiftGL
//
//  Created by Michael Sinclair on 2022-03-17.
//

#include "Scene.hpp"

Scene::~Scene(){
    for(Drawable *d : drawables)
        delete d;
    delete camera;
}

void Scene::addDrawable(Drawable *d){
    drawables.push_back(d);
}

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

void Scene::update(){
    updateTransform();
    lastFrame = std::chrono::steady_clock::now();
}

void Scene::updateTransform(){
    for(Drawable* drawable : drawables)
        drawable->updateTransform();
}

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

void Scene::loadModels(){
    camera = Camera::GetInstance();
    reset();
}

void MazeScene::addWall(bool horizontal, float posX, float posY, float alternateScale, int textureListIndex){
    addDrawable(new Cube(1));
    int lindex = drawables.size() - 1;
    drawables[lindex]->globalTransform->setPosition(glm::vec3(posX, 0.25f, posY));
    if(horizontal)
        drawables[lindex]->globalTransform->setScale(glm::vec3(alternateScale, 0.25f, 0.01f));
    else
        drawables[lindex]->globalTransform->setScale(glm::vec3(0.01f, 0.25f, alternateScale));
}

void MazeScene::addCoin(float posX, float posY, float radius, float thickness, int textureListIndex, int sectorCount){
    Drawable *coinDrawable = new Cylinder(textureListIndex, radius, thickness, sectorCount);
    addDrawable(coinDrawable);
    coinDrawables.push_back(coinDrawable);
    
    coinDrawable->globalTransform->setPosition(vec3(posX, 1.f, posY));
    coinDrawable->assignAnimator(new Animator(vec3(0, 0.00000005f, 0.00000005f)));
    coinDrawable->anim->setEnabled(true);
}

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
    
    int row = posX;
    
    return collision;
}

bool MazeScene::isAllCoinsCollected() {
    return coinDrawables.empty();
}
