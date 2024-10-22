#pragma once

#include <map>
#include <stack>
#include "scene.hpp"

class SceneHandler {
    private:
        Scene* scene;
    public:
        SceneHandler(Scene& firstScene);
        ~SceneHandler();
    public:
        void render(sf::RenderWindow& window) const;
        void update();
        void handleEvent(const sf::Event& event, sf::RenderWindow& window);

        void changeToScene(Scene& scene);
};