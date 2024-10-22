#pragma once

#include <string>
#include <SFML/Graphics/RenderWindow.hpp>

#include "gameObject.hpp"
#include <functional>

class Scene {
    private:
        const std::string identifier;
        std::vector<GameObject*> listOfGameObjects;
        std::function<void()> enterAction;
        std::function<void()> exitAction;

    public:
        Scene(std::string identifier);
        ~Scene();

    public:
        void addGameObject(GameObject& object);
        //void removeGameObject(GameObject& object);
        void onEnter(std::function<void()> action);
        void onExit(std::function<void()> action);

        void update();
        void render(sf::RenderWindow& window);
        void handleEvent(const sf::Event& event, sf::RenderWindow& window);
        //void removeGameObject(GameObject & object);
        std::string getIdentifier() const;
        friend class SceneHandler;
//private:
//    void erase(std::vector<GameObject*>& v, GameObject& num);
};