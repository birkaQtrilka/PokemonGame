#pragma once

#include "gameObject.hpp"
#include "scene.hpp"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <functional>


class Console : 
    public GameObject{
private:
    sf::Text textMesh;
    sf::Font& font;
    sf::RectangleShape shape;
    std::string text;
    std::vector<std::string> log;
    int visibleText;
    int logCount;

    // buttonAction action;
    std::function<void()> action;
public:
    Console(std::string identifier, sf::Font& font, std::string text,
        sf::Vector2f size, sf::Color color, int visibleTxt);
    ~Console();

    void update() override;
    void render(sf::RenderWindow& window) override;
    void setCharacterSize(const int size);
    void setPosition(const sf::Vector2f position);
    void pushText(const std::string& txt);
    void UpdateTxtPosition();
    void clearText();
};

