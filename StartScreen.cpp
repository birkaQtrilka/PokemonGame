#include "StartScreen.h"
#include "sceneHandler.hpp"

StartScreen::StartScreen(std::string identifier, SceneHandler& manager) : Scene(identifier, manager, sf::Color(7, 79, 87))
{
    SetUpInterface();
    SetUpBehavior();
}

StartScreen::~StartScreen()
{

}

void StartScreen::SetUpInterface()
{
    gameTitle = std::make_shared<TextObject>("GameNameText", manager->mainFont, "PogaemonCo");

    gameTitle->setPosition(sf::Vector2f(109.0f, 34.0f));
    gameTitle->setCharacterSize(26);
    gameTitle->setFillColor(manager->programmerColor);

    startNewBtn = std::make_shared<Button>("attackItselfButton", manager->mainFont, "Start new",
        sf::Vector2f(192.5f, 50.0f), manager->darkColor);
    startNewBtn->setPosition(sf::Vector2f(108.0f, 400.0f));

    continueBtn = std::make_shared<Button>( "attackItselfButton", manager->mainFont, "Continue",
        sf::Vector2f(192.5f, 50.0f), manager->darkColor) ;
    continueBtn->setPosition(sf::Vector2f(108.0f, 500.0f));

    quitBtn = std::make_shared<Button>("QuitBtn", manager->mainFont, "Quit",
        sf::Vector2f(192.5f, 50.0f), manager->darkColor);
    quitBtn->setPosition(sf::Vector2f(108.0f, 600.0f));

    addGameObject(gameTitle);
    addGameObject(continueBtn);
    addGameObject(startNewBtn);
    addGameObject(quitBtn);
}

void StartScreen::SetUpBehavior()
{
    startNewBtn->setButtonAction([&]()
    {
        manager->changeToScene("CharacterScene");

    });

    continueBtn->setButtonAction([&]()
    {
        manager->changeToScene("BattleScene");
    });
    
    quitBtn->setButtonAction([&]()
    {
        manager->window->close();
    });
}
