#pragma once
#include <SFML/Graphics/Font.hpp>

class SceneAssets
{
private:
    SceneAssets();
    ~SceneAssets() {};

    static SceneAssets* _instance;
public:
    sf::Font mainFont;
    const sf::Color black = sf::Color(0, 0, 0, 255);
    const sf::Color darkColor = sf::Color(71, 82, 94, 255);
    const sf::Color darkGreyColor = sf::Color(132, 146, 166, 255);
    const sf::Color lightGreyColor = sf::Color(129, 144, 165, 255);
    const sf::Color programmerColor = sf::Color(212, 86, 26, 255);
    const std::vector<std::string> enemyImages;
    const int h1FontSize = 42;
    const int h2FontSize = 26;

    static SceneAssets* GetInstance();
    //returns correct path depending on release mode or debug mode
    std::string GetPath(const std::string& rootPath) const;
};

