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
    camera->getTransform()->setPosition(vec3(2.5, 0.f, -1.75f));
    camera->getTransform()->setAngles(vec3(-45.f, 0.f, 90.f));
}

void Scene::pan(float panX, float panY){
    auto curTime = chrono::steady_clock::now();
    auto elapsedTime = chrono::duration_cast<std::chrono::milliseconds>(curTime - lastFrame).count();
    float ratio = elapsedTime * 0.000005f;
    
    if(abs(panY) >= abs(panX)){
        camera->getTransform()->translate(vec3(-panY * ratio, 0, 0));
    } else{
        float hyp = sqrt(pow(panX, 2) + pow(panY, 2));
        float sin = panX / hyp;
        camera->getTransform()->rotate(vec3(0, 0, asin(-sin)));
    }
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
        //glUniform1i(uniforms[UNIFORM_TEXTURE], textureIds[drawable->getTextureListIndex()]);
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
    drawables[lindex]->globalTransform->setPosition(glm::vec3(posX, posY, 0.125f));
    if(horizontal)
        drawables[lindex]->globalTransform->setScale(glm::vec3(alternateScale, 0.01f, 0.25f));
    else
        drawables[lindex]->globalTransform->setScale(glm::vec3(0.01f, alternateScale, 0.25f));
}

void MazeScene::loadModels(){
    Scene::loadModels();
    addDrawable(new Cube(0));
    drawables[0]->globalTransform->setScale(vec3(2.f, 2.f, 0.1f));
    
    srand (time(NULL));
    float wallNum = rand() % 6 + 6;
    Maze* maze = new Maze(wallNum);//random maze size
    maze->print();
    
    float sector = 2.f / wallNum;
    addWall(true, 0.f, 2.f, 2.f);
    addWall(false, -2.f, -sector, 2.f - sector);
    for(int i = 0; i < wallNum; i++){
        int wallTypeHor = ((i > 0) ? 1 : 2),
            wallTypeVer = ((i > 0) ? 0 : 1);
        
        for(int j = 0; j < wallNum; j++){
            if(!maze->maze[i * wallNum + j].getWallHidden(wallTypeHor))
                addWall(true, -2.f + 2 * sector * (j + 1) - sector, 2.f - 2 * sector * (i + 1), sector);
            if(!maze->maze[i * wallNum + j].getWallHidden(wallTypeVer))
                addWall(false, -2.f + 2 * sector * (j + 1), 2.f - 2 * sector * (i + 1) + sector, sector);
        }
    }
    
    addDrawable(new Sphere(1, 0.15f, 10, 10));
    drawables[drawables.size() - 1]->globalTransform->setPosition(vec3(0, 0.15f, 0.5f));
    drawables[drawables.size() - 1]->assignAnimator(new Animator(vec3(0, 0, 0.000001f)));
    drawables[drawables.size() - 1]->anim->setEnabled(true);
}
