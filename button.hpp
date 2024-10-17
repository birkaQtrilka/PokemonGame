#pragma once

#include "gameObject.hpp"
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "scene.hpp"
#include <functional>
// typedef std::function<void()> buttonAction;

class Button : public GameObject {

    private:
        sf::Text text;
        sf::Font& font;
        sf::RectangleShape shape;
        std::string buttonText;

        // buttonAction action;
        std::function<void()> action;
    public:
        Button(std::string identifier, sf::Font& font, std::string buttonText, 
                sf::Vector2f size, sf::Color color);
        ~Button();

        void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
        virtual void onClick();

        void update() override;
        void render(sf::RenderWindow& window) override;
        // void setButtonAction(buttonAction action);
        void setButtonAction(std::function<void()> action);
        void setCharacterSize(const int size);
        void setPosition(const sf::Vector2f position);
};

