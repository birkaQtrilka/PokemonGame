#include "scene.hpp"

Scene::Scene(std::string identifier) : identifier(identifier) { }

Scene::~Scene() 
{
    
}

//void Scene::erase(std::vector<GameObject*>& v, GameObject& num) {
//    std::vector<GameObject*> myNumbers;
//    myNumbers.erase(std::remove_if(myNumbers.begin(), myNumbers.end(), [&](GameObject* obj) { return *obj == num; }), myNumbers.end());
//}

void Scene::onEnter(std::function<void()> action) {
    this->enterAction = action;
}

void Scene::onExit(std::function<void()> action) {
    this->exitAction = action;
}

void Scene::addGameObject(GameObject& object) {
    this->listOfGameObjects.push_back(&object);
}

//void Scene::removeGameObject(GameObject& object) 
//{
//    this->erase(listOfGameObjects, object);
//}

void Scene::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
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
    for(unsigned int i = 0; i < this->listOfGameObjects.size(); i++) {
        if (this->listOfGameObjects[i]->GetActive())
            this->listOfGameObjects[i]->render(window);
    }
}

std::string Scene::getIdentifier() const {
    return this->identifier;
}