#pragma once

#include <map>
#include <stack>
#include <random>


#include "character.hpp"
#include "scene.hpp"

class SceneHandler {
public:
    sf::RenderWindow* window;

    private:
        std::map<std::string, Scene*> scenes;
        Scene* scene;

    public:
        SceneHandler(sf::RenderWindow& window);
        ~SceneHandler();
    public:
        void render(sf::RenderWindow& window) const;
        void update();
        void handleEvent(const sf::Event& event, sf::RenderWindow& window);

        void changeToScene(const std::string& sceneID);
        void addScene(Scene& scene);
        Scene* GetScene(const std::string& sceneID);
        void ShutDown();
};