#include "scene.hpp"
#include "sceneHandler.hpp"

Scene::Scene(std::string identifier, SceneHandler& manager, sf::Color bgColor) : identifier(identifier), manager(&manager) { 
    background.setFillColor(bgColor);
    auto size = manager.window->getSize();
    background.setSize(sf::Vector2f((float)size.x, (float)size.y));
}

Scene::~Scene() 
{
    
}

void Scene::erase(std::vector<std::shared_ptr<GameObject>>& v, const GameObject& toDelete) 
{
    v.erase(std::remove_if(v.begin(), v.end(), [&](std::shared_ptr<GameObject> obj) { return *obj == toDelete; }), v.end());
}

void Scene::onEnter(std::function<void()> action) 
{
    this->enterAction = action;
}

void Scene::onExit(std::function<void()> action) 
{
    this->exitAction = action;
}

void Scene::addGameObject(const std::shared_ptr<GameObject> object) 
{
    this->listOfGameObjects.push_back(object);
}

void Scene::removeGameObject(const std::shared_ptr<GameObject> object)
{
    this->erase(this->listOfGameObjects, *object);
}

void Scene::handleEvent(const sf::Event& event, sf::RenderWindow& window) 
{
    for(unsigned int i = 0; i < this->listOfGameObjects.size(); i++) {
        this->listOfGameObjects[i]->handleEvent(event, window);
    }
}

void Scene::update() { 

    for(unsigned int i = 0; i < this->listOfGameObjects.size(); i++) {
        if(this->listOfGameObjects[i]->GetActive())
            this->listOfGameObjects[i]->update();
    }
}

void Scene::render(sf::RenderWindow& window) { 
    window.draw(background);

    for(unsigned int i = 0; i < this->listOfGameObjects.size(); i++) {
        if (this->listOfGameObjects[i]->GetActive())
            this->listOfGameObjects[i]->render(window);
    }
}

std::string Scene::getIdentifier() const {
    return this->identifier;
}