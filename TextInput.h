#pragma once
#include "gameObject.hpp";

class TextInput : public GameObject
{
    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    void update() override;
    void render(sf::RenderWindow& window) override;
};

