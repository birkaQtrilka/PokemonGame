#include "sceneHandler.hpp"

SceneHandler::SceneHandler(Scene& firstScene) 
{
    this->scene = &firstScene;
    if(firstScene.enterAction)
        firstScene.enterAction();
;}

SceneHandler::~SceneHandler() 
{ }

void SceneHandler::render(sf::RenderWindow& window) const {
    this->scene->render(window);
}

void SceneHandler::update() {
    this->scene->update();
}

void SceneHandler::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    this->scene->handleEvent(event, window);
}

void SceneHandler::changeToScene(Scene& scene) 
{
    if (this->scene->exitAction)
        this->scene->exitAction();
    
    this->scene = &scene;

    if(scene.enterAction)
        scene.enterAction();
}


