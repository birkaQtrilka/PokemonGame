#pragma once

#include <map>
#include <stack>
#include <random>
#include <SFML/Graphics/Font.hpp>

#include "character.hpp"
#include "scene.hpp"

class SceneHandler {
public:
    sf::Font mainFont;
    sf::RenderWindow* window;
    const sf::Color black = sf::Color(0,0,0, 255);
    const sf::Color darkColor = sf::Color(71, 82, 94, 255);
    const sf::Color darkGreyColor = sf::Color(132, 146, 166, 255);
    const sf::Color lightGreyColor = sf::Color(129, 144, 165, 255);
    const sf::Color programmerColor = sf::Color(212, 86, 26, 255);
    //const std::string enemyImages[] {"enemy1.png", "enemy2.png"};
    const std::vector<std::string> enemyImages;
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
        void GenerateBattleFile(const Character& player, const int difficulty) const;
};