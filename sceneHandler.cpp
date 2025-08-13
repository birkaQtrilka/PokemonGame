#pragma once
#include "sceneHandler.hpp"
#include "DifficultyManager.h"
#include "SceneAssets.h"

extern std::string folderPrefix;

SceneHandler::SceneHandler(sf::RenderWindow& window) : 
    scene(nullptr)
{
    this->window = &window;
}

SceneHandler::~SceneHandler() 
{ 

}

void SceneHandler::render(sf::RenderWindow& window) const {
    this->scene->render(window);
}

void SceneHandler::update() {
    this->scene->update();
}

void SceneHandler::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    this->scene->handleEvent(event, window);
}

void SceneHandler::changeToScene(const std::string& sceneID) 
{
    Scene* newScene = scenes[sceneID];
    if (this->scene != nullptr && this->scene->exitAction)
        this->scene->exitAction();
    
    this->scene = newScene;

    if(newScene->enterAction)
        scene->enterAction();
}

void SceneHandler::addScene(Scene& scene)
{
    this->scenes.emplace(scene.getIdentifier(), &scene);
}

Scene* SceneHandler::GetScene(const std::string& sceneID)
{
    return scenes[sceneID];
}

void SceneHandler::ShutDown()
{
    if (this->scene != nullptr && this->scene->exitAction)
        this->scene->exitAction();
    window->close();

}
