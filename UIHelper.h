#pragma once
#include <memory>
#include <string>
#include "button.hpp"
#include "textObject.hpp"

class UIHelper
{
public:
    static std::shared_ptr<TextObject> CreateText(const std::string& name,
        sf::Font& font,
        const std::string& str,
        const sf::Vector2f& pos,
        unsigned int charSize,
        const sf::Color& color)
    {
        auto text = std::make_shared<TextObject>(name, font, str);
        text->setPosition(pos);
        text->setCharacterSize(charSize);
        text->setFillColor(color);
        return text;
    }

    static std::shared_ptr<Button> CreateButton(const std::string& name,
        sf::Font& font,
        const std::string& label,
        const sf::Vector2f& size,
        const sf::Color& color,
        const sf::Vector2f& pos)
    {
        auto btn = std::make_shared<Button>(name, font, label, size, color);
        btn->setPosition(pos);
        return btn;
    }
};