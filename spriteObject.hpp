#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "gameObject.hpp"

class SpriteObject : public GameObject {
    private:
        std::string spriteFile;
        sf::Sprite sprite;
        sf::Texture texture;
protected:
    public:
        SpriteObject(std::string identifier, std::string spriteFile);
        SpriteObject(const SpriteObject& other);
        ~SpriteObject();
        
    public:
        void update() override;
        void render(sf::RenderWindow& window) override;
        
        void setPosition(sf::Vector2f position);
        void setScale(sf::Vector2f scale);
        std::string getSpriteFile() const;
};



