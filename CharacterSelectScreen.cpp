#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "CharacterSelectScreen.h"
#include "character.hpp"
#include "sceneHandler.hpp"

extern std::string folderPrefix;


CharacterSelectScreen::CharacterSelectScreen(std::string identifier, SceneHandler& manager) 
    : currentCharacterIndex(0), Scene(identifier, manager, sf::Color(7, 79, 87))
{
    SetUpInterface();
    SetUpBehavior();
}

CharacterSelectScreen::~CharacterSelectScreen()
{
    
}



void CharacterSelectScreen::SetUpInterface()
{
    difficultyTxt.push_back("Guaranteed to win");
    difficultyTxt.push_back("Easy");
    difficultyTxt.push_back("Normal");
    difficultyTxt.push_back("Hard");
    difficultyTxt.push_back("Guaranteed to lose");

    auto windowSize = manager->window->getSize();;
    sf::Vector2f offset((float)windowSize.x / 2 - 100, 34);

    difficculty = std::make_shared<TextObject>("difficultyTxt", manager->mainFont, "Easy");
    difficculty->setPosition(sf::Vector2f(offset.x, offset.y));
    difficculty->setCharacterSize(26);
    difficculty->setFillColor(manager->black);
    
    characterName = std::make_shared<TextObject>( "characterNameText", manager->mainFont, "nothing");
    characterName->setPosition(sf::Vector2f(offset.x, offset.y + 34.0f));
    characterName->setCharacterSize(26);
    characterName->setFillColor(manager->black);

    offset.y = 290;
    hpText = std::make_shared<TextObject>("hpText", manager->mainFont, "HP: -1");
    hpText->setPosition(offset);
    hpText->setCharacterSize(26);
    hpText->setFillColor(manager->black);

    offset.y += 40;
    attackText = std::make_shared<TextObject>("attackText", manager->mainFont, "ATTACK: -1");
    attackText->setPosition(offset);
    attackText->setCharacterSize(26);
    attackText->setFillColor(manager->black);

    offset.y += 40;
    defenseText = std::make_shared<TextObject>("defenseText", manager->mainFont, "DEFENSE: -1");
    defenseText->setPosition(offset);
    defenseText->setCharacterSize(26);
    defenseText->setFillColor(manager->black);
    
    offset.y += 40;
    charLeftBtn = std::make_shared<Button>( "CharLeftBtn", manager->mainFont, "<", sf::Vector2f(90.5f, 40.0f), manager->darkColor);
    charLeftBtn->setPosition(offset);

    charRightBtn = std::make_shared<Button>( "CharLeftBtn", manager->mainFont, ">", sf::Vector2f(90.5f, 40.0f), manager->darkColor);
    charRightBtn->setPosition(sf::Vector2f(offset.x + 95, offset.y));

    offset.y += 55;
    diffLeftBtn = std::make_shared<Button>("DiffLeftBtn", manager->mainFont, "easier", sf::Vector2f(90.5f, 40.0f), manager->darkColor);
    diffLeftBtn->setPosition(offset);

    diffRightBtn = std::make_shared<Button>("DiffRightBtn", manager->mainFont, "harder", sf::Vector2f(90.5f, 40.0f), manager->darkColor);
    diffRightBtn->setPosition(sf::Vector2f(offset.x + 95, offset.y));

    offset.y += 55;
    playBtn = std::make_shared<Button>("quitButton", manager->mainFont, "Play", sf::Vector2f(192.5f, 50.0f), manager->darkColor);
    playBtn->setPosition(offset);

    offset.y += 55;
    quitButton = std::make_shared<Button>("quitButton", manager->mainFont, "QUIT", sf::Vector2f(192.5f, 50.0f), manager->darkColor);
    quitButton->setPosition(offset);

    offset.y += 55;
    backButton = std::make_shared<Button>("backButton", manager->mainFont, "BACK", sf::Vector2f(192.5f, 50.0f), manager->darkColor);
    backButton->setPosition(offset);


    addGameObject(characterName);
    addGameObject(hpText);
    addGameObject(attackText);
    addGameObject(defenseText);
    addGameObject(quitButton);

    addGameObject(charLeftBtn);
    addGameObject(charRightBtn);
    
    addGameObject(diffLeftBtn);
    addGameObject(diffRightBtn);

    addGameObject(playBtn);
    addGameObject(backButton);
    addGameObject(difficculty);
}

void CharacterSelectScreen::SetUpBehavior()
{
    onEnter([&]() 
    {
        auto windowSize = manager->window->getSize();;
        sf::Vector2f offset((float)windowSize.x / 2 - 100, 124);

        std::ifstream characterSelectionFileRead(folderPrefix + "CharacterSelection.txt");
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
            currentCharacter->setPosition(offset);
            currentCharacter->setScale(sf::Vector2f(5.5f, 5.5f));
        }
        characterSelectionFileRead.close();

        characters[currentCharacterIndex]->SetActive(true);
        characters[currentCharacterIndex]->UpdateText(characterName, hpText, attackText, defenseText);
        difficculty->setText(difficultyTxt[currentDifficultyIndex]);
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

    diffLeftBtn->setButtonAction([&]() 
    {
        if (currentDifficultyIndex != 0)
            currentDifficultyIndex--;
        difficculty->setText(difficultyTxt[currentDifficultyIndex]);

    });

    diffRightBtn->setButtonAction([&]() 
    {
        if (currentDifficultyIndex != characters.size() - 1)
            currentDifficultyIndex++;
        difficculty->setText(difficultyTxt[currentDifficultyIndex]);

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
        manager->GenerateBattleFile(*characters[currentCharacterIndex], currentDifficultyIndex);

        manager->changeToScene("BattleScene");

    });

}



