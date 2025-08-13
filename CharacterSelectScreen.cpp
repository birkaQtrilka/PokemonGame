#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "CharacterSelectScreen.h"
#include "character.hpp"
#include "sceneHandler.hpp"
#include "SceneAssets.h"
#include "UIHelper.h"
#include "BattleScreen.h"

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
    difficultyTxt = { "Guaranteed to win", "Easy", "Normal", "Hard", "Guaranteed to lose" };

    SceneAssets* assets = SceneAssets::GetInstance();
    sf::Vector2u windowSize = manager->window->getSize();
    sf::Vector2f offset((float)windowSize.x / 2 - 100, 34);
    int txtSize = assets->h2FontSize;

    difficculty = UIHelper::CreateText("difficultyTxt", assets->mainFont, "Easy", sf::Vector2f(offset.x, offset.y), txtSize, assets->black);
    characterName = UIHelper::CreateText("characterNameText", assets->mainFont, "nothing", sf::Vector2f(offset.x, offset.y + 34.0f), txtSize, assets->black);

    offset.y = 290;
    hpText = UIHelper::CreateText("hpText", assets->mainFont, "HP: -1", offset, txtSize, assets->black);

    offset.y += 40;
    attackText = UIHelper::CreateText("attackText", assets->mainFont, "ATTACK: -1", offset, txtSize, assets->black);

    offset.y += 40;
    defenseText = UIHelper::CreateText("defenseText", assets->mainFont, "DEFENSE: -1", offset, txtSize, assets->black);

    const sf::Vector2f smallBtnSize(90.5f, 40.0f);
    const sf::Vector2f bigBtnSize(192.5f, 50.0f);

    offset.y += 40;
    charLeftBtn = UIHelper::CreateButton("CharLeftBtn", assets->mainFont, "<", smallBtnSize, assets->darkColor, offset);
    charRightBtn = UIHelper::CreateButton("CharRightBtn", assets->mainFont, ">", smallBtnSize, assets->darkColor, sf::Vector2f(offset.x + 95, offset.y));

    offset.y += 55;
    difficultyLeftBtn = UIHelper::CreateButton("difficultyLeftBtn", assets->mainFont, "easier", smallBtnSize, assets->darkColor, offset);
    difficultyRightBtn = UIHelper::CreateButton("difficultyRightBtn", assets->mainFont, "harder", smallBtnSize, assets->darkColor, sf::Vector2f(offset.x + 95, offset.y));

    offset.y += 55;
    playBtn = UIHelper::CreateButton("playButton", assets->mainFont, "Play", bigBtnSize, assets->darkColor, offset);

    offset.y += 55;
    quitButton = UIHelper::CreateButton("quitButton", assets->mainFont, "QUIT", bigBtnSize, assets->darkColor, offset);

    offset.y += 55;
    backButton = UIHelper::CreateButton("backButton", assets->mainFont, "BACK", bigBtnSize, assets->darkColor, offset);

    addGameObject(characterName);
    addGameObject(hpText);
    addGameObject(attackText);
    addGameObject(defenseText);
    addGameObject(quitButton);

    addGameObject(charLeftBtn);
    addGameObject(charRightBtn);

    addGameObject(difficultyLeftBtn);
    addGameObject(difficultyRightBtn);

    addGameObject(playBtn);
    addGameObject(backButton);
    addGameObject(difficculty);
}

void CharacterSelectScreen::SetUpBehavior()
{
    onEnter([&]() 
    {
        std::ifstream characterSelectionFileRead(folderPrefix + "CharacterSelection.txt");

        DeserealizeCharacters(characterSelectionFileRead);
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
        SelectCharacter();

    });


    charRightBtn->setButtonAction([&]() 
    {
        int previousIndex = currentCharacterIndex;
        if (currentCharacterIndex != characters.size() - 1)
            currentCharacterIndex++;
        characters[previousIndex]->SetActive(false);
        SelectCharacter();

    
    });

    difficultyLeftBtn->setButtonAction([&]() 
    {
        if (currentDifficultyIndex != 0)
            currentDifficultyIndex--;
        difficculty->setText(difficultyTxt[currentDifficultyIndex]);

    });

    difficultyRightBtn->setButtonAction([&]() 
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
        BattleScreen* s = dynamic_cast<BattleScreen*>(manager->GetScene("BattleScene"));
        s->InitBattleFile(characters[currentCharacterIndex], currentDifficultyIndex);
        manager->changeToScene("BattleScene");

    });

}

void CharacterSelectScreen::AddCharacter(std::shared_ptr<Character> character, sf::Vector2f pos)
{
    characters.push_back(character);
    addGameObject(character);

    character->SetActive(false);
    character->setPosition(pos);
    character->setScale(sf::Vector2f(5.5f, 5.5f));
}

void CharacterSelectScreen::SelectCharacter()
{
    characters[currentCharacterIndex]->SetActive(true);
    characters[currentCharacterIndex]->UpdateText(characterName, hpText, attackText, defenseText);
}

void CharacterSelectScreen::DeserealizeCharacters(std::ifstream& file)
{
    auto windowSize = manager->window->getSize();;
    sf::Vector2f offset((float)windowSize.x / 2 - 100, 124);

    std::string line;
    std::getline(file, line);

    int characterCount = std::stoi(line);

    for (size_t i = 0; i < characterCount; i++)
    {
        if (file.fail()) return;

        std::getline(file, line);

        std::shared_ptr<Character> currentCharacter = std::make_shared<Character>(line);
        AddCharacter(currentCharacter, offset);
    }
    file.close();

    SelectCharacter();
    difficculty->setText(difficultyTxt[currentDifficultyIndex]);
}
