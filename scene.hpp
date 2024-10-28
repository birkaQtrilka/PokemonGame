#pragma once

#include <string>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <functional>

#include "gameObject.hpp"

class SceneHandler;
class Scene {
    private:
        const std::string identifier;
        std::vector<std::shared_ptr<GameObject>> listOfGameObjects;
        std::function<void()> enterAction;
        std::function<void()> exitAction;
        sf::RectangleShape background;
public:
        SceneHandler* manager;

    public:
        Scene(std::string identifier, SceneHandler& manager, sf::Color);
        ~Scene();


    public:
        void erase(std::vector<std::shared_ptr<GameObject>>& v, const GameObject& toDelete);
        void addGameObject(std::shared_ptr<GameObject> object);
        void removeGameObject(const std::shared_ptr<GameObject> object);
        void onEnter(std::function<void()> action);
        void onExit(std::function<void()> action);

        void update();
        void render(sf::RenderWindow& window);
        void handleEvent(const sf::Event& event, sf::RenderWindow& window);
        std::string getIdentifier() const;
        friend class SceneHandler;
//private:
//    void erase(std::vector<GameObject*>& v, GameObject& num);
};