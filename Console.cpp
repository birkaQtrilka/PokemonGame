#include "Console.h"

Console::Console(std::string identifier, sf::Font& font, std::string txt, sf::Vector2f size, sf::Color color, int visibleTxt) :
    GameObject(identifier), font(font), text(txt), visibleText(visibleTxt)
{
    this->shape.setSize(size);
    this->shape.setFillColor(color);
    this->shape.setOutlineColor(sf::Color::Black);
    this->shape.setOutlineThickness(1);

    this->textMesh.setFont(font);
    this->textMesh.setString(txt);
    this->textMesh.setCharacterSize(26);
    this->textMesh.setFillColor(sf::Color::White);

    sf::FloatRect textRect = this->textMesh.getLocalBounds();
    this->textMesh.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

    this->setPosition(sf::Vector2f(0.0f, 0.0f));
}

Console::~Console() { }

void Console::update() { }

void Console::render(sf::RenderWindow& window) {
    window.draw(this->shape);
    window.draw(this->textMesh);
}

void Console::setCharacterSize(const int size) {
    this->textMesh.setCharacterSize(size);
}

void Console::setPosition(const sf::Vector2f position) {
    this->shape.setPosition(position);
    sf::Vector2f size = this->shape.getSize();
    sf::Vector2f centerButton(position.x + (size.x / 2.0f), position.y + (size.y / 2.0f));
    this->textMesh.setPosition(centerButton);
}

void Console::pushText(const std::string& txt)
{
    if (this->log.size() < this->visibleText)
        this->log.push_back(txt);
    else
    {
        //shifting
        std::string temp;
        std::string temp2 = txt;

        for (int i = this->log.size() - 1; i >= 0; i--)
        {
            temp = this->log[i];

            this->log[i] = temp2;

            temp2 = temp;
        }
    }
    std::string newTxt;
    
    for (int i = this->log.size() - 1; i >= 0; i--)
    {
        newTxt += this->log[i] + '\n';
    }
    printf(newTxt.c_str());
    this->textMesh.setString(newTxt);

}
