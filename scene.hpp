#pragma once

#include <string>
#include <SFML/Graphics/RenderWindow.hpp>

#include "gameObject.hpp"

class Scene {
    private:
        const std::string identifier;
        std::vector<GameObject*> listOfGameObjects;

    public:
        Scene(std::string identifier);
        ~Scene();

    public:
        void addGameObject(GameObject& object);
        //void removeGameObject(GameObject& object);
        void update();
        void render(sf::RenderWindow& window);
        void handleEvent(const sf::Event& event, sf::RenderWindow& window);
        //void removeGameObject(GameObject & object);
        std::string getIdentifier() const;
//private:
//    void erase(std::vector<GameObject*>& v, GameObject& num);
};