#include "TextInput.h"
#include <SFML/Window/Event.hpp>

void TextInput::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::TextEntered) {
        //printf(event.text.unicode);
        //cout << event.text.unicode;
    }
    
}

void TextInput::render(sf::RenderWindow& window) 
{
}

void TextInput::update()
{

}