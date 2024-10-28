#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "CharacterSelectScreen.h"
#include "character.hpp"
#include "sceneHandler.hpp"



CharacterSelectScreen::CharacterSelectScreen(std::string identifier, SceneHandler& manager) : currentCharacterIndex(0), Scene(identifier, manager, sf::Color(7, 79, 87))
{
    SetUpInterface();
    SetUpBehavior();
}

CharacterSelectScreen::~CharacterSelectScreen()
{
    
}



void CharacterSelectScreen::SetUpInterface()
{
    auto windowSize = manager->window->getSize();;
    sf::Vector2f offset((float)windowSize.x / 2 - 100, 0);

    characterName = std::make_shared<TextObject>( "characterNameText", manager->mainFont, "nothing");
    characterName->setPosition(sf::Vector2f(offset.x, 34.0f));
    characterName->setCharacterSize(26);
    characterName->setFillColor(manager->darkColor);

    hpText = std::make_shared<TextObject>( "hpText", manager->mainFont, "HP: -1");
    hpText->setPosition(sf::Vector2f(offset.x, 238.0f + 40));
    hpText->setCharacterSize(26);
    hpText->setFillColor(manager->darkColor);

    attackText = std::make_shared<TextObject>("attackText", manager->mainFont, "ATTACK: -1");
    attackText->setPosition(sf::Vector2f(offset.x, 290.0f + 40));
    attackText->setCharacterSize(26);
    attackText->setFillColor(manager->darkColor);

    defenseText = std::make_shared<TextObject>("defenseText", manager->mainFont, "DEFENSE: -1");
    defenseText->setPosition(sf::Vector2f(offset.x, 345.0f + 40));
    defenseText->setCharacterSize(26);
    defenseText->setFillColor(manager->darkColor);

    charLeftBtn = std::make_shared<Button>( "CharLeftBtn", manager->mainFont, "<", sf::Vector2f(90.5f, 40.0f), manager->darkColor);
    charLeftBtn->setPosition(sf::Vector2f(offset.x, 445.0f));

    charRightBtn = std::make_shared<Button>( "CharLeftBtn", manager->mainFont, ">", sf::Vector2f(90.5f, 40.0f), manager->darkColor);
    charRightBtn->setPosition(sf::Vector2f(offset.x + 95, 445.0f));

    playBtn = std::make_shared<Button>("quitButton", manager->mainFont, "Play", sf::Vector2f(192.5f, 50.0f), manager->darkColor);
    playBtn->setPosition(sf::Vector2f(offset.x, 500.0f ));

    quitButton = std::make_shared<Button>("quitButton", manager->mainFont, "QUIT", sf::Vector2f(192.5f, 50.0f), manager->darkColor);
    quitButton->setPosition(sf::Vector2f(offset.x, 500.0f + 55));

    backButton = std::make_shared<Button>("backButton", manager->mainFont, "BACK", sf::Vector2f(192.5f, 50.0f), manager->darkColor);
    backButton->setPosition(sf::Vector2f(offset.x, 500.0f + 110));


    addGameObject(characterName);
    addGameObject(hpText);
    addGameObject(attackText);
    addGameObject(defenseText);
    addGameObject(quitButton);
    addGameObject(charLeftBtn);
    addGameObject(charRightBtn);
    addGameObject(playBtn);
    addGameObject(backButton);
}

void CharacterSelectScreen::SetUpBehavior()
{
    onEnter([&]() 
    {
        auto windowSize = manager->window->getSize();;
        sf::Vector2f offset((float)windowSize.x / 2 - 100, 0);

        std::ifstream characterSelectionFileRead("CharacterSelection.txt");
        std::string data;
        std::getline(characterSelectionFileRead, data);

        int characterCount = std::stoi(data);

        for (size_t i = 0; i < characterCount; i++)
        {
            if (characterSelectionFileRead.fail()) return;

            std::getline(characterSelectionFileRead, data);

            std::shared_ptr<Character> currentCharacter = std::make_shared<Character>(data);
            characters.push_back(currentCharacter);
            addGameObject(currentCharacter);

            currentCharacter->SetActive(false);
            currentCharacter->setPosition(sf::Vector2f(offset.x, 90.0f));
            currentCharacter->setScale(sf::Vector2f(6.0f, 6.0f));
        }
        characterSelectionFileRead.close();

        characters[currentCharacterIndex]->SetActive(true);
        characters[currentCharacterIndex]->UpdateText(characterName, hpText, attackText, defenseText);
    });
    
    onExit([&]() 
    {
        for (std::shared_ptr<Character> character : characters)
        {
            removeGameObject(character);
        }
        characters.clear();
    });


    charLeftBtn->setButtonAction([&]() {

        int previousIndex = currentCharacterIndex;
        if (currentCharacterIndex != 0)
            currentCharacterIndex--;
        characters[previousIndex]->SetActive(false);
        characters[currentCharacterIndex]->SetActive(true);
        characters[currentCharacterIndex]->UpdateText(characterName, hpText, attackText, defenseText);

    });


    charRightBtn->setButtonAction([&]() 
    {
        int previousIndex = currentCharacterIndex;
        if (currentCharacterIndex != characters.size() - 1)
            currentCharacterIndex++;
        characters[previousIndex]->SetActive(false);
        characters[currentCharacterIndex]->SetActive(true);
        characters[currentCharacterIndex]->UpdateText(characterName, hpText, attackText, defenseText);
    });

    quitButton->setButtonAction([&]() 
    {
        manager->window->close();
    });

    backButton->setButtonAction([&]()
    {
        manager->changeToScene("StartScene");

    });

    playBtn->setButtonAction([&]()
    {
        manager->GenerateBattleFile(*characters[currentCharacterIndex]);

        manager->changeToScene("BattleScene");

    });

}



