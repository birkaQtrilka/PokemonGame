#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

class GameObject {
    private: 
        const std::string identifier;
        bool _enabled;
    public:
        GameObject(std::string identifier);
        GameObject(const GameObject& other);

        virtual ~GameObject();
    public:
        void SetActive(bool val);
        bool GetActive() const;
        virtual void handleEvent(const sf::Event& event, sf::RenderWindow& window) { };
        virtual void update() = 0;
        virtual void render(sf::RenderWindow& window) = 0;
        std::string getIdentifier() const;
        bool operator==(const GameObject& other) {
            return other.getIdentifier() == identifier;
        }
};

