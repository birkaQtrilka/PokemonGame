#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

#include "gameObject.hpp"

class TextObject : public GameObject {
    private:
        sf::Font& font;
        sf::Text text;
        std::string textStr;
        sf::Vector2f position;
    public:
        TextObject(std::string identifier, sf::Font& font, std::string textStr);
        ~TextObject();
    
        void update() override;
        void render(sf::RenderWindow& window) override;

        std::string getTextStr() const;
        void setText(const std::string textStr);
        void setCharacterSize(const int size);
        void setFillColor(const sf::Color color);
        void setPosition(const sf::Vector2f position);
};


